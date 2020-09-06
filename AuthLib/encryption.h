#pragma once
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

class Encryption {
private:
	static std::string encrypt_string(const std::string& plain_text, const std::string& key, const std::string& iv);
	static std::string decrypt_string(const std::string& cipher_text, const std::string& key, const std::string& iv);
public:
	static std::string sha256(const std::string& plain_text);

	static std::string encrypt(std::string message, std::string enc_key, std::string iv);

	static std::string decrypt(std::string message, std::string enc_key, std::string iv);

	static std::string iv_key();
};

#endif;