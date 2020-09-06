// AuthClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#include "AuthLib.h"

#include <Windows.h>
#include <iostream>

#define PRODUCT_CODE ("f6e712c013784f21b0c08c587f1a5d34")

int main()
{
    Auth Auth(PRODUCT_CODE);
    Auth.RequestVariables({ "test1", "test3" });

    string Key;

    cout << "Enter a key : "; cin >> Key;
    Key = "98C81C64-CFBC-4ABC-9E55-ADCC5FB7B930";

    Response Response;
    bool AuthSucceed = Auth.ProcessKey(Response, Key);

    if (!AuthSucceed) {
        cout << BOLDRED << "------------ Auth Failed ------------" << RESET << endl;
        cout << "Error: " << Response.Error.m_error << endl;

        Sleep(60000);
        return 0;
    }

    if (Response.Error.m_succeed) {
        cout << GREEN << "------------ Auth Succeed ------------" << RESET << endl;
    }
    else {
        // Key was Expired or Product Paused / Detected / Maintenance. Means the key should not be valid.
        cout << BOLDRED << "------------ Auth Failed ------------" << RESET << endl;
    }

    cout << "Error: " << Response.Error.m_error << endl;

    cout << "Expiry: " << Response.LicenseKey.m_expiry << endl;
    cout << "Product: " << Response.Product.m_name << endl;
    cout << "Package: " << Response.Package.m_name << endl;

    cout << BOLDRED << "------------ Auth Variables ------------" << RESET << endl;
    for (auto& Variable : Response.Variables)
        cout << Variable.m_name << " => " << Variable.m_value << endl;

    Sleep(60000);
    return 0;
}
