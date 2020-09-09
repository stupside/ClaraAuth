#include "wrapper.h"

#include "xor.h"

#pragma region Wrapper

Wrapper::~Wrapper(void) { };

Error Wrapper::ErrorToObject(json JsonObject) {
    Error ErrorObject;

    ErrorObject.m_error = JsonObject[_xor_("error")];
    ErrorObject.m_succeed = JsonObject[_xor_("succeed")];

    return ErrorObject;
}

Product Wrapper::ProductToObject(json JsonObject) {
    Product ProductObject;

    ProductObject.m_description = JsonObject[_xor_("description")];
    ProductObject.m_game_name = JsonObject[_xor_("game_name")];
    ProductObject.m_game_version = JsonObject[_xor_("game_version")];
    ProductObject.m_name = JsonObject[_xor_("name")];
    ProductObject.m_version = JsonObject[_xor_("version")];
    ProductObject.m_status = JsonObject[_xor_("status")];

    return ProductObject;
}

Package Wrapper::PackageToObject(json JsonObject) {
    Package PackageObject;

    PackageObject.m_duration_int = JsonObject[_xor_("duration_int")];
    PackageObject.m_duration_type = JsonObject[_xor_("duration_type")];
    PackageObject.m_name = JsonObject[_xor_("name")];

    return PackageObject;
}

LicenseKey Wrapper::LicenseKeyToObject(json JsonObject) {
    LicenseKey LicenseKeyObject;
    LicenseKeyObject.m_discord = JsonObject[_xor_("discord")];
    LicenseKeyObject.m_customer = JsonObject[_xor_("customer")];
    LicenseKeyObject.m_expiry = JsonObject[_xor_("expiry")];
    LicenseKeyObject.m_key = JsonObject[_xor_("key")];

    return LicenseKeyObject;
}

list<Variable> Wrapper::VariablesToObject(json JsonObject) {
    list<Variable> Variables;
    for (auto& e : JsonObject.items())
        Variables.push_front(Variable::Variable(e.value()[_xor_("name")], e.value()[_xor_("value")]));
    return Variables;
}
#pragma endregion