#include <iostream>
#include "dfa.h"



int main(int argc, char** argv) {

    DFA dfa;

    if (dfa.configure("dfa_build_files/dfa.txt") < 0) {
        std::cout << "error configuring dfa from config file" << std::endl;
        return 1;
    }
    else {
        std::cout << "DFA construction succeeded." << std::endl;
    }

    dfa.compute("abccba");
    dfa.compute("aaaaaaaaaaaabbbbbc");
    dfa.compute("abcbabcbabcbabcbabc");
    
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