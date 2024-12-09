#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "dfa.h"



int DFA::configure(std::string file_name) {

    std::string line;
    std::ifstream dfa_config_file(file_name);
    int alphabet_size;
    int num_states;

    if (!dfa_config_file.is_open()) {
        perror("file failed to open");
        return -1;
    }

    if ((num_states = extract_states(dfa_config_file)) <= 0) {
        std::cout << "Parse Error : failed to find or extract states from config file" << std::endl;
        return -1; 
    }

    if (extract_init_state(dfa_config_file) < 0) {
        std::cout << "Parse Error : failed to find or extract initial state from config file" << std::endl;
        return -1;
    }

    dfa_machine.set_state(state_to_int(init_state));

    if (extract_acc_states(dfa_config_file) < 0) {
        std::cout << "Parse Error : failed to find or extract accepting states from config file" << std::endl;
        return -1;
    }

    if ((alphabet_size = extract_alphabet(dfa_config_file)) < 0) {
        std::cout << "Parse Error : failed to find or extract alphabet from config file" << std::endl;
        return -1;
    } 

    dfa_machine.alloc_transition_func_mem(num_states, alphabet_size);

    if (extract_transition_function(dfa_config_file) < 0) {
        std::cout << "Parse Error : failed to find or extract transition function from config file" << std::endl;
        return -1;
    } 

    if (!all_possible_transitions_defined()) {
        std::cout << "Parse Error : dfa config file failed to define state transitions for every possible alphabet input on every possible state" << std::endl; 
        return -1;
    }

    return 1;

}

std::string DFA::dfa_cur_state() {
    return int_to_state(dfa_machine.get_state());
}

int DFA::compute(std::string input) {

    std::cout << "inputting string " << input << " into the DFA..." << std::endl;
    std::cout << "DFA state : " << dfa_cur_state() << std::endl;

    for (char c : input) {

        if (!is_valid_alphabet_char(c)) {
            std::cout << "Error : input character not a member of DFA alphabet" << std::endl;
            return -1;
        }

        std::cout << "(" << dfa_cur_state() << ", " << c << ") --> ";
        dfa_machine.step(alphabet_to_int(c));
        std::cout << dfa_cur_state() << std::endl;
    }

    return 1;
}

bool DFA::all_possible_transitions_defined() {
    int** tf = dfa_machine.get_tf();
    for (int i = 0; i < states.size(); ++i) {
        for (int j = 0; j < alphabet.size(); ++j) {
            if (tf[i][j] == -1) return false;
        }
    }
    return true;
}

bool DFA::is_valid_dfa() {
    return all_possible_transitions_defined();
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



/* PARSING ====================================================
=============================================================== */


int parse_by_space(std::vector<std::string>& tokens, std::string str) {
    std::stringstream ss(str);
    std::string word;
    int num_tokens = 0;
    while (ss >> word) {
        tokens.push_back(word);
        num_tokens++;
    }
    return num_tokens;
}  

int DFA::extract_states(std::ifstream& in_file) {

    std::string line;
    std::vector<std::string> read_tokens;

    while (std::getline(in_file, line)) {
        if (line !=  "") break;
    }

    if (in_file.eof()) return -1;

    parse_by_space(read_tokens, line);

    for (std::string token : read_tokens) states.insert(token);

    return states.size();

}

int DFA::extract_init_state(std::ifstream& in_file) {

    std::string line;

    while (std::getline(in_file, line)) {
        if (line != "") break;
    }

    if (in_file.eof()) return -1;
    init_state = line;
    return 1;
}


int DFA::extract_acc_states(std::ifstream& in_file) {

    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(in_file, line)) {
        if (line !=  "") break;
    }

    if (in_file.eof()) return -1;

    parse_by_space(tokens, line);

    for (std::string token : tokens) acc_states.insert(token);

    return acc_states.size();
}



int DFA::extract_alphabet(std::ifstream& in_file) {

    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(in_file, line)) {
        if (line !=  "") break;
    }

    if (in_file.eof()) return -1;

    parse_by_space(tokens, line);

    for (std::string token : tokens) {
        if (token.length() > 0) alphabet.insert(token[0]);
    }

    return alphabet.size();
}



int DFA::extract_transition_function(std::ifstream& in_file) {

    std::string line;
    int** tf = dfa_machine.get_tf();
    std::regex tf_pattern("\\(\\s*([^,]+)\\s*,\\s*(.?)\\s*\\)\\s*->\\s*(.+)");
    std::smatch matches;
    std::streampos tf_line_start;

    /* iterate file pointer to first non-empty line */

    while (!in_file.eof()) {

        tf_line_start = in_file.tellg();
        std::getline(in_file, line);
        if (line != "") break;

    }

    if (in_file.eof()) return -1;

    /* reset file pointer to the beginning of the first non-empty
    line, to circumvent the first while loop consuming the line
    and incrementing the file pointer to the second entry of the
    transition function (aka the second non-empty line) 
    (e.i this is just to make sure the next
    while loop starts with the file pointer pointing to
    the beginning of the the first entry of the transition
    function) */

    in_file.clear();
    in_file.seekg(tf_line_start);

    while (std::getline(in_file, line)) {

        if (line == "") continue;
        if (std::regex_match(line, matches, tf_pattern)) {

            int state_in_id;
            int state_out_id;
            int alphabet_in_id;
            char alphabet_in;

            std::string state_in;
            std::string state_out;
            std::string temp;

            state_in = matches[1];
            temp = matches[2];
            alphabet_in = temp[0]; // e.g cast "a" to 'a'
            state_out = matches[3];

            if (!is_valid_state(state_in)) {
                std::cout << "Parse Error : unknown input state \'" << state_in << "\' found in transition function" << std::endl;
                return -1;
            } 

            if (!is_valid_state(state_out)) {
                std::cout << "Parse Error : unknown output state \'" << state_out << "\' found in transition function" << std::endl;
                return -1;
            }

            if (!is_valid_alphabet_char(alphabet_in)) {
                std::cout << "Parse Error : unknown input alphabet char \'" << alphabet_in << "\' found in transition function" << std::endl;
                return -1;
            }

            state_in_id = state_to_int(state_in);
            state_out_id = state_to_int(state_out);
            alphabet_in_id = alphabet_to_int(alphabet_in);

            tf[state_in_id][alphabet_in_id] = state_out_id;
            
        }
    }

    return 1;

}