#ifndef AUTH_H
#define AUTH_H

#include "client.h"
#include "response.h"

using namespace std;

#define AUTH_ENDPOINT ((string) "http://api.tenet.ooo/api/LicenseKey/Process")
#define AUTH_ISSUER ((string) "Clara_Client")
#define AUTH_AUDIENCE ((string) "Clara")
#define AUTH_EXPIRY ((int) 15)

class Auth
{
private:

	void GetSignature();

	bool VerifyToken(string Token);
	string BuildToken();

	Client Client;

	string m_signature;
	const string m_product_code;
public:

	Auth(string product_code) : m_product_code(product_code) { }
	~Auth();

	void ProcessKey(Response& response, string Key);

	struct auth_exception : public std::runtime_error {
		auth_exception()
			: std::runtime_error("\nAuth server is Offline.\n")
		{}
		explicit auth_exception(const std::string& msg)
			: std::runtime_error("\nAuth server error: " + msg + ".\n")
		{}
	};

};

#endif
