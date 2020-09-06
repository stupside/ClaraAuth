#include "encryption.h"

#include <windows.h>

#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <cryptopp/ccm.h>

#pragma comment(lib, "rpcrt4.lib")

std::string Encryption::encrypt_string(const std::string& plain_text, const std::string& key, const std::string& iv) {
	std::string cipher_text;

	try {
		CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption;
		encryption.SetKeyWithIV((CryptoPP::byte*)key.c_str(), key.size(), (CryptoPP::byte*)iv.c_str());

		CryptoPP::StringSource encryptor(plain_text, true,
			new CryptoPP::StreamTransformationFilter(encryption,
				new CryptoPP::Base64Encoder(
					new CryptoPP::StringSink(cipher_text),
					false
				)
			)
		);
	}
	catch (CryptoPP::Exception) {
		throw;
	}

	return cipher_text;
}

std::string Encryption::decrypt_string(const std::string& cipher_text, const std::string& key, const std::string& iv) {
	std::string plain_text;

	try {
		CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
		decryption.SetKeyWithIV((CryptoPP::byte*)key.c_str(), key.size(), (CryptoPP::byte*)iv.c_str());

		CryptoPP::StringSource decryptor(cipher_text, true,
			new CryptoPP::Base64Decoder(
				new CryptoPP::StreamTransformationFilter(decryption,
					new CryptoPP::StringSink(plain_text)
				)
			)
		);
	}
	catch (CryptoPP::Exception) {
		throw;
	}

	return plain_text;
}

std::string Encryption::sha256(const std::string& plain_text) {
	std::string hashed_text;
	
	try{
		CryptoPP::SHA256 hash;

		CryptoPP::StringSource hashing(plain_text, true,
			new CryptoPP::HashFilter(hash,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(hashed_text),
					false
				)
			)
		);
	}
	catch (CryptoPP::Exception) {
		throw;
	}

	return hashed_text;
}

std::string Encryption::encrypt(std::string message, std::string enc_key, std::string iv) {
	enc_key = sha256(enc_key).substr(0, 32);

	return encrypt_string(message, enc_key, sha256(iv).substr(0, 16));
}

std::string Encryption::decrypt(std::string message, std::string enc_key, std::string iv) {
	enc_key = sha256(enc_key).substr(0, 32);

	return decrypt_string(message, enc_key, sha256(iv).substr(0, 16));
}

std::string Encryption::iv_key() {
	UUID uuid = { 0 };
	std::string guid;

	::UuidCreate(&uuid);

	RPC_CSTR szUuid = NULL;
	if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
	{
		guid = (char*)szUuid;
		::RpcStringFreeA(&szUuid);
	}

	return guid.substr(0, 8) + guid.substr(9, 4) + guid.substr(14, 4);
}