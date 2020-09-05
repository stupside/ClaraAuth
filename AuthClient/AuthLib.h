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

	list<string> m_requested_variables;
public:

	Auth(string product_code) : m_product_code(product_code) { }
	~Auth(void);

	void RequestVariables(list<string> Variables);

	bool ProcessKey(Response& response, string Key);
};

#endif
