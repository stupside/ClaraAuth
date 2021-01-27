#ifndef MAPPER_H
#define MAPPER_H

#include "../modules/features.h"

#include <nlohmann/json.hpp>

namespace mappers {
	template<class T>
	class Mapper {
	protected:
		static std::string fix_string(nlohmann::json json) {
			return json.is_null() ? std::string() : json.get<std::string>();
		};
	};

	class UserMapper : Mapper<features::models::User> {
	public:
		static features::models::User map(nlohmann::json json);
	};

	class ProductMapper : Mapper<features::models::Product> {
	public:
		static features::models::Product map(nlohmann::json json);
	};

	class PackageMapper : Mapper<features::models::Package> {
	public:
		static features::models::Package map(nlohmann::json json);
	};

	class LicenseMapper : Mapper<features::models::License> {
	public:
		static features::models::License map(nlohmann::json json);
		static features::models::License::Extension mapExtension(nlohmann::json json);
	};
}

#endif