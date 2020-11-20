#include "AuthLib.h"

#include <Windows.h>
#include <iostream>

#pragma region secret
    #define PRODUCT_CODE ((std::string)"2897a71d64c845c4a36522ca07840ec9")
#pragma endregion

int main()
{
    tenet::Auth auth(PRODUCT_CODE);

    std::list<tenet::HwidOption> options 
        = { tenet::HwidOption::Physical_Memory, tenet::HwidOption::Computer_Name, tenet::HwidOption::Base_Board, tenet::HwidOption::Username };

    auth.with_hwid(options);
    auth.with_variables({ "var1", "var2" });

    std::string key;
    std::cout << "Enter a key : "; std::cin >> key;

    tenet::Response response = auth.process(key);

    if (response.Succeed())
    {
        std::cout << "product: " << response.licenseKey->product.name << std::endl;
        std::cout << "package: " << response.licenseKey->package.name << std::endl;
        std::cout << "expiration: " << response.licenseKey->expiration << std::endl;

        for (auto& e : response.Variables()) {
            std::cout << e.first << " = " << e.second << std::endl;
        }
    }
    else {
        std::cout << response.Message() << std::endl;
    }

    Sleep(60000);
    return 0;
}
