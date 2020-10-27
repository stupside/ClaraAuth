#include "AuthLib.h"

#include "xor.h"

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

#if defined _DEBUG 
#define AUTH_ENDPOINT ((std::string) _xor_("http://localhost:56494/licensekey/process"))
#else 
#define AUTH_ENDPOINT ((std::string) _xor_("http://api.tenet.ooo/licensekeys/process"))
#endif

#define AUTH_ISSUER ((std::string) _xor_("Tenet_Client"))
#define AUTH_AUDIENCE ((std::string) _xor_("Tenet"))
#define AUTH_EXPIRY ((int) 15)

using nlohmann::json;

namespace tenet {
	Auth::~Auth(void) { }

	void Auth::SetSignature() {
		auto b32 = cppcodec::base32_rfc4648::encode(m_product_code);
		auto otp = totp(Bytes::fromBase32(b32), time(nullptr), 0, AUTH_EXPIRY, 6);
		m_security_key = Encryption::sha256(to_string(otp) + Encryption::sha256(m_product_code));
	}

	std::string Auth::GetToken() {
		SetSignature();

		std::string m_iv = Encryption::iv_key();

		json object;
		object[_xor_("client")][_xor_("variables")] = m_requested_variables;
		object[_xor_("client")][_xor_("hwid")] = GetHwid();

		auto encrypted_data = Encryption::encrypt(object.dump(), m_security_key, m_iv);
		auto encrypted_iv = Encryption::encrypt(m_iv, m_product_code, m_security_key);

		std::string Token = jwt::create()
			.set_audience(AUTH_AUDIENCE).set_issuer(AUTH_ISSUER).set_type(_xor_("JWT"))
			.set_issued_at(std::chrono::system_clock::now()).set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ AUTH_EXPIRY })
			.set_payload_claim(_xor_("data"), jwt::claim(encrypted_data))
			.set_payload_claim(_xor_("iv"), jwt::claim(encrypted_iv))
			.sign(jwt::algorithm::hs256{ m_security_key });
		return Token;
	}

	std::string Auth::GetHwid() {

		m_hwid_options.sort();
		m_hwid_options.unique();

		std::string hwid;

		std::string hwid1 = Client::get_computer_sid();
		std::string hwid2;

		if (!m_hwid_options.empty())
		{
			for (HwidOption& HwidOption : m_hwid_options) {
				switch (HwidOption) {
				case HwidOption::Base_Board:
					hwid2 += Client::get_base_board();
				case HwidOption::Computer_Name:
					hwid2 += Client::get_computer_name();
				case HwidOption::Physical_Memory:
					hwid2 += Client::get_physical_memory();
				case HwidOption::Username:
					hwid2 += Client::get_username();
				}
			}
		}
		return Encryption::sha256(Encryption::sha256(hwid) + (hwid2.empty() ? "" : "." + Encryption::sha256(hwid2)));
	}

	void Auth::RequestVariables(std::list<std::string> Variables) {

		Variables.sort();
		Variables.unique();

		auto is_empty_variable = [&](std::string s) { return s.empty(); };

		Variables.remove_if(is_empty_variable);
		m_requested_variables = Variables;
	}

	bool Auth::VerifyToken(std::string Token) {
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
		return true;
	}

	bool Auth::ProcessKey(Response& response, std::string Key) {
		cpr::AsyncResponse  fr = cpr::PostAsync(
			cpr::Url{ AUTH_ENDPOINT },
			cpr::Header{ { _xor_("accept"), _xor_("application/json")} },
			cpr::Parameters{
				{ _xor_("Token"), GetToken().c_str() },
				{ _xor_("Key"), Key.c_str() }
			}
		);

		cpr::Response req = fr.get();

		response.elapsed = req.elapsed;

		if (req.status_code == 429) {
			response.Error = Error(req.text, false);
			return false;
		}

		if (req.status_code != 200) {
			response.Error = Error(req.text.empty() ? req.error.message : req.text, false);
			return false;
		}
		json json = json::parse(req.text.c_str());
		if (json[_xor_("token")] == nullptr) {
			response.Error = Error(_xor_("Token was empty"), false);
			return false;
		}

		if (!VerifyToken(json[_xor_("token")])) {
			response.Error = Error(_xor_("Invalid client signature"), false);
			return false;
		}

		auto decoded = jwt::decode(json[_xor_("token")]);

		std::string encrypted_iv = decoded.get_payload_claim(_xor_("iv")).as_string();
		std::string decrypted_iv = Encryption::decrypt(encrypted_iv, m_product_code, m_security_key);
		std::string encrypted_datas = decoded.get_payload_claim(_xor_("data")).as_string();
		std::string decrypted_datas = Encryption::decrypt(encrypted_datas, m_security_key, decrypted_iv);

		auto datas = json::parse(decrypted_datas);

		Wrapper Wrapper;
		response.Error = Wrapper.ErrorToObject(datas[_xor_("error")]);
		if (datas[_xor_("product")] != nullptr && datas[_xor_("package")] != nullptr && datas[_xor_("licensekey")] != nullptr) {
			response.Product = Wrapper.ProductToObject(datas[_xor_("product")]);
			response.Package = Wrapper.PackageToObject(datas[_xor_("package")]);
			response.LicenseKey = Wrapper.LicenseKeyToObject(datas[_xor_("licensekey")]);
			response.Variables = Wrapper.VariablesToObject(datas[_xor_("variables")]);
			return true;
		}

		response.Error = Error(_xor_("Something went wrong."), false);
		return false;
	}
}
