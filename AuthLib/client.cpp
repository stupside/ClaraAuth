#pragma warning (disable: 4474)
#pragma warning (disable: 4172)
#define _WIN32_WINNT 0x0500

#include "client.h"

#include "md5.h"

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sddl.h>

string Client::GetHwid() {
	HANDLE h_token = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h_token))
	{
		return nullptr;
	}
	DWORD dw_buffer_size = 0;
	if (!GetTokenInformation(h_token, TokenUser, nullptr, 0, &dw_buffer_size) && (GetLastError() !=
		ERROR_INSUFFICIENT_BUFFER))
	{
		CloseHandle(h_token);
		h_token = nullptr;
		return nullptr;
	}
	std::vector<BYTE> buffer;
	buffer.resize(dw_buffer_size);
	const auto p_token_user = reinterpret_cast<PTOKEN_USER>(&buffer[0]);
	if (!GetTokenInformation(h_token, TokenUser, p_token_user, dw_buffer_size, &dw_buffer_size))
	{
		CloseHandle(h_token);
		h_token = nullptr;

		return nullptr;
	}
	if (!IsValidSid(p_token_user->User.Sid))
	{
		CloseHandle(h_token);
		h_token = nullptr;

		return nullptr;
	}
	CloseHandle(h_token);
	h_token = nullptr;

	LPSTR psz_sid = nullptr;

	if (!ConvertSidToStringSidA(p_token_user->User.Sid, &psz_sid))
	{
		return nullptr;
	}

	string psz_sid_str(psz_sid);
	return md5(psz_sid_str);
}