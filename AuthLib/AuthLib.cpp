#include "AuthLib.h"

#include "src/modules/api.h"

#include "src/utils/logging.h"

#include "src/exceptions/all_exceptions.h"

#include "src/utils/encryption.h"

#include "src/utils/hardware.h"

const bool						tenet::Auth::can_authenticate()
{
	return configuration.is_valid() && initialized && !code.empty() && !response->authenticated();
}

const bool						tenet::Auth::is_authenticated()
{
	return configuration.is_valid() && initialized && !code.empty() && response->authenticated() && !key.empty();
}

features::Authenticate			tenet::Auth::authenticate(std::string key, int attempts) {
	if (!can_authenticate())
	{
		if (configuration.debug.activated()) { Logging::log(configuration.debug.get(), "The auth wasn't ready"); }
		throw exceptions::GenericException("Auth not initialized");
	}

	features::Authenticate* response = new features::Authenticate(Api::authenticate(
		key, configuration.hardware.get(), code, configuration.extension.get(), configuration.endpoints.authenticate, attempts));

	this->response = response;

	if (response->succeed())
		this->key = key;

	return *response;
}

features::Stream				tenet::Auth::stream(features::Authenticate& authenticate)
{
	if (!is_authenticated() || !authenticate.authenticated())
		throw exceptions::GenericException("Client not ready");

	features::Stream* response = new features::Stream(Api::stream(authenticate, configuration.endpoints.stream));

	return *response;
}

bool							tenet::Configuration::is_valid() {
	if (!hardware.is_valid())
		return false;

	return true;
}

tenet::Configuration			tenet::Configuration::with_endpoints(tenet::Configuration::Endpoints endpoints)
{
	if (!endpoints.authenticate.empty())
		this->endpoints.authenticate = endpoints.authenticate;

	if (!endpoints.stream.empty())
		this->endpoints.stream = endpoints.stream;

	return *this;
}
tenet::Configuration			tenet::Configuration::with_hardware(tenet::Configuration::Hardware::Base options)
{
	options.sort();
	options.unique();

	std::string value = "";
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

	if (value.empty())
		throw exceptions::GenericException("Empty hwid");

	return with_hardware(Encryption::sha256(value));
}
tenet::Configuration			tenet::Configuration::with_hardware(std::string value)
{
	hardware.value = value;
	return *this;
}
tenet::Configuration			tenet::Configuration::with_variables(std::list<std::string> names)
{
	extension.variables = names;
	return *this;
}
tenet::Configuration			tenet::Configuration::add_variable(std::string name)
{
	extension.variables.push_front(name);
	return *this;
}
tenet::Configuration			tenet::Configuration::with_debug(std::string path) {
	debug.enabled = true;
	debug.path = path;
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