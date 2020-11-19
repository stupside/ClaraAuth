#include "token.h"

#include "../helpers/hotp.h"
#include "../exceptions/GenericException.h"
#include "../utils/encryption.h"

#include <cppcodec/base64_rfc4648.hpp>
#include <cppcodec/base32_rfc4648.hpp>

#include <jwt-cpp/jwt.h>

#define AUTH_ISSUER ((std::string) "Tenet_Client")
#define AUTH_AUDIENCE ((std::string) "Tenet")
#define AUTH_EXPIRY ((int) 15)

std::string Token::verify(std::string token, std::string code)
{
	std::string pass = signature(code);

	if (token.empty())
		throw GenericException("Invalid token");

	token.erase(std::remove(token.begin(), token.end(), '"'), token.end());

	auto decoded = jwt::decode(token);

	try {
		jwt::verify()
			.allow_algorithm(jwt::algorithm::hs256(pass))
			.with_audience(std::set<std::string>{ AUTH_AUDIENCE })
			.with_issuer(AUTH_ISSUER)
			.verify(jwt::decode(token));
	}
	catch (jwt::signature_verification_exception) {
		throw GenericException("Signature verification failed");
	}
	catch (jwt::token_verification_exception) {
		throw GenericException("Token verification failed");
	}
	catch (jwt::signature_generation_exception) {
		throw GenericException("Signature verification failed");
	}
	catch (std::bad_cast) {
		throw GenericException("Something went wrong");
	}
	catch (std::invalid_argument) {
		throw GenericException("Invalid token");
	}

	std::string encrypted_iv = decoded.get_payload_claim("iv").as_string();
	std::string iv = Encryption::decrypt(encrypted_iv, code, pass); // IV

	std::string encrypted_datas = decoded.get_payload_claim("data").as_string();

	return Encryption::decrypt(encrypted_datas, pass, iv); // DATA
}

std::string Token::generate(json object, std::string code)
{
	std::string pass = signature(code);

	std::string m_iv = Encryption::iv_key();

	auto encrypted_data = Encryption::encrypt(object.dump(), pass, m_iv);
	auto encrypted_iv = Encryption::encrypt(m_iv, code, pass);

	std::string Token = jwt::create()
		.set_audience(AUTH_AUDIENCE).set_issuer(AUTH_ISSUER).set_type("JWT")
		.set_issued_at(std::chrono::system_clock::now()).set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ AUTH_EXPIRY })
		.set_payload_claim("data", jwt::claim(encrypted_data))
		.set_payload_claim("iv", jwt::claim(encrypted_iv))
		.sign(jwt::algorithm::hs256{ pass });

	return Token;
}

std::string Token::signature(std::string code)
{
	auto b32 = cppcodec::base32_rfc4648::encode(code);
	auto otp = totp(Bytes::fromBase32(b32), time(nullptr), 0, AUTH_EXPIRY, 6);
	return Encryption::sha256(std::to_string(otp) + Encryption::sha256(code));
}
