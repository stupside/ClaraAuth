#ifndef JSON_MAPPER_H
#define JSON_MAPPER_H

#include "../mappers/jsonMapper.h"

const std::string ResponseMapper::getString(nlohmann::json JsonObject)
{
	return JsonObject.is_null() ? "" : JsonObject.get<std::string>();
}

const tenet::PackageResponse ResponseMapper::mapToPackageResponse(nlohmann::json JsonObject)
{
	return tenet::PackageResponse(JsonObject["Id"].get<int>(),
		getString(JsonObject["Name"]),
		getString(JsonObject["Description"]),
		JsonObject["Duration"].get<int>(),
		JsonObject["Type"].get<int>(),
		JsonObject["Price"].get<int>(),
		JsonObject["MinQuantity"].get<int>()
	);
}

const tenet::ProductResponse ResponseMapper::mapToProductResponse(nlohmann::json JsonObject)
{
	return tenet::ProductResponse(JsonObject["Id"].get<int>(),
		JsonObject["Status"].get<int>(),
		JsonObject["Name"].get<std::string>(),
		getString(JsonObject["Download"]),
		getString(JsonObject["Description"]),
		getString(JsonObject["Version"]),
		getString(JsonObject["Image"])
	);
}

const tenet::ApplicationUserResponse ResponseMapper::mapToApplicationUserResponse(nlohmann::json JsonObject)
{
	return tenet::ApplicationUserResponse(JsonObject["Id"].get<std::string>(), JsonObject["UserName"].get<std::string>());
}

tenet::LicenseKeyResponse ResponseMapper::mapToLicenseKeyResponse(nlohmann::json JsonObject)
{
	tenet::PackageResponse package = mapToPackageResponse(JsonObject["Package"]);
	tenet::ProductResponse product = mapToProductResponse(JsonObject["Product"]);
	tenet::ApplicationUserResponse owner = mapToApplicationUserResponse(JsonObject["Owner"]);
	tenet::ApplicationUserResponse creator = mapToApplicationUserResponse(JsonObject["Creator"]);

	tenet::LicenseKeyExtensionResponse extension = tenet::LicenseKeyExtensionResponse(getString(JsonObject["Extension"]["Customer"]), getString(JsonObject["Extension"]["Notes"]));

	tenet::LicenseKeyResponse response = tenet::LicenseKeyResponse(JsonObject["Id"].get<int>(),
		JsonObject["Key"].get<std::string>(),
		JsonObject["Hwid"].get<std::string>(),
		getString(JsonObject["Generation"]),
		JsonObject["Status"].get<int>(),
		JsonObject["Creation"].get<std::string>(),
		JsonObject["Activation"].get<std::string>(),
		JsonObject["Expiration"].get<std::string>(),
		extension,
		package,
		product,
		owner,
		creator
	);

	return response;
}



#endif