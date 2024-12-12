#include <fstream>
#include <set>
#include <string>
#include <vector>


template<typename T>
int set_includes(const std::set<T>&, const T&);

int parse_by_space(std::vector<std::string>& tokens, const std::string& str);

int extract_states(std::set<std::string>& states, std::ifstream& in_file);

int extract_init_state(std::string& init_state, const std::set<std::string>& states, std::ifstream& in_file);

int extract_acc_states(std::set<std::string>& acc_states, const std::set<std::string>& states, std::ifstream& in_file);

int extract_alphabet(std::set<char>& alphabet, std::ifstream& in_file);

int extract_transition_function(int** tf, const std::set<std::string>& states, const std::set<char>& alphabet, std::ifstream& in_file);

bool all_possible_transitions_defined(int** tf, const int& n_states, const int& alphabet_size);




int state_to_int_ID(const std::string& state, const std::set<std::string>& states);

int alphabet_to_int_ID(const char& c, const std::set<char>& alphabet);

std::string int_ID_to_state(const int& ID, const std::set<std::string>& states);

char int_ID_to_alphabet(const int& ID, const std::set<char>& alphabet);


int load_dfa_config_from_file(std::set<std::string>& states,
                                std::string& init_state,
                                std::set<std::string>& acc_states,
                                std::set<char>& alphabet,
                                int**& tf,
                                const std::string&& config_file_name);

