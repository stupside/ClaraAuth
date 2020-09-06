#pragma warning (disable: 4474)
#pragma warning (disable: 4172)
#define _WIN32_WINNT 0x0500

#include "client.h"

#include "encryption.h"

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sddl.h>

#include <atlbase.h>
#include <Wbemidl.h>
#include <codecvt>
#include <comdef.h>

#include <future>
#include <Iphlpapi.h>

#pragma comment(lib, "wbemuuid.lib")

string get_processor_name()
{
	int CPUInfo[4] = { -1 };
	char CPUBrandString[0x40];
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];
	memset(CPUBrandString, 0, sizeof(CPUBrandString));
	for (int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	return CPUBrandString;
}

string get_username()
{
	wchar_t name[UNLEN + 1];
	DWORD size = UNLEN + 1;
	std::string UserName;
	if (GetUserNameW(name, &size))
	{
		std::wstring strname(name);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		UserName = conv.to_bytes(strname);
	}
	return UserName;
}

string get_BaseBoard()
{
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE, NULL);
	CComPtr<IWbemLocator> pWbemLocator;
	hr = pWbemLocator.CoCreateInstance(CLSID_WbemLocator);
	CComPtr<IWbemServices> pWbemServices;
	hr = pWbemLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, NULL, &pWbemServices);
	CComPtr<IEnumWbemClassObject> pEnum;
	std::string part1 = ("Select SerialNumber from Win32_BaseBoard");

	CComBSTR cbsQuery(part1.c_str());

	hr = pWbemServices->ExecQuery(_bstr_t("WQL"), cbsQuery, WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);
	ULONG uObjectCount = 0;
	CComPtr<IWbemClassObject> pWmiObject;
	hr = pEnum->Next(WBEM_INFINITE, 1, &pWmiObject, &uObjectCount);
	CComVariant cvtVersion;
	if (uObjectCount != 0) {
		hr = pWmiObject->Get(L"SerialNumber", 0, &cvtVersion, 0, 0);
		std::wstring ws(cvtVersion.bstrVal, SysStringLen(cvtVersion.bstrVal));

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		std::string sOsVersion = conv.to_bytes(ws);
		return sOsVersion;
	}
	return ("Win32_BaseBoardFAILED");
}

string get_PhysicalMemory()
{
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE, NULL);
	CComPtr<IWbemLocator> pWbemLocator;
	hr = pWbemLocator.CoCreateInstance(CLSID_WbemLocator);
	CComPtr<IWbemServices> pWbemServices;
	hr = pWbemLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, NULL, &pWbemServices);
	CComPtr<IEnumWbemClassObject> pEnum;
	std::string part1 = ("Select PartNumber FROM Win32_PhysicalMemory WHERE PartNumber IS NOT NULL");

	CComBSTR cbsQuery(part1.c_str());

	hr = pWbemServices->ExecQuery(_bstr_t("WQL"), cbsQuery, WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);
	ULONG uObjectCount = 0;
	CComPtr<IWbemClassObject> pWmiObject;
	hr = pEnum->Next(WBEM_INFINITE, 1, &pWmiObject, &uObjectCount);
	CComVariant cvtVersion;
	if (uObjectCount != 0) {
		hr = pWmiObject->Get(L"PartNumber", 0, &cvtVersion, 0, 0);
		std::wstring ws(cvtVersion.bstrVal, SysStringLen(cvtVersion.bstrVal));

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		std::string sOsVersion = conv.to_bytes(ws);

		return sOsVersion;
	}
	return ("Win32_PhysicalMemoryFAILED");
}

string get_computer_name()
{
	wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
	DWORD cchBufferSize = sizeof(buffer) / sizeof(buffer[0]);
	if (!GetComputerNameW(buffer, &cchBufferSize))
		return "coputerNameFAILED";

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	std::string ws2s = conv.to_bytes(std::wstring(&buffer[0]));
	return ws2s;
}

string get_computer_sid() {
	HANDLE h_token = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h_token))
	{
		return "sidFAILED";
	}
	DWORD dw_buffer_size = 0;
	if (!GetTokenInformation(h_token, TokenUser, nullptr, 0, &dw_buffer_size) && (GetLastError() !=
		ERROR_INSUFFICIENT_BUFFER))
	{
		CloseHandle(h_token);
		h_token = nullptr;
		return "sidFAILED";
	}
	std::vector<BYTE> buffer;
	buffer.resize(dw_buffer_size);
	const auto p_token_user = reinterpret_cast<PTOKEN_USER>(&buffer[0]);
	if (!GetTokenInformation(h_token, TokenUser, p_token_user, dw_buffer_size, &dw_buffer_size))
	{
		CloseHandle(h_token);
		h_token = nullptr;

		return "sidFAILED";
	}
	if (!IsValidSid(p_token_user->User.Sid))
	{
		CloseHandle(h_token);
		h_token = nullptr;

		return "sidFAILED";
	}
	CloseHandle(h_token);
	h_token = nullptr;

	LPSTR psz_sid = nullptr;

	if (!ConvertSidToStringSidA(p_token_user->User.Sid, &psz_sid))
	{
		return "sidFAILED";
	}

	string psz_sid_str(psz_sid);

	return psz_sid_str;
}

string Client::GetHwid() {

	string hwid1 = get_processor_name() + "." + get_BaseBoard() + "." + get_PhysicalMemory();
	string hwid2 = get_computer_name() + "." + get_username();

	string hwid = Encryption::sha256(hwid1) + '.' + Encryption::sha256(hwid2);
	return hwid;
}