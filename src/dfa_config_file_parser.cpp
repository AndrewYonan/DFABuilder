#include <regex>
#include <iostream>
#include <sstream>
#include <vector>
#include "dfa_config_file_parser.h"


template<typename T> 
int set_includes(const std::set<T>& set, const T& elem) {
    for (auto it = set.begin(); it != set.end(); it++) {
        if (*it == elem) {
            return true;
        }
    }
    return false;
}

int parse_by_space(std::vector<std::string>& tokens, const std::string& str) {
    std::stringstream ss(str);
    std::string word;
    int num_tokens = 0;
    while (ss >> word) {
        tokens.push_back(word);
        num_tokens++;
    }
    return num_tokens;
}  


int extract_states(std::set<std::string>& states, std::ifstream& in_file) {

    std::string line;
    std::vector<std::string> read_tokens;

    while (std::getline(in_file, line)) if (line !=  "") break;
    if (in_file.eof()) return -1;

    parse_by_space(read_tokens, line);
    for (std::string token : read_tokens) states.insert(token);

    return states.size();

}

int extract_init_state(std::string& init_state, const std::set<std::string>& states, std::ifstream& in_file) {

    std::string line;

    while (std::getline(in_file, line)) if (line != "") break;
    if (in_file.eof()) return -1;

    init_state = line;
    if (!set_includes(states, init_state)) {
        std::cout << "Parse Error : initial state \'" << init_state << "\' not found in set of states." << std::endl;
        return -1;
    }

    return 1;
}


int extract_acc_states(std::set<std::string>& acc_states, const std::set<std::string>& states, std::ifstream& in_file) {

    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(in_file, line)) if (line !=  "") break;
    if (in_file.eof()) return -1;

    parse_by_space(tokens, line);

    for (std::string token : tokens) {
        if (!set_includes(states, token)) {
            std::cout << "Parse Error : accepting state \'" << token << "\' not found in set of states." << std::endl;
            return -1;
        }
        acc_states.insert(token);
    } 

    return acc_states.size();
}



int extract_alphabet(std::set<char>& alphabet, std::ifstream& in_file) {

    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(in_file, line)) if (line !=  "") break;
    if (in_file.eof()) return -1;

    parse_by_space(tokens, line);
    for (std::string token : tokens) {
        if (token.length() > 0) alphabet.insert(token[0]);
    }

    return alphabet.size();
}



int state_to_int_ID(const std::string& state, const std::set<std::string>& states) {
    auto it = std::find(states.begin(), states.end(), state);
    if (it != states.end()) {
        return std::distance(states.begin(), it);
    }
    return -1;
}  


int alphabet_to_int_ID(const char& c, const std::set<char>& alphabet) {
    auto it = std::find(alphabet.begin(), alphabet.end(), c);
    if (it != alphabet.end()) {
        return std::distance(alphabet.begin(), it);
    }
    return -1;   
}



std::string int_ID_to_state(const int& ID, const std::set<std::string>& states) {
    if (ID == -1) return "—";
    auto it = states.begin();
    int idx = 0;
    while (it != states.end() && idx < ID) {
        idx++;
        it++;
    }
    if (it != states.end()) return *it;
    return "???";
}



char int_ID_to_alphabet(const int& ID, const std::set<char>& alphabet) {
    auto it = alphabet.begin();
    int idx = 0;
    while (it != alphabet.end() && idx < ID) {
        idx++;
        it++;
    }
    if (it != alphabet.end()) return *it;
    return '?';
}




int extract_transition_function(int** tf, const std::set<std::string>& states, const std::set<char>& alphabet, std::ifstream& in_file) {

    std::string line;
    std::regex tf_pattern("\\(\\s*([^,]+)\\s*,\\s*(.?)\\s*\\)\\s+.*\\s+(.+)");
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

            if (!set_includes(states, state_in)) {
                std::cout << "Parse Error : unknown input state \'" << state_in << "\' found in transition function" << std::endl;
                return -1;
            } 

            if (!set_includes(states, state_out)) {
                std::cout << "Parse Error : unknown output state \'" << state_out << "\' found in transition function" << std::endl;
                return -1;
            }

            if (!set_includes(alphabet, alphabet_in)) {
                std::cout << "Parse Error : unknown input alphabet char \'" << alphabet_in << "\' found in transition function" << std::endl;
                return -1;
            }

            state_in_id = state_to_int_ID(state_in, states);
            state_out_id = state_to_int_ID(state_out, states);
            alphabet_in_id = alphabet_to_int_ID(alphabet_in, alphabet);


            if (state_in_id < 0 || state_in_id >= states.size()) {
                std::cout << "Prog Error : state_in_id (" << state_in_id << ") not within bounds of tf_matrix" << std::endl; 
                return -1;
            }

            if (state_out_id < 0 || state_out_id >= states.size()) {
                std::cout << "Prog Error : state_out_id (" << state_out_id << ") not within bounds of tf_matrix" << std::endl; 
                return -1;
            }

            if (alphabet_in_id < 0 || alphabet_in_id >= alphabet.size()) {
                std::cout << "Prog Error : alphabet_in_id (" << alphabet_in_id << ") not within bounds of tf_matrix" << std::endl; 
                return -1;
            }

            tf[state_in_id][alphabet_in_id] = state_out_id;
            
        }
    }

    return 1;

}



bool all_possible_transitions_defined(int** tf, const int& n_states, const int& alphabet_size) {
    for (int i = 0; i < n_states; ++i) {
        for (int j = 0; j < alphabet_size; ++j) {
            if (tf[i][j] == -1) return false;
        }
    }
    return true;
}

