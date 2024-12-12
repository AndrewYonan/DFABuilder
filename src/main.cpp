#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <sstream>
#include <filesystem>
#include "dfa.h"
#include "dfa_config_file_parser.h"


template<typename T>
void print_set(const std::set<T>& s) {
    std::cout << "{";
    for (auto it = s.begin(); it != s.end(); it++) {
        if (it == std::prev(s.end())) {
            std::cout << *it;
        }
        else {
            std::cout << *it << ", ";
        }
    }
    std::cout << "}\n";
}


void print_tf_matrix(int** tf, std::set<std::string>& states, std::set<char>& alphabet) {
    std::cout << "transition function : " << std::endl;
    for (int i = 0; i < states.size(); ++i) {
        for (int j = 0; j < alphabet.size(); ++j) {
            std::cout << "(" << int_ID_to_state(i, states) << ", " 
                            << int_ID_to_alphabet(j, alphabet)
                            << ") ==> "
                            << int_ID_to_state(tf[i][j], states) << std::endl;
        }
    }
}


void print_dfa_config_data(std::set<std::string>& states,
                                std::string& init_state,
                                std::set<std::string>& acc_states,
                                std::set<char>& alphabet,
                                int**& tf) {
    std::cout << "states : ";
    print_set(states);
    std::cout << "Init state : " << init_state << std::endl;
    std::cout << "accept states : ";
    print_set(acc_states);
    std::cout << "alphabet : ";
    print_set(alphabet);
    print_tf_matrix(tf, states, alphabet);
}


int** alloc_tf_matrix(const int& n_states, const int& alphabet_size) {
    int** tf = new int*[n_states];
    for (int i = 0; i < n_states; ++i) {
        tf[i] = new int[alphabet_size];
        for (int j = 0; j < alphabet_size; ++j) {
            tf[i][j] = -1;
        }
    }
    return tf;
}


int load_dfa_config_from_file(std::set<std::string>& states,
                                std::string& init_state,
                                std::set<std::string>& acc_states,
                                std::set<char>& alphabet,
                                int**& tf,
                                const std::string&& config_file_name) {
    
    std::ifstream config_file(config_file_name);

    if (!config_file.is_open()) {
        std::cout << "Error : dfa config file \'" << config_file_name << "\' could not be opened" << std::endl;
        return -1;
    }

    if (extract_states(states, config_file) < 0) {
        config_file.close(); 
        return -1;
    }

    if (extract_init_state(init_state, states, config_file) < 0) {
        config_file.close(); 
        return -1;
    }

    if (extract_acc_states(acc_states, states, config_file) < 0) {
        config_file.close(); 
        return -1;
    }

    if (extract_alphabet(alphabet, config_file) < 0) {
        config_file.close(); 
        return -1;
    }

    tf = alloc_tf_matrix(states.size(), alphabet.size());

    if (extract_transition_function(tf, states, alphabet, config_file) < 0) {
        config_file.close(); 
        return -1;
    }

    if (!all_possible_transitions_defined(tf, states.size(), alphabet.size())) {
        std::cout << "Error : the provided file data does not define a valid DFA (missing transitions)" << std::endl;
        config_file.close();
        return - 1;
    
    }

    std::cout << "DFA parsed successfully..." << std::endl;
    std::cout << "DFA parameters : \n======================\n";
    print_dfa_config_data(states, init_state, acc_states, alphabet, tf);
    std::cout << "======================\n";
    config_file.close();

    return 1;

}




template <typename T>
std::string to_cpp_string_set(const std::set<T>& s, std::string string_elem_delim) {
    std::ostringstream oss;
    oss << "{";
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it != s.begin()) {
            oss << ", ";
        }
        oss << string_elem_delim << *it << string_elem_delim;
    }
    oss << "}";
    return oss.str();
}



std::string to_cpp_matrix_init_statement(int** tf, const int& n_states, const int& alphabet_size) {
    std::ostringstream oss;
    oss << "{";
    for (int i = 0; i < n_states; ++i) {
        oss << "{";
        for (int j = 0; j < alphabet_size; ++j) {
            if (j > 0) {
                oss << ", ";
            }
            oss << std::to_string(tf[i][j]);
        }
        oss << "}";
        if (i < n_states - 1) {
            oss << ", ";
        }
    }
    oss << "}";
    return oss.str();
}



void gen_exec_dfa_obj(std::set<std::string>& states,
                        std::string& init_state,
                        std::set<std::string>& acc_states,
                        std::set<char>& alphabet,
                        int** tf) {

        
    std::string cpp_file_name = "dfa_core.cpp";
    std::ofstream out_file(cpp_file_name);

    out_file << 
    
    R"cpp(

    #include <iostream>
    #include <set>
    #include <string>
    #include "dfa.h"

    int** static_transition_function_to_heap_ptr(int arr[][)cpp" << std::to_string(alphabet.size()) << R"cpp(], int num_rows);

    int verbose_mode;

    const int num_states = )cpp" << std::to_string(states.size()) << ";\n\t"

    << R"cpp(const int alphabet_size = )cpp" << std::to_string(alphabet.size()) << ";\n\t"

    << R"cpp(std::set<std::string> states = )cpp" << to_cpp_string_set(states, "\"") << ";\n\t"

    << R"cpp(std::string init_state = )cpp" << "\"" << init_state << "\";\n\t" 

    << R"cpp(std::set<std::string> acc_states = )cpp" << to_cpp_string_set(acc_states, "\"") << ";\n\t"

    << R"cpp(std::set<char> alphabet = )cpp" << to_cpp_string_set(alphabet, "\'") << ";\n\t"

    << R"cpp(int tf_arr[num_states][alphabet_size] = )cpp" << to_cpp_matrix_init_statement(tf, states.size(), alphabet.size()) << ";\n\t"
    
    << R"cpp(int** tf = static_transition_function_to_heap_ptr(tf_arr, num_states);

    int main(int argc, char** argv) {

        if (argc < 2) {
            std::cout << "usage : <dfa_prog> <strings_input_file> <verbose_mode = 0,1, or 2>" << std::endl;
            return 1;
        }

        if (argc == 3) {
            verbose_mode = std::stoi(argv[2]);
            if (verbose_mode > 2) verbose_mode = 2;
            if (verbose_mode < 0) verbose_mode = 0;
        }
        else {
            verbose_mode = 1;
        }

        DFA dfa;
        dfa.configure(states, init_state, acc_states, alphabet, tf);
        dfa.set_verbose_mode(verbose_mode);
        dfa.compute_from_file(argv[1]);
    }


    int** static_transition_function_to_heap_ptr(int arr[][)cpp" << std::to_string(alphabet.size()) << R"cpp(], int num_rows) {
        int** ptr = new int*[num_rows];
        for (int i = 0; i < num_rows; ++i) {
            ptr[i] = new int[)cpp" << std::to_string(alphabet.size()) << R"cpp(];
            for (int j = 0; j < )cpp" << std::to_string(alphabet.size()) << R"cpp(; ++j) {
                ptr[i][j] = arr[i][j];
            }
        }
        return ptr;
    }
    
    )cpp";

    out_file.close();

    std::string gcc = "g++ -std=c++11 " + cpp_file_name + " dfa.cpp dfa_machine.cpp -o dfa";
    system(gcc.c_str());

    assert(cpp_file_name == "dfa_core.cpp");
    std::remove(cpp_file_name.c_str());

    std::cout << "DFA executable configured successfully..." << std::endl;
    std::cout << "Now run the DFA executable \'dfa\' ..." << std::endl;
}




int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Error : usage : <prog> <dfa_config_file>" << std::endl;
        return 1;
    }

    std::set<std::string> states;
    std::string init_state;
    std::set<std::string> acc_states;
    std::set<char> alphabet;
    int** tf;
    

    if (load_dfa_config_from_file(states, init_state, acc_states, alphabet, tf, argv[1]) < 0) {
        std::cout << "DFA config file parse error..." << std::endl;
        return 1;
    }

    gen_exec_dfa_obj(states, init_state, acc_states, alphabet, tf);

    return 0;
}