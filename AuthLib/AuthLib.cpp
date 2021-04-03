#include "AuthLib.h"

#include "src/modules/api.h"

#include "src/utils/logging.h"

#include "src/exceptions/all_exceptions.h"

#include "src/utils/encryption.h"

#include "src/utils/hardware.h"

const tenet::Configuration::Hardware::Base& tenet::Configuration::Hardware::DEFAULT_OPTIONS = { 
	tenet::Configuration::Hardware::Options::Physical_Memory, tenet::Configuration::Hardware::Options::Base_Board 
};

void tenet::Configuration::Hardware::set(const std::string& value)
{
	if (value.empty())
		throw exceptions::GenericException("Empty hwid");

	this->m_value = value;
}

void tenet::Configuration::Hardware::set(const tenet::Configuration::Hardware::Base& options)
{
	std::string value = std::string();
	if (options.empty()) {
		value.append(Encryption::sha256(hardware::computer_sid()));
	}

	else if (!options.empty())
	{
		for (auto& option : options) {
			switch (option) {
			case tenet::Configuration::Hardware::Options::Base_Board:
				value = value.append(hardware::base_board());
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

	this->set(Encryption::sha256(value));
}

const std::string& tenet::Configuration::Endpoints::DEFAULT_URL= "http://auth.tenet.ooo";

const std::string& tenet::Configuration::Endpoints::STREAM = "/api/stream";
const std::string& tenet::Configuration::Endpoints::AUTH = "/api/auth";

tenet::Auth::Auth(const std::string& code)
	: m_context(nullptr), m_code(code)
{
	if (code.empty())
		throw exceptions::GenericException("Invalid product code");
}

tenet::Auth::~Auth() {
	if (m_context != nullptr)
		delete this->m_context;
}

const int tenet::Auth::DEFAULT_ATTEMPS = 2;

const bool tenet::Auth::valid() const
{
	if (this->m_code.empty())
		return false;

	if (this->m_context != nullptr)
		return this->context() != nullptr && this->m_context->succeed();

	return true;
}

const bool tenet::Auth::authenticated() const
{
	if (this->context() == nullptr)
		return false;

	return this->m_context->succeed();
}

const features::Response<features::Authenticate>& tenet::Auth::authenticate(const std::string& key, int attempts) {

	if (!valid())
		throw exceptions::GenericException("Auth is not initialized");

	std::string test = tenet::Configuration::endpoints().auth();

	this->m_context = Api::authenticate(key,
		tenet::Configuration::hardware().get(),
		this->m_code,
		tenet::Configuration::extension().get(),
		test,
		attempts
	);

	return *this->m_context;
}

const features::Response<features::Stream>& tenet::Auth::stream() const
{
	if (this->context() == nullptr || this->context()->canStream())
		throw exceptions::GenericException("Client is not authenticated");

	return Api::stream(*this->context(), tenet::Configuration::endpoints().stream());
}