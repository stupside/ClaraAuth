#include "AuthLib.h"

#include <iostream>

#pragma region secret
    #define PRODUCT_CODE ((std::string)"b3f6cec1ea2a4755b2b31bc126b75c71")
#pragma endregion

int main()
{
    tenet::Auth auth(PRODUCT_CODE);

    // Make sure ot have rw access to the path
    auth.with_debug();

    // auth.with_custom_hwid("custom_hwid");
    // OR
    std::list<tenet::HwidOption> options = { 
        tenet::HwidOption::Physical_Memory,
        tenet::HwidOption::Base_Board
    };

    auth.with_hwid(options);

    //auth.with_variables({ "var1", "var2" });

    std::string key;
    std::cout << "Enter a key : "; std::cin >> key;
    key = "83D9E624-0F91-4DA5-BDCA-B40BA5DB1FCB";
    tenet::Response response = auth.process(key, 10);

    if (response.Succeed())
    {
        std::cout << "expiration: " << response.licenseKey->expiration << std::endl;
        // response.Variables()
    }
    else {
        std::cout << response.Message() << std::endl;
    }

    return 0;
}
