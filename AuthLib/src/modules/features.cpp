#include "features.h"

#include "../exceptions/exceptions.h"

#include "../utils/token.h"
#include "../utils/encryption.h"

const std::string& features::Stream::descrypt(const std::string& secret) const
{
	if (!valid())
		throw exceptions::GenericException("Invalid stream");

	std::string decrypted = Encryption::decrypt(this->encrypted(), secret, this->iv());
	return decrypted;
}
bool features::Stream::valid() const
{
	if (this->encrypted().empty() || this->iv().empty() || this->hash().empty())
		return false;

	return this->hash() == Encryption::sha256(this->encrypted() + this->iv());
}

const bool features::Authenticate::canStream() const
{
	return !this->m_ist.empty();
}
