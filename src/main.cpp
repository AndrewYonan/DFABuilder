#include <iostream>
#include "dfa.h"

/* Usage : dfa.compute() accepts a single string,
or a text file containing a string on each line to compute  */

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Error : usage : <prog> <dfa_config_file>" << std::endl;
        return 1;
    }

    DFA dfa;

    if (dfa.configure(argv[1]) < 0) {
        std::cout << "error configuring dfa from config file \'" << argv[1] << "\'" << std::endl;
        return 1;
    }
    else {
        std::cout << "DFA construction succeeded..." << std::endl;
    }

    //dfa.set_verbose_mode(true);
    dfa.compute_from_file("dfa_input_files/strings1.txt");

    return 0;
}


















// state / alphabet to unique integer id conversion TESTS

// std::cout << "TESTING STATE TO INT CONVERSION ============\n";
//     for (int i = 0; i < 5; ++i) {

//         std::string state = dfa.int_to_state(i);
//         std::cout << "dfa_int_to_state(" << i << ") = " << state << std::endl;

//         int id = dfa.state_to_int(state);
//         std::cout << "dfa_state_to_int(" << state << ") = " << id << std::endl;
//     }

//     std::cout << "TESTING ALPHABET TO INT CONVERSION ============\n";

//     for (int i = 0; i < 3; ++i) {

//         char alphabet = dfa.int_to_alphabet(i);
//         std::cout << "dfa_int_to_alphabet(" << i << ") = " << alphabet << std::endl;

//         int id = dfa.alphabet_to_int(alphabet);
//         std::cout << "dfa_alphabet_to_int(" << alphabet << ") = " << id << std::endl;
//     }