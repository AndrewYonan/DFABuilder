#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "dfa.h"
#include "dfa_config_file_parser.h"


void DFA::set_init_state(const std::string& state) {
    init_state = state;
}

void DFA::set_states(const std::set<std::string>& states_set) {
    states = states_set;
}

void DFA::set_accept_states(const std::set<std::string>& acc_states_set) {
    acc_states = acc_states_set;
}

void DFA::set_alphabet(const std::set<char>& alphabet_set) {
    alphabet = alphabet_set;
}

void DFA::set_transition_function(int**& tf, int n_states) {
    dfa_machine.set_tf(tf, n_states);
}

void DFA::set_verbose_mode(int val) {
    verbose_mode = val;
}

void DFA::configure(std::set<std::string>& states,
                    std::string& init_state,
                    std::set<std::string>& acc_states,
                    std::set<char>& alphabet,
                    int**& tf) {
    set_states(states);
    set_init_state(init_state);
    set_accept_states(acc_states);
    set_alphabet(alphabet);
    set_transition_function(tf, states.size());
}

std::string DFA::dfa_cur_state() {
    return int_to_state(dfa_machine.get_state());
}

void DFA::set_dfa_state(std::string state) {
    dfa_machine.set_state(state_to_int(state));
}

bool DFA::in_accepting_state() {
    for (std::string acc_state : acc_states) {
        if (acc_state == dfa_cur_state()) {
            return true;
        }
    }
    return false;
}


bool DFA::compute(std::string input) {

    if (verbose_mode == 2) {

        if (input == "") {
            std::cout << "inputting string <EMPTY STRING> into the DFA..." << std::endl;
        }
        else {
            std::cout << "inputting string \'" << input << "\' into the DFA..." << std::endl;
        }
        std::cout << "DFA state : " << dfa_cur_state() << std::endl;
    }

    set_dfa_state(init_state);

    for (char c : input) {

        if (!is_valid_alphabet_char(c)) {
            std::cout << "Error : input character \'" << c << "\' not a member of DFA alphabet" << std::endl;
            return -1;
        }

        if (verbose_mode == 2) {
            std::cout << "(" << dfa_cur_state() << ", " << c << ") --> ";
            dfa_machine.step(alphabet_to_int(c));
            std::cout << dfa_cur_state() << std::endl;
        }
        else {
            dfa_machine.step(alphabet_to_int(c));
        }
        
    }


    if (in_accepting_state()) {
        if (verbose_mode > 0) {
            std::cout << "\'" << input << "\' ACCEPTED (final state = " << dfa_cur_state() << ")" << std::endl; 
        }
        return true;
    }
    else {
        if (verbose_mode > 0) {
            std::cout << "\'" << input << "\' REJECTED (final state = " << dfa_cur_state() << ")" << std::endl; 
        }
        return false;
    }
}

int DFA::compute_from_file(std::string input_file_name) {

    std::string line;
    std::ifstream in_file(input_file_name);
    int accepted_count = 0;
    int rejected_count = 0;

    if (!in_file.is_open()) {
        std::cout << "Failed to open file \'" << input_file_name << "\'" << std::endl;
        return -1;
    }

    while (std::getline(in_file, line)) {
        if (compute(line)) {
            accepted_count++;
        }
        else {
            rejected_count++;
        }
    } 

    std::cout << "accepted " << accepted_count << " strings " << std::endl;
    std::cout << "rejected " << rejected_count << " strings " << std::endl;

    return 1;
}   

bool DFA::is_valid_state(std::string state) {
    return (state_to_int(state) != -1);
}

bool DFA::is_valid_alphabet_char(char c) {
    return (alphabet_to_int(c) != -1);
}


int DFA::state_to_int(std::string state) {
    auto it = std::find(states.begin(), states.end(), state);
    if (it != states.end()) {
        return std::distance(states.begin(), it);
    }
    return -1;
}

std::string DFA::int_to_state(int id) {
    if (id == -1) return "—";
    auto it = states.begin();
    int idx = 0;
    while (it != states.end() && idx < id) {
        idx++;
        it++;
    }
    if (it != states.end()) return *it;
    return "???";
}

int DFA::alphabet_to_int(char c) {
    auto it = std::find(alphabet.begin(), alphabet.end(), c);
    if (it != alphabet.end()) {
        return std::distance(alphabet.begin(), it);
    }
    return -1;
}

char DFA::int_to_alphabet(int id) {
    auto it = alphabet.begin();
    int idx = 0;
    while (it != alphabet.end() && idx < id) {
        idx++;
        it++;
    }
    if (it != alphabet.end()) return *it;
    return '?';
}

void DFA::print_states() {
    std::cout << "States : ";
    for (std::string state : states) {
        std::cout << "[" << state << "] ";
    }
    std::cout << std::endl;
}


void DFA::print_init_state() {
    std::cout << "Initial state : [" << init_state << "]" << std::endl;
}

void DFA::print_acc_states() {
    std::cout << "Accepting states: ";
    for (std::string acc_state : acc_states) {
        std::cout << "[" << acc_state << "] ";
    }
    std::cout << std::endl;
}

void DFA::print_alphabet() {
    std::cout << "Alphabet : ";
    for (char c : alphabet) {
        std::cout << "[" << c << "] ";
    }   
    std::cout << std::endl;
}

void DFA::print_transition_function() {
    std::cout << "Transition Function: " << std::endl;
    int** tf = dfa_machine.get_tf();
    for (int i = 0; i < states.size(); ++i) {
        for (int j = 0; j < alphabet.size(); ++j) {
            std::cout << "(" << int_to_state(i) << ", "
            << int_to_alphabet(j) << ") --> " << int_to_state(tf[i][j]) << std::endl;
        }
    }
}

void DFA::print_raw_transition_function() {
    std::cout << "Raw Transition Function: " << std::endl;
    int** tf = dfa_machine.get_tf();
    for (int i = 0; i < states.size(); ++i) {
        for (int j = 0; j < alphabet.size(); ++j) {
            std::cout << tf[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


void DFA::print() {
    print_states();
    print_init_state();
    print_acc_states();
    print_alphabet();
    print_transition_function();
}