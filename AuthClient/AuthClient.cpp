#include "auth.h"
#include "features.h"

#include <iostream>

#pragma region config
#define HARDWARE { tenet::Configuration::Hardware::Options::Physical_Memory, tenet::Configuration::Hardware::Options::Base_Board }

#define PRODUCT_CODE "2897a71d64c845c4a36522ca07840ec9"
#define STREAM_SECRET "secret"

#pragma endregion

int main()
{
	std::string key;
	std::cout << "Key : ";
	std::cin >> key;

	tenet::Configuration::debug().enable(false);

	auto& auth = tenet::Auth::init(PRODUCT_CODE);

	auto& authenticate = auth.authenticate(key);
	if (!authenticate.succeed())
	{
		std::cout << authenticate.message() << std::endl;
		return 0;
	}

	std::cout << "product: " << auth.context()->license().product().name() << std::endl;
	std::cout << "package: " << auth.context()->license().package().name() << std::endl;

	/*
	features::Response<features::Stream> stream = auth.stream();
	if (!stream.succeed())
	{
		std::cout << stream.message() << std::endl;
		return 0;
	}

	if (!stream.data()->valid())
		return 0;

	std::cout << stream.data()->descrypte(STREAM_SECRET) << std::endl;
	*/

	return 0;
}
