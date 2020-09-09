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

#include "antidebug.h"
#include "xor.h"

using namespace tenet;
using namespace std;

#define PRODUCT_CODE (_xor_("2897a71d64c845c4a36522ca07840ec9"))

int main()
{
    list<HwidOption> HwidOptions = { HwidOption::Physical_Memory, HwidOption::Computer_Name, HwidOption::Base_Board, HwidOption::Username };
    Auth Auth(PRODUCT_CODE, HwidOptions);
    
    Auth.RequestVariables({ _xor_("var1"), _xor_("var2"), _xor_("var3") });

    string Key;
    cout << "Enter a key : "; cin >> Key;

    Response Response;
    bool AuthSucceed = Auth.ProcessKey(Response, Key);

    { // Protection
        protection_struct* protection{ 0 };
        protection->initialize();
    }


    cout << Response.elapsed << " seconds elasped." << endl;

    if (!AuthSucceed) {
        cout << BOLDRED << "------------ Auth Failed ------------" << RESET << endl;
        cout << BOLDYELLOW << Response.Error.m_error << RESET << endl;
        Sleep(60000);
        return 0;
    }

    if (Response.Error.m_succeed) {
        cout << GREEN << "------------ Auth Succeed ------------" << RESET << endl;
    }
    else {
        // Key was Expired or Product Paused / Detected / Maintenance.
        cout << BOLDRED << "------------ Auth Failed ------------" << RESET << endl;
    }
    cout << BOLDYELLOW << Response.Error.m_error << RESET << endl;
    cout << CYAN << "Welcome, " << Response.LicenseKey.m_customer << " !" << RESET << endl;

    cout << "Expiry: " << Response.LicenseKey.m_expiry << endl;
    cout << "Product: " << Response.Product.m_name << endl;
    cout << "Description: " << Response.Product.m_description << endl;

    cout << "Package: " << Response.Package.m_name << endl;

    cout << BOLDRED << "-------------- Variables -------------" << RESET << endl;
    for (auto& Variable : Response.Variables)
        cout << Variable.m_name << " => " << Variable.m_value << endl;
    Sleep(60000);
    return 0;
}
