#ifndef AUTH_H
#define AUTH_H

#include "response.h"

class Auth
{
private:

	void GetSignature();

	bool VerifyToken(string Token);
	string BuildToken();

	string m_signature;
	const string m_product_code;

	const string m_hwid;
public:

	Auth(string product_code) : m_product_code(product_code) { }
	~Auth();

	bool ProcessKey(Response& response, string Key);

	struct auth_exception : public runtime_error {
		auth_exception()
			: runtime_error("\nAuth server is Offline.\n")
		{}
		explicit auth_exception(const string& msg)
			: runtime_error("\nAuth server error: " + msg + ".\n")
		{}
	};

};

#endif
