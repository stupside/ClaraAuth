#include "AuthLib.h"

#include "xor.h"

#include "source/exceptions/GenericException.h"

#include "source/utils/hardware.h"
#include "source/utils/token.h"
#include "source/utils/encryption.h"
#include "source/mappers/jsonMapper.h"

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <sddl.h>

using nlohmann::json;

#if defined _DEBUG 
#define AUTH_ENDPOINT ((std::string) _xor_("http://localhost:56494/v3/licensekey/process"))
#else 
#define AUTH_ENDPOINT ((std::string) _xor_("http://api.tenet.ooo/v3/licensekeys/process"))
#endif

#define AUTH_ISSUER ((std::string) _xor_("Tenet_Client"))
#define AUTH_AUDIENCE ((std::string) _xor_("Tenet"))
#define AUTH_EXPIRY ((int) 15)

namespace tenet {
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

		std::string hwid1 = Hardware::get_computer_sid();
		std::string hwid2;

		if (!hwid_options.empty())
		{
			for (HwidOption& HwidOption : hwid_options) {
				switch (HwidOption) {
				case HwidOption::Base_Board:
					hwid2 += Hardware::get_base_board();
				case HwidOption::Computer_Name:
					hwid2 += Hardware::get_computer_name();
				case HwidOption::Physical_Memory:
					hwid2 += Hardware::get_physical_memory();
				case HwidOption::Username:
					hwid2 += Hardware::get_username();
				}
			}
		}
		hwid = Encryption::sha256((hwid2.empty() ? "" : "." + Encryption::sha256(hwid2)));
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

	tenet::Response Auth::process(std::string key) {

		if (!init)
			throw GenericException("Auth not initialized");

		json object;
		object[_xor_("client")][_xor_("variables")] = requested_variables;
		object[_xor_("client")][_xor_("hwid")] = hwid;

		std::string token = Token::generate(object, product_code);

		cpr::AsyncResponse  fr = cpr::PostAsync(
			cpr::Url{ AUTH_ENDPOINT },
			cpr::Header{ { _xor_("accept"), _xor_("application/json")} },
			cpr::Parameters{
				{ _xor_("token"), token.c_str() },
				{ _xor_("key"), key.c_str() }
			}
		);

		cpr::Response req = fr.get();

		#if defined _DEBUG 
		std::cout << "text: " << req.text << std::endl;
		std::cout << "status_code: "<< req.status_code << std::endl;
		std::cout << "message: " << req.error.message << std::endl;
		std::cout << "reason: " << req.reason << std::endl;
		#endif

		if (req.status_code != 200) {
			return tenet::Response(req.text.empty() ? "The API can't be reached" : req.text);
		}
		
		std::string data;
		try {
		 	data = Token::verify(req.text, product_code);
		}
		catch(GenericException ex){
			return tenet::Response(ex.what());
		}

		if (data.empty())
			return tenet::Response("Empty data set");

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

		return *response;
	}
}
