#include <string>
using namespace std;

#pragma region Classes
class LicenseKey {
public:
    LicenseKey() = default;
    LicenseKey(string const& key, string const& expiry);
public:
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
#pragma endregion

class Response {
private:
public:
    ~Response(void);
    Error Error;
    Product Product;
    Package Package;
    LicenseKey LicenseKey;
};