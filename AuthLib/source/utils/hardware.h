#include <list>
#include <iostream>

class Hardware {
public:
    static std::string get_computer_sid();
    static std::string get_processor_name();
    static std::string get_username();
    static std::string get_base_board();
    static std::string get_physical_memory();
    static std::string get_computer_name();
};