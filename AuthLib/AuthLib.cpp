#include "AuthLib.h"

#include "xor.h"

#include "src/utils/logging.h"

#include "src/exceptions/EmptyKeyException.h"
#include "src/exceptions/GenericException.h"

#include "src/utils/hardware.h"
#include "src/utils/token.h"
#include "src/utils/encryption.h"
#include "src/mappers/jsonMapper.h"

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <sddl.h>

using nlohmann::json;

#if defined _DEBUG 
#define AUTH_ENDPOINT ((std::string) _xor_("http://localhost:5004/licensekey/authenticate"))
#else 
#define AUTH_ENDPOINT ((std::string) _xor_("http://api.tenet.ooo/v3/licensekey/process"))
#endif

#define AUTH_ISSUER ((std::string) _xor_("Tenet_Client"))
#define AUTH_AUDIENCE ((std::string) _xor_("Tenet"))
#define MAX_ATTEMPTS ((int) 10)

namespace tenet {

	void Auth::with_debug(std::string path)
	{
		debug = true;
		debug_path = path;
	}

	void Auth::with_custom_hwid(std::string custom_hwid) {
		if (!init)
			throw GenericException("Auth not initialized");

		hwid = custom_hwid;
	}

	void Auth::with_hwid(std::list<HwidOption> hwid_options){
		if (!init)
			throw GenericException("Auth not initialized");

		hwid_options.sort();
		hwid_options.unique();

		std::string tmp_hwid;
		if (!hwid_options.empty())
		{
			for (HwidOption& option : hwid_options) {
				switch (option) {
					case HwidOption::Base_Board:
						tmp_hwid += Hardware::get_base_board();
						break;
					case HwidOption::Computer_Name:
						tmp_hwid += Hardware::get_computer_name();
						break;
					case HwidOption::Physical_Memory:
						tmp_hwid += Hardware::get_physical_memory();
						break;
					case HwidOption::Username:
						tmp_hwid += Hardware::get_username();
						break;
					default:
						break;
				}
			}
		}
		else {
			tmp_hwid = "";
		}

		tmp_hwid = Encryption::sha256(tmp_hwid) + Encryption::sha256(Hardware::get_computer_sid());
		hwid = Encryption::sha256(tmp_hwid);
	}

	void Auth::with_variables(std::list<std::string> variables) {
		if (!init)
			throw GenericException("Auth not initialized");

		variables.sort();
		variables.unique();

		auto is_empty_variable = [&](std::string s) { return s.empty(); };

		variables.remove_if(is_empty_variable);
		requested_variables = variables;
	}

	tenet::Response Auth::process(std::string key, int attempts) {
		Logging logging;
		if (attempts > MAX_ATTEMPTS)
			attempts = MAX_ATTEMPTS;

		if(attempts == 0)
			return tenet::Response("Max attempts reached");
		
		--attempts;

		Sleep(2000);

		if (debug)
		{
			logging = Logging::Logging(debug_path);
		}

		if (key.empty())
		{
			if (debug) { logging.log("Empty key"); }
			throw EmptyKeyException("License Key was empty");
		}

		if (!init)
		{
			if (debug) { logging.log("The auth wasn't initialized"); }
			throw GenericException("Auth not initialized");
		}

		json object;
		object[_xor_("client")][_xor_("variables")] = requested_variables;
		object[_xor_("client")][_xor_("hwid")] = hwid;

		std::string token = Token::generate(object, product_code);

		cpr::AsyncResponse  fr = cpr::PostAsync(
			cpr::Url{ AUTH_ENDPOINT },
			cpr::Header{ { _xor_("accept"), _xor_("application/json")}, { _xor_("TN-Key"), key.c_str()} },
			cpr::Parameters{
				{ _xor_("token"), token.c_str() },
				{ _xor_("key"), key.c_str() }
			}
		);

		cpr::Response req;
		req = fr.get();

		#if defined _DEBUG 
			std::cout << "text: " << req.text << std::endl;
			std::cout << "status_code: " << req.status_code << std::endl;
			std::cout << "message: " << req.error.message << std::endl;
			std::cout << "reason: " << req.reason << std::endl;
		#endif

		if(req.status_code == 449 && attempts > 0)
			return Auth::process(key, attempts);
		

		if (req.status_code != 200) {

			if (debug) {
				logging.log(
					"[" + req.reason + " " + std::to_string(req.status_code) + "]" +
					" text: " + req.text +
					" message: " + req.error.message
				);
			}

			if (req.status_code == 429) {
				return tenet::Response(req.text.empty() ? (req.reason.empty() ? "Rate limited" : req.reason) : req.text);
			}
			else if(req.status_code == 520){
				return Auth::process(key, attempts); // 520 => CF error we try again by default
			}
			else {
				return tenet::Response(req.text.empty() ? "The API can't be reached" : req.text);
			}
		}
		
		std::string data;
		try {
		 	data = Token::verify(req.text, product_code);
		}
		catch(GenericException ex){
			if (debug) {
				logging.log(ex.what());
			}

			if (attempts > 0)
			{
				return Auth::process(key, attempts);
			}
			else {
				return tenet::Response(ex.what());
			}
		}

		if (data.empty())
		{
			if (debug) { logging.log("Data set was empty"); }

			if (attempts > 0)
			{
				return Auth::process(key, --attempts);
			}
			else {
				return tenet::Response("Empty data set");
			}
		}

		json datas = json::parse(data);
		json json_licenseKey = datas["LicenseKey"];
		json json_variables = datas["Variables"];

		tenet::Response *response = new tenet::Response(true, "Authentication succeed");

		response->licenseKey = new tenet::LicenseKeyResponse(ResponseMapper::mapToLicenseKeyResponse(json_licenseKey));

		if (!json_variables.is_null())
		{
			for (json json_variable : json_variables) {
				if (!json_variable.is_null()) {
					response->variables.insert({ json_variable["Name"].get<std::string>(), json_variable["Value"].get<std::string>() });
				}
			}
		}

		this->success = true;
		this->key = key;

		return *response;
	}
}
