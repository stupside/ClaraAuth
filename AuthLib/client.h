#ifndef CLIENT_H
#define CLIENT_H

#include <list>
#include <iostream>
using namespace std;

class Client {
public:
    static string get_computer_sid();
    static string get_processor_name();
    static string get_username();
    static string get_base_board();
    static string get_physical_memory();
    static string get_computer_name();
};

#endif