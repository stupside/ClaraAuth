#include "features.h"

#include "../exceptions/all_exceptions.h"

#include "../utils/token.h"
#include "../utils/encryption.h"

std::string		features::Stream::decrypt(std::string secret)
{
	if (!valid())
		throw exceptions::GenericException("Invalid stream");

	std::string decrypted = Encryption::decrypt(this->encrypted, secret, this->iv);
	return decrypted;
}
bool			features::Stream::valid()
{
	if (this->encrypted.empty() || this->iv.empty() || this->hash.empty())
		return false;

	std::string reqh = Encryption::sha256(this->encrypted + this->iv);

	return this->hash == reqh;
}

const bool		features::Authenticate::can_stream()
{
	return this->succeed() && !this->ist.empty();
}

const bool		features::Authenticate::authenticated()
{
	return this->license != nullptr && succeed();
}

bool features::Response::succeed()
{
	return _succeed;
}

std::string features::Response::message()
{
	return _message;
}
