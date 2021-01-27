#include "features.h"

#include "../exceptions/all_exceptions.h"

#include "../utils/token.h"
#include "../utils/encryption.h"

std::string		features::Stream::decrypt(std::string key)
{
	if (!valid())
		throw exceptions::GenericException("Invalid stream");

	return Encryption::decrypt(this->encrypted, key, this->iv);
}
bool			features::Stream::valid()
{
	if (hash != Encryption::sha256(this->encrypted + '.' + this->iv))
		return false;

	return (!this->encrypted.empty() && !this->iv.empty() && !this->hash.empty());
}

const bool		features::Authenticate::can_stream()
{
	return this->succeed() && !this->ist.empty();
}

const bool		features::Authenticate::authenticated()
{
	return this->license != nullptr && this->succeed();
}

bool features::Response::succeed()
{
	return _succeed;
}

std::string features::Response::message()
{
	return _message;
}
