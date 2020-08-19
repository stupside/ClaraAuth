#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using nlohmann::json;

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