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

#define PRODUCT_CODE ("701152d938ff4252b556756f766ca2b5")

int main()
{
    Auth Auth(PRODUCT_CODE);

    string Key;
    cout << "Enter a key: ";
    cin >> Key;

    Response Response;
    bool AuthSucceed = Auth.ProcessKey(Response, Key);

    // THEN YOU CAN DO WHATEVER YOU WANT
    if (AuthSucceed) {

        if (Response.Error.m_succeed) { // Key was Expired or Product Paused / Detected / Maintenance.
            cout << GREEN << "------------ Auth Succeed ------------" << RESET << endl;
        }
        else {
            cout << BOLDRED << "------------ Auth Failed ------------" << RESET << endl;
        }

        cout << "Error: " << Response.Error.m_error << endl;

        cout << "Expiry: " << Response.LicenseKey.m_expiry << endl;
        cout << "Product: " << Response.Product.m_name << endl;
        cout << "Package: " << Response.Package.m_name << endl;
    }
    else
    {
        cout << BOLDRED << "------------ Auth Failed ------------" << RESET << endl;
        cout << "Error: " << Response.Error.m_error << endl;
    }
    Sleep(30000);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
