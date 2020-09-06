#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

#if defined _DEBUG 
#define AUTH_ENDPOINT ((string) "http://localhost:56494/api/LicenseKey/Process")
#else 
#define AUTH_ENDPOINT ((string) "http://api.tenet.ooo/api/LicenseKey/Process")
#endif

#define AUTH_ISSUER ((string) "Tenet_Client")
#define AUTH_AUDIENCE ((string) "Tenet")
#define AUTH_EXPIRY ((int) 15)

#include "AuthLib.h"

#include "encryption.h"

#include "client.h"

#include "hotp.h"

#include "wrapper.h"

#include <cppcodec/base64_rfc4648.hpp>
#include <cppcodec/base32_rfc4648.hpp>
#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>
#include <cpr/cpr.h>
#include <sddl.h>

using nlohmann::json;

Auth::~Auth(void) { }

void Auth::SetSignature() {
	auto b32 = cppcodec::base32_rfc4648::encode(m_product_code);
	auto otp = totp(Bytes::fromBase32(b32), time(nullptr), 0, AUTH_EXPIRY, 6);
	m_security_key = Encryption::sha256(to_string(otp) + Encryption::sha256(m_product_code));
}

// product_code, sha256, totp
string Auth::BuildToken() {
	SetSignature();

	string m_iv = Encryption::iv_key();

	json object;
	object["client"]["variables"] = m_requested_variables;
	object["client"]["hwid"] = Client::GetHwid();

	auto encrypted_data = Encryption::encrypt(object.dump(), m_security_key, m_iv);
	auto encrypted_iv = Encryption::encrypt(m_iv, m_product_code, m_security_key);

	string Token = jwt::create()
		.set_audience(AUTH_AUDIENCE).set_issuer(AUTH_ISSUER).set_type("JWT")
		.set_issued_at(std::chrono::system_clock::now()).set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ AUTH_EXPIRY })
		.set_payload_claim("data", jwt::claim(encrypted_data))
		.set_payload_claim("iv", jwt::claim(encrypted_iv))
		.sign(jwt::algorithm::hs256{ m_security_key });

	return Token;
}

void Auth::RequestVariables(list<string> Variables) {
	auto is_empty_variable = [&](string s) { return s.empty(); };
	Variables.remove_if(is_empty_variable);
	m_requested_variables = Variables;
}

bool Auth::VerifyToken(string Token) {
	try {
		jwt::verify()
			.allow_algorithm(jwt::algorithm::hs256(m_security_key))
			.with_audience(std::set<std::string>{ AUTH_AUDIENCE })
			.with_issuer(AUTH_ISSUER)
			.verify(jwt::decode(Token));
	}
	catch (jwt::signature_verification_exception Ex) {
		return false;
	}
	catch (jwt::token_verification_exception Ex) {
		return false;
	}
	catch (jwt::rsa_exception Ex) {
		return false;
	}
	catch (jwt::signature_generation_exception Ex) {
		return false;
	}
	catch (std::bad_cast Ex) {
		return false;
	}
	catch (std::invalid_argument Ex) {
		return false;
	}
	catch (int i) {
		return false;
	}
	return true;
}

bool Auth::ProcessKey(Response& response, string Key) {
	cpr::AsyncResponse  fr = cpr::PostAsync(
		cpr::Url{ AUTH_ENDPOINT },
		cpr::Header{ { "accept", "application/json"} },
		cpr::Parameters{
			{ "Token", BuildToken().c_str() },
			{ "Key", Key.c_str() }
		}
	);
	fr.wait();

	cpr::Response req = fr.get();

	Sleep(2000);

	if (req.status_code == 429) {
		response.Error = Error(req.text, false);
		return false;
	}

	if (req.status_code != 200) {
		response.Error = Error(req.text, false);
		return false;
	}

	json json = json::parse(req.text.c_str());
	if (json["token"] == nullptr) {
		response.Error = Error("Invalid Token.", false);
		return false;
	}

	if (!VerifyToken(json["token"])) {
		response.Error = Error("Invalid Token.", false);
		return false;
	}

	auto decoded = jwt::decode(json["token"]);

	string encrypted_iv = decoded.get_payload_claim("iv").as_string();
	string decrypted_iv = Encryption::decrypt(encrypted_iv, m_product_code, m_security_key);
	string encrypted_datas = decoded.get_payload_claim("data").as_string();
	string decrypted_datas = Encryption::decrypt(encrypted_datas, m_security_key, decrypted_iv);

	auto datas = json::parse(decrypted_datas);

	Wrapper Wrapper;
	response.Error = Wrapper.ErrorToObject(datas["error"]);
	if (datas["product"] != nullptr && datas["package"] != nullptr && datas["licensekey"] != nullptr) {
		response.Product = Wrapper.ProductToObject(datas["product"]);
		response.Package = Wrapper.PackageToObject(datas["package"]);
		response.LicenseKey = Wrapper.LicenseKeyToObject(datas["licensekey"]);
		response.Variables = Wrapper.VariablesToObject(datas["variables"]);
		return true;
	}

	response.Error = Error("Invalid Token.", false);
	return false;
}