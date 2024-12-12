#include <iostream>
#include <string>
#include <cstdlib>



int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "usage : <prog> <dfa_config_file>" << std::endl;
        return 1;
    }

    std::string comp_cmd = "g++ -std=c++11 main.cpp dfa.cpp dfa_machine.cpp -o prog";

}