// AuthClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "AuthLib.h"

#include <Windows.h>
#include <iostream>

#define PRODUCT_CODE ("7c9272aa041e4f1e87df88587545f495")

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
        cout << "------------ Auth Succeed ------------" << endl;
        cout << "Error: " << Response.Error.m_error << endl;

        cout << "Expiry: " << Response.LicenseKey.m_expiry << endl;
        cout << "Product: " << Response.Product.m_name << endl;
        cout << "Package: " << Response.Package.m_name << endl;

        if (!Response.Error.m_succeed)
            return 0; // Key was Expired or Product Paused / Detected / Maintenance.
    }
    else
    {
        cout << "------------ Auth Failed ------------" << endl;
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
