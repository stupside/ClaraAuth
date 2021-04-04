#include "api.h"

#define MAX_ATTEMPTS ((int) 10)

#include <nlohmann/json.hpp>

#include "../exceptions/exceptions.h"

#include "../utils/token.h"

#include "../utils/encryption.h"
#include "../mappers/mapper.h"

const features::Response<features::Authenticate>* Api::authenticate(const std::string& key, const std::string& hwid, const std::string& code, std::list<std::string> variables, const std::string& endpoint, int attempts)
{
	json object;
	object["variables"] = variables;
	object["hwid"] = hwid;

	std::string token = token::generate(object, code);

	cpr::Response response = post_req(
		cpr::Url{ endpoint },
		cpr::Parameters{
			{ "token", token.c_str() },
		},
		cpr::Header{
			{"accept","application/json"},
			{"TN-KEY", key.c_str() }
		}, attempts);

	if (response.status_code != 200)
	{
		return new features::Response<features::Authenticate>(response.text);
	}

	std::string data;

	try {
		response.text.erase(std::remove(response.text.begin(), response.text.end(), '"'), response.text.end()); // We need some formating there of the token
		data = token::verify(response.text, code);
	}
	catch (exceptions::TokenException ex)
	{
		return new features::Response<features::Authenticate>(ex.what());
	}

	if (data.empty())
		return new features::Response<features::Authenticate>("Empty datas");

	json datas = json::parse(data);
	json json_license = datas["License"];
	json json_variables = datas["Variables"];

	std::map<std::string, std::string> tmp;

	if (!json_variables.is_null())
	{
		for (json json_variable : json_variables) {
			if (!json_variable.is_null()) {
				tmp.emplace(json_variable["Name"].get<std::string>(), json_variable["Value"].get<std::string>());
			}
		}
	}

	features::models::License license = features::models::License(mappers::LicenseMapper::map(json_license));
			
	const features::Authenticate* authenticate = new features::Authenticate(license, tmp, response.header["TN-IST"]);

	return new features::Response<features::Authenticate>(authenticate);
}

const features::Response<features::Stream>& Api::stream(const features::Authenticate& authenticate, const std::string& endpoint) {

	cpr::Response response = post_req(
		cpr::Url{ endpoint },
		cpr::Parameters{},
		cpr::Header{
			{"TN-KEY", authenticate.license().key().c_str()},
			{"TN-IST", authenticate.m_ist.c_str()}
		}, 1);

	if (response.status_code != 200)
		return features::Response<features::Stream>("Cannot stream from server");

	const features::Stream* stream = new features::Stream(response.text, response.header["i"], response.header["h"]);

	return features::Response<features::Stream>(stream);
}

cpr::Response Api::post_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts)
{
	if (attempts > MAX_ATTEMPTS)
		attempts = MAX_ATTEMPTS;

	//cpr::SslOptions ssl =
	//	cpr::Ssl(cpr::ssl::CaPath{ baseDirPath + "ca.cer" }, cpr::ssl::CertFile{ baseDirPath + "client.cer" },
	//		cpr::ssl::KeyFile{ baseDirPath + "client.key" }, cpr::ssl::VerifyPeer{ false },
	//		cpr::ssl::VerifyHost{ false }, cpr::ssl::VerifyStatus{ false });

	cpr::AsyncResponse  fr = cpr::PostAsync(
		cpr::Url{ endpoint },
		headers,
		parameters
	);

	Sleep(2000);

	cpr::Response req;
	req = fr.get();

#if defined _DEBUG 
	std::cout << "text: " << req.text << std::endl;
	std::cout << "status_code: " << req.status_code << std::endl;
	std::cout << "message: " << req.error.message << std::endl;
	std::cout << "reason: " << req.reason << std::endl;
#endif

	if ((req.status_code == 449 || req.status_code == 520) && attempts > 0)
		return get_req(endpoint, parameters, headers, req.status_code == 449 ? --attempts : attempts);

	return req;
}

cpr::Response Api::get_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts)
{
	if (attempts > MAX_ATTEMPTS)
		attempts = MAX_ATTEMPTS;

	cpr::AsyncResponse  fr = cpr::GetAsync(
		cpr::Url{ endpoint },
		headers,
		parameters
	);

	Sleep(2000);

	cpr::Response req;
	req = fr.get();

#if defined _DEBUG 
	std::cout << "text: " << req.text << std::endl;
	std::cout << "status_code: " << req.status_code << std::endl;
	std::cout << "message: " << req.error.message << std::endl;
	std::cout << "reason: " << req.reason << std::endl;
#endif

	if ((req.status_code == 449 || req.status_code == 520) && attempts > 0)
		return get_req(endpoint, parameters, headers, req.status_code == 449 ? --attempts : attempts);

	return req;
}