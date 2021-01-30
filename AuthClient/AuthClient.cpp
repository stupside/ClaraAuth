#include "auth.h"
#include "features.h"

#include <iostream>

#pragma region config
#define PROXY "http://localhost:5005"
#define TENET "http://localhost:5004"
#define HARDWARE { tenet::Configuration::Hardware::Options::Physical_Memory, tenet::Configuration::Hardware::Options::Base_Board }


#define PRODUCT_CODE "1e22fc2d1f8f480984dc4009e130e739"
#define STREAM_CODE "secret"
#pragma endregion

int main()
{
	std::string key;
	std::cout << "Key : ";
	std::cin >> key;

	//std::cout <<  << std::endl;

	tenet::Configuration config = tenet::Configuration()
		.with_endpoints(PROXY)
		.with_hardware(HARDWARE);

	tenet::Auth auth(PRODUCT_CODE, config);

	features::Authenticate response = auth.authenticate(key, 10);
	if (!response.succeed())
	{
		std::cout << response.message() << std::endl;
		return 0;
	}

	if (!auth.is_authenticated())
		return 0;

	std::cout << "product: " << response.license->product.name << std::endl;
	std::cout << "package: " << response.license->package.name << std::endl;

	features::Stream stream = auth.stream(response);
	if (!stream.succeed())
	{
		std::cout << stream.message() << std::endl;
		return 0;
	}

	if (!stream.valid())
		return 0;

	std::cout << stream.decrypt(STREAM_CODE) << std::endl;

	return 0;
}
