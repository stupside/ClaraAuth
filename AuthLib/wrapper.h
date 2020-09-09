#pragma once
#ifndef WRAPPER_H
#define WRAPPER_H

#include "response.h"
#include <nlohmann/json.hpp>

using namespace tenet;
using nlohmann::json;

class Wrapper {
public:
    ~Wrapper(void);
    Error ErrorToObject(json JsonObject);
    Product ProductToObject(json JsonObject);
    Package PackageToObject(json JsonObject);
    LicenseKey LicenseKeyToObject(json JsonObject);
    list<Variable> VariablesToObject(json JsonObject);
};

#endif;
