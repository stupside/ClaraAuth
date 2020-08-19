#ifndef AUTH_H
#define AUTH_H

#include "response.h"

class Auth
{
private:

	void SetSignature();

	bool VerifyToken(string Token);
	string BuildToken();

	string m_signature;

	const string m_product_code;
	const string m_hwid;
public:

	Auth(string product_code) : m_product_code(product_code) { }
	~Auth();

	bool ProcessKey(Response& response, string Key);
};

#endif
