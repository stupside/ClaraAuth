#include "AuthLib.h"

#include "src/modules/api.h"

#include "src/utils/logging.h"

#include "src/exceptions/all_exceptions.h"

#include "src/utils/encryption.h"

#include "src/utils/hardware.h"

tenet::Configuration::Hardware::Hardware() {

}
tenet::Configuration::Hardware::Hardware(std::string value) {
	if (value.empty())
		throw exceptions::GenericException("Empty hwid");

	this->value = value;
}
tenet::Configuration::Hardware::Hardware(tenet::Configuration::Hardware::Base options)
{
	options.sort();
	options.unique();

	std::string value = std::string();
	if (options.empty()) {
		value.append(Encryption::sha256(hardware::computer_sid()));
	}
	else if (!options.empty())
	{
		for (tenet::Configuration::Hardware::Options& option : options) {
			switch (option) {
			case tenet::Configuration::Hardware::Options::Base_Board:
				value = value.assign(hardware::base_board());
				break;
			case tenet::Configuration::Hardware::Options::Computer_Name:
				value = value.append(hardware::computer_name());
				break;
			case tenet::Configuration::Hardware::Options::Physical_Memory:
				value = value.append(hardware::physical_memory());
				break;
			case tenet::Configuration::Hardware::Options::Username:
				value = value.append(hardware::username());
				break;
			case tenet::Configuration::Hardware::Options::Sid:
				value = value.append(hardware::computer_sid());
				break;
			case tenet::Configuration::Hardware::Options::Processor:
				value = value.append(hardware::processor_name());
				break;
			default:
				break;
			}
		}
	}

	this->value = Encryption::sha256(value);
}

tenet::Configuration::Endpoints::Endpoints()
{
	this->root = DEFAULT_ROOT;
}
tenet::Configuration::Endpoints::Endpoints(std::string url)
{
	if (url.empty())
		throw exceptions::GenericException("Invalid endpoint");



	this->root = url;
}

tenet::Auth::Auth(std::string product_code, tenet::Configuration configuration)
{
	if (product_code.empty())
		throw exceptions::GenericException("Invalid product code");

	this->code = product_code;
	this->configuration = configuration;
	this->response = nullptr;
}

const bool						tenet::Auth::can_authenticate()
{
	if (!configuration.is_valid() || initialized && code.empty())
		return false;

	if (response != nullptr)
		return response->authenticated();

	return true;

}
const bool						tenet::Auth::is_authenticated()
{
	if (!configuration.is_valid() || initialized && code.empty() ||key.empty())
		return false;

	if (response == nullptr)
		return false;

	return response->authenticated();
}
features::Authenticate			tenet::Auth::authenticate(std::string key, int attempts) {
	if (!can_authenticate())
		throw exceptions::GenericException("Auth is not initialized");

	features::Authenticate* response = new features::Authenticate(Api::authenticate(
		key, configuration.hardware.get(), code, configuration.extension.get(), configuration.endpoints.root, attempts));

	this->response = response;

	if (response->succeed())
		this->key = key;

	return *response;
}
features::Stream				tenet::Auth::stream(features::Authenticate& authenticate)
{
	if (!is_authenticated() || !authenticate.authenticated())
		throw exceptions::GenericException("Client is not authenticated");

	features::Stream* response = new features::Stream(Api::stream(authenticate, configuration.endpoints.root));

	return *response;
}

bool							tenet::Configuration::is_valid() {
	if (!hardware.is_valid())
		return false;

	if (endpoints.root.empty())
		return false;

	return true;
}

tenet::Configuration			tenet::Configuration::with_endpoints(std::string root)
{
	this->endpoints = tenet::Configuration::Endpoints(root);
	return *this;
}
tenet::Configuration			tenet::Configuration::with_hardware(tenet::Configuration::Hardware::Base options)
{
	this->hardware = tenet::Configuration::Hardware(options);
	return *this;
}
tenet::Configuration			tenet::Configuration::with_hardware(std::string value)
{
	this->hardware = tenet::Configuration::Hardware(value);
	return *this;
}
tenet::Configuration			tenet::Configuration::with_variables(std::list<std::string> names)
{
	this->extension.variables = names;
	return *this;
}
tenet::Configuration			tenet::Configuration::add_variable(std::string name)
{
	extension.variables.push_front(name);
	return *this;
}
tenet::Configuration			tenet::Configuration::with_debug(std::string path) {
	this->debug.enabled = true;
	this->debug.path = path;
	return *this;
}

bool							tenet::Configuration::Hardware::is_valid() {
	return !value.empty();
}
std::string						tenet::Configuration::Hardware::get() {
	return value;
}
std::list<std::string>			tenet::Configuration::Extension::get()
{
	return variables;
}

std::string						tenet::Configuration::Debug::get()
{
	return path;
}
bool							tenet::Configuration::Debug::activated() {
	return enabled;
}