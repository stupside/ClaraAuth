#include "auth.h"
#include "features.h"

#include <iostream>

#pragma region config
    #define AUTH ((std::string)"http://localhost:5004/auth")
    #define STREAM ((std::string)"http://localhost:49156/auth/stream")

    #define HARDWARE (tenet::Configuration::Hardware::Options::Physical_Memory, tenet::Configuration::Hardware::Options::Base_Board )
    #define PRODUCT_CODE ((std::string)"b3f6cec1ea2a4755b2b31bc126b75c71")
#pragma endregion

int main()
{
    tenet::Configuration config = tenet::Configuration()
        .with_hardware({ HARDWARE })
        .with_endpoints(tenet::Configuration::Endpoints(AUTH, STREAM));

    tenet::Auth auth(PRODUCT_CODE, config);

    std::string key;
    std::cout << "Key : "; std::cin >> key;
    key = "31376974-38E4-483C-9E31-B623BCB4C97A";

    features::Authenticate response = auth.authenticate(key, 10);
    if (response.succeed())
    {
        if (!auth.is_authenticated())
            return 0;

        std::cout << response.license->get_id() << std::endl;
    }
    else {
        std::cout << response.message() << std::endl;
        return 0;
    }

    features::Stream stream = auth.stream(response);
    if (!stream.succeed())
    {
        if(!stream.valid())
            return 0;
    }
    else {
        std::cout << stream.message() << std::endl;
        return 0;
    }


    std::string decrypted = stream.decrypt("supersecret");

    return 0;
}
