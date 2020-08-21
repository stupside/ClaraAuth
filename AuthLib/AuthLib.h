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
public:

	Auth(string product_code) : m_product_code(product_code) { }
	~Auth(void);

	bool ProcessKey(Response& response, string Key);
};

#endif
