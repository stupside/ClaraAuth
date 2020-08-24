#include "response.h"
#include "wrapper.h"

#include <iostream>
#include <nlohmann/json.hpp>


#include <string>
using nlohmann::json;

#pragma region Wrapper

Wrapper::~Wrapper(void) { };

Error Wrapper::ErrorToObject(json JsonObject) {
    Error ErrorObject;

    ErrorObject.m_error = JsonObject["error"];
    ErrorObject.m_succeed = JsonObject["succeed"];

    return ErrorObject;
}

Product Wrapper::ProductToObject(json JsonObject) {
    Product ProductObject;

    ProductObject.m_description = JsonObject["description"];
    ProductObject.m_game_name = JsonObject["game_name"];
    ProductObject.m_game_version = JsonObject["game_version"];
    ProductObject.m_name = JsonObject["name"];
    ProductObject.m_version = JsonObject["version"];
    ProductObject.m_status = JsonObject["status"];

    return ProductObject;
}

Package Wrapper::PackageToObject(json JsonObject) {
    Package PackageObject;

    PackageObject.m_duration_int = JsonObject["duration_int"];
    PackageObject.m_duration_type = JsonObject["duration_type"];
    PackageObject.m_name = JsonObject["name"];

    return PackageObject;
}

LicenseKey Wrapper::LicenseKeyToObject(json JsonObject) {
    LicenseKey LicenseKeyObject;
    LicenseKeyObject.m_discord = JsonObject["discord"];
    LicenseKeyObject.m_customer = JsonObject["customer"];
    LicenseKeyObject.m_expiry = JsonObject["expiry"];
    LicenseKeyObject.m_key = JsonObject["key"];

    return LicenseKeyObject;
}
#pragma endregion