#include "api.h"

#define MAX_ATTEMPTS ((int) 10)

#define AUTH "/api/auth"
#define STREAM "/api/stream"

#include <nlohmann/json.hpp>

#include "../exceptions/all_exceptions.h"

#include "../utils/token.h"

#include "../utils/encryption.h"
#include "../mappers/mapper.h"

features::Authenticate	Api::authenticate(std::string key, std::string hwid, std::string code, std::list<std::string> variables, std::string endpoint, int attempts)
{
	json object;
	object["variables"] = variables;
	object["hwid"] = hwid;

	cpr::Response response = post_req(
		cpr::Url{ endpoint + AUTH },
		cpr::Parameters{
			{ "token", token::generate(object, code).c_str() },
		},
		cpr::Header{
			{"accept","application/json"},
			{"TN-KEY", key.c_str()}
		});

	if (response.status_code != 200)
	{
		return features::Authenticate(response.text.empty() ?
			(response.reason.empty() ? "Something went wrong" : response.reason)
			: response.text);
	}

	std::string data;
	try {
		data = token::verify(response.text, code);
	}
	catch (exceptions::TokenException ex)
	{
		return features::Authenticate(ex.what());
	}

	if (data.empty())
		return features::Authenticate("Empty datas");

	json datas = json::parse(data);
	json json_license = datas["License"];
	json json_variables = datas["Variables"];
	
	features::models::License* license = new features::models::License(mappers::LicenseMapper::map(json_license));

	features::Authenticate* rep = new features::Authenticate(license);

	rep->ist = response.header["TN-IST"];

	if (!json_variables.is_null())
	{
		for (json json_variable : json_variables) {
			if (!json_variable.is_null()) {
				rep->variables->insert({ json_variable["Name"].get<std::string>(), json_variable["Value"].get<std::string>() });
			}
		}
	}
	
	return *rep;
}

features::Stream		Api::stream(features::Authenticate authenticate, std::string endpoint, int attempts) {

	cpr::Response response = post_req(
		cpr::Url{ endpoint + STREAM },
		cpr::Parameters{},
		cpr::Header{
			{"TN-KEY", authenticate.license->key.c_str()},
			{"TN-IST", authenticate.ist.c_str()}
		});

	if (response.status_code != 200)
		return features::Stream("Cannot stream from server");

	std::string encrypted_stream = response.text;
	std::string iv = response.header["i"];
	std::string hash = response.header["h"];

	return features::Stream(encrypted_stream, iv, hash);
}

cpr::Response			Api::post_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts)
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
cpr::Response			Api::get_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts)
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