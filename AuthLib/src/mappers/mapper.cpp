#include "mapper.h"

features::models::User mappers::UserMapper::map(nlohmann::json json) {
	return features::models::User(fix_string(json["Id"]), fix_string(json["UserName"]));
}

features::models::Product mappers::ProductMapper::map(nlohmann::json json) {
	return features::models::Product(
		/*id*/			json["Id"].get<int>(),
		/*status*/		features::models::Product::Status(json["Status"].get<int>()),
		/*name*/		fix_string(json["Name"]),
		/*download*/	fix_string(json["Download"]),
		/*description*/	fix_string(json["Description"]),
		/*version*/		fix_string(json["Version"]),
		/*image*/		fix_string(json["Image"])
	);
}

features::models::Package mappers::PackageMapper::map(nlohmann::json json) {
	return features::models::Package(
		/*id*/			json["Id"].get<int>(),
		/*name*/		fix_string(json["Name"]),
		/*description*/	fix_string(json["Description"]),
		/*duration*/	json["Duration"].get<int>(),
		/*type*/		features::models::Package::Type(json["Type"].get<int>()),
		/*price*/		json["Price"].get<float>()
	);
}

features::models::License::Extension mappers::LicenseMapper::mapExtension(nlohmann::json json) {
	return features::models::License::Extension(
		/*customer*/	fix_string(json["Customer"]),
		/*notes*/		fix_string(json["Notes"]),
		/*claimed*/		json["Claimed"].get<bool>()
	);
}

features::models::License mappers::LicenseMapper::map(nlohmann::json json) {

	features::models::License::Extension extension = LicenseMapper::mapExtension(json["Extension"]);
	features::models::Product product = ProductMapper::map(json["Product"]);
	features::models::Package package = PackageMapper::map(json["Package"]);
	features::models::User owner = UserMapper::map(json["Owner"]);
	features::models::User creator = UserMapper::map(json["Creator"]);

	return features::models::License(
		/*id*/			json["Id"].get<int>(),
		/*key*/			fix_string(json["Key"]),
		/*hwid*/		fix_string(json["Hwid"]),
		/*status*/		features::models::License::Status(json["Status"].get<int>()),
		/*creation*/	fix_string(json["Creation"]),
		/*activation*/	fix_string(json["Activation"]),
		/*expiration*/	fix_string(json["Expiration"]),
		/*extension*/	extension,
		/*extesnion*/	product,
		/*package*/		package,
		/*owner*/		owner,
		/*creator*/		creator
	);
}
