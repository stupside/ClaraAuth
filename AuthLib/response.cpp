#include "response.h"
#pragma region Classes
#pragma region LicenseKey
LicenseKey::LicenseKey(string const& key, string const& expiry)
    : m_key(key), m_expiry(expiry) {}
#pragma endregion

#pragma region Package
Package::Package(string const& name, int duration_int, string const& duration_type)
    : m_name(name), m_duration_int(duration_int), m_duration_type(duration_type) {}
#pragma endregion

#pragma region Product
Product::Product(string const& name, string const& version, string const& game_name, string const& game_version, string const& description, string const& status)
    : m_name(name), m_version(version), m_game_name(game_name), m_game_version(game_version), m_description(description), m_status(status) {}
#pragma endregion

#pragma region Error
Error::Error(string const& error, bool succeed)
    : m_error(error), m_succeed(succeed) {}
#pragma endregion
#pragma endregion

Response::~Response() {}