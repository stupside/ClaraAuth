#include "token.h"

#include "../exceptions/all_exceptions.h"

#include "../helpers/hotp.h"

#include "encryption.h"

#include <jwt-cpp/jwt.h>

#define AUTH_ISSUER "Tenet_Client"
#define AUTH_AUDIENCE "Tenet"
#define AUTH_EXPIRY 60

std::string token::verify(std::string token, std::string code) {

	std::string pass = Encryption::pass(code);

	if (token.empty())
		throw exceptions::TokenException("Invalid token");

	token.erase(std::remove(token.begin(), token.end(), '"'), token.end());

	auto decoded = jwt::decode(token);

	try {
		auto verify = jwt::verify()
			.allow_algorithm(jwt::algorithm::hs256{ pass })
			.with_audience(std::set<std::string>({ AUTH_AUDIENCE }))
			.with_issuer({ AUTH_ISSUER });


		verify.verify(decoded);
	}
	catch (jwt::signature_verification_exception) {
		throw exceptions::TokenException("Signature verification failed");
	}
	catch (jwt::token_verification_exception) {
		throw exceptions::TokenException("Token verification failed");
	}
	catch (jwt::signature_generation_exception) {
		throw exceptions::TokenException("Signature generation failed");
	}
	catch (std::bad_cast ex) {
		throw exceptions::TokenException("Token Bad cast");
	}
	catch (std::invalid_argument) {
		throw exceptions::TokenException("Invalid token");
	}

	std::string encrypted_iv = decoded.get_payload_claim("iv").as_string();
	std::string encrypted_datas = decoded.get_payload_claim("data").as_string();

	std::string decrypted = Encryption::decrypt_routine(encrypted_iv, encrypted_datas, code, pass);
	return decrypted; // DATA
}

std::string token::generate(json object, std::string code)
{
	std::string encrypted_iv;
	std::string pass;
	std::string encrypted_datas = Encryption::encrypt_routine(code, object.dump(), encrypted_iv, pass);

	std::string token = jwt::create()
		.set_audience(AUTH_AUDIENCE).set_issuer(AUTH_ISSUER).set_type("JWT")
		.set_issued_at(std::chrono::system_clock::now()).set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ AUTH_EXPIRY })
		.set_payload_claim("data", jwt::claim(encrypted_datas))
		.set_payload_claim("iv", jwt::claim(encrypted_iv))
		.sign(jwt::algorithm::hs256{ pass });

	return token;
}
