#ifndef RESPONSE_H
#define RESPONSE_H

#include <list> 
#include <string>
using namespace std;

namespace tenet {
#pragma region Classes
    class LicenseKey {
    public:
        LicenseKey() = default;
        LicenseKey(string const& key, string const& expiry, string const& discord, string const& customer);
    public:
        string m_discord;
        string m_customer;
        string m_key;
        string m_expiry;
    };

    class Package {
    public:
        Package() = default;
        Package(string const& name, int duration_int, string const& duration_type);
    public:
        string m_name;
        int m_duration_int = 0;
        string m_duration_type;
    };

    class Product {
    public:
        Product() = default;
        Product(string const& name, string const& version, string const& game_name, string const& game_version, string const& description, string const& status);
    public:
        string m_name;
        string m_version;
        string m_game_name;
        string m_game_version;
        string m_description;
        string m_status;
    };

    class Error {
    public:
        Error() = default;
        Error(string const& error, bool succeed);
    public:
        string m_error;
        bool m_succeed;
    };

    class Variable {
    public:
        Variable() = default;
        Variable(string const& name, string const& value);
    public:
        string m_name;
        string m_value;
    };

#pragma endregion

    class Response {
    private:
    public:
        ~Response(void);

        double elapsed;

        Error Error;
        Product Product;
        Package Package;
        LicenseKey LicenseKey;
        list<Variable> Variables;
    };
}

#endif