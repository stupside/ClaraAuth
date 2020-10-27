#include "AuthLib.h"

#include <Windows.h>
#include <iostream>

#include "antidebug.h"
#include "xor.h"

using namespace std;

#define PRODUCT_CODE (_xor_("2897a71d64c845c4a36522ca07840ec9"))

int main()
{
    list<tenet::HwidOption> HwidOptions = { tenet::HwidOption::Physical_Memory, tenet::HwidOption::Computer_Name, tenet::HwidOption::Base_Board, tenet::HwidOption::Username };
    tenet::Auth Auth(PRODUCT_CODE, HwidOptions);
    
    Auth.RequestVariables({ _xor_("var1"), _xor_("var2"), _xor_("var3") });

    string Key;
    cout << "Enter a key : "; cin >> Key;

    tenet::Response Response;
    bool AuthSucceed = Auth.ProcessKey(Response, Key);

    {
        protection_struct* protection{ 0 };
        protection->initialize();
    }

    cout << Response.elapsed << " seconds elasped." << endl;
    if (!AuthSucceed) { // FIRST CHECK !
        cout << "------------ Auth Failed ------------" << endl;
        cout << Response.Error.m_error << endl;
        Sleep(60000);
        return 0;
    }

    if (Response.Error.m_succeed) { // SECOND CHECK !
        cout << "------------ Auth Succeed ------------" << endl;
    }
    else { // Key was Expired or Product Paused / Detected / Maintenance.
        cout << "------------ Auth Failed ------------" << endl;
    }
    cout << Response.Error.m_error << endl;
    cout << "Expiry: " << Response.LicenseKey.m_expiry << endl;
    cout << "Product: " << Response.Product.m_name << endl;
    cout << "Package: " << Response.Package.m_name << endl;

    cout << "-------------- Variables -------------" << endl;
    for (auto& Variable : Response.Variables)
        cout << Variable.m_name << " => " << Variable.m_value << endl;
    Sleep(60000);
    return 0;
}
