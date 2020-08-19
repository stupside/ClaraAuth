#include "client.h"

#include "md5.h"

#include <cppcodec/base64_rfc4648.hpp>
#include <cppcodec/base32_rfc4648.hpp>
#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>
#include <cpr/cpr.h>
#include <sddl.h>
#include <iostream>
#include <nlohmann/json.hpp>

#include <string>
using nlohmann::json;

Client::Client() {
	SetHwid();
}

Client::~Client(){}

Client::Client(string hwid)
    : m_hwid(hwid) {}

void Client::SetHwid() {
	HANDLE h_token = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h_token))
	{
		return;
	}
	DWORD dw_buffer_size = 0;
	if (!GetTokenInformation(h_token, TokenUser, nullptr, 0, &dw_buffer_size) && (GetLastError() !=
		ERROR_INSUFFICIENT_BUFFER))
	{
		CloseHandle(h_token);
		return;
	}
	std::vector<BYTE> buffer;
	buffer.resize(dw_buffer_size);
	const auto p_token_user = reinterpret_cast<PTOKEN_USER>(&buffer[0]);
	if (!GetTokenInformation(h_token, TokenUser, p_token_user, dw_buffer_size, &dw_buffer_size))
	{
		CloseHandle(h_token);
		return;
	}

	if (!IsValidSid(p_token_user->User.Sid))
	{
		CloseHandle(h_token);
		return;
	}
	CloseHandle(h_token);

	LPTSTR psz_sid = nullptr;
	if (!ConvertSidToStringSid(p_token_user->User.Sid, &psz_sid)) {
		return;
	}
	
	this->m_hwid = md5(string((char*)psz_sid));
}