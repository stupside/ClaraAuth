#include "response.h"

#pragma region Classes
#pragma region LicenseKey
LicenseKey::LicenseKey(std::string const& key, std::string const& expiry, std::string const& discord, std::string const& customer)
    : m_key(key), m_expiry(expiry), m_customer(customer), m_discord(discord) {}
#pragma endregion

#pragma region Package
Package::Package(std::string const& name, int duration_int, std::string const& duration_type)
    : m_name(name), m_duration_int(duration_int), m_duration_type(duration_type) {}
#pragma endregion

#pragma region Product
Product::Product(std::string const& name, std::string const& version, std::string const& game_name, std::string const& game_version, std::string const& description, std::string const& status)
    : m_name(name), m_version(version), m_game_name(game_name), m_game_version(game_version), m_description(description), m_status(status) {}
#pragma endregion

#pragma region Error
Error::Error(std::string const& error, bool succeed)
    : m_error(error), m_succeed(succeed) {}
#pragma endregion

#pragma region Variables
Variable::Variable(std::string const& name, std::string const& value)
    : m_name(name), m_value(value) {}
#pragma endregion

#pragma endregion

Response::~Response(void) {}