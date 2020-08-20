// AuthClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "AuthLib.h"
#include <iostream>

int main()
{
    Auth Auth("a1b094f6d27a4e46b33b076fd2ad7cb4");

    string Key = "C458F372-BCFB-4BDB-9BE1-63BC6525E671";
    
    Response Response;
    if (Auth.ProcessKey(Response, Key)) {
        cout << Response.LicenseKey.m_expiry << endl;
        cout << "Auth Succeed." << endl;
    }
    else
    {
        cout << Response.Error.m_error << endl;
        cout << "Auth Failed." << endl;
    }
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
