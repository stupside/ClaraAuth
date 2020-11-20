#include "../models/responses.h"
#include <nlohmann/json.hpp>

class ResponseMapper {
private:
	static const std::string getString(nlohmann::json JsonObject);
public:
	static const tenet::PackageResponse mapToPackageResponse(nlohmann::json JsonObject);
	static const tenet::ProductResponse mapToProductResponse(nlohmann::json JsonObject);
	static const tenet::ApplicationUserResponse mapToApplicationUserResponse(nlohmann::json JsonObject);
	static tenet::LicenseKeyResponse mapToLicenseKeyResponse(nlohmann::json JsonObject);
};