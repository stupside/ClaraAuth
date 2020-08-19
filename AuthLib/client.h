#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
using namespace std;

class Client {
private:
    void SetHwid();
public:
    Client();
    ~Client();

    Client(string hwid);
public:
    string m_hwid;
};

#endif