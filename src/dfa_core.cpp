

    #include <iostream>
    #include <set>
    #include <string>
    #include "dfa.h"

    int** static_transition_function_to_heap_ptr(int arr[][3], int num_rows);

    int verbose_mode;

    const int num_states = 5;
	const int alphabet_size = 3;
	std::set<std::string> states = {"q_0", "q_1", "q_2", "q_3", "q_4"};
	std::string init_state = "q_2";
	std::set<std::string> acc_states = {"q_0", "q_3"};
	std::set<char> alphabet = {'a', 'b', 'c'};
	int tf_arr[num_states][alphabet_size] = {{4, 1, 2}, {3, 4, 3}, {1, 2, 4}, {4, 4, 1}, {2, 1, 4}};
	int** tf = static_transition_function_to_heap_ptr(tf_arr, num_states);

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


    int** static_transition_function_to_heap_ptr(int arr[][3], int num_rows) {
        int** ptr = new int*[num_rows];
        for (int i = 0; i < num_rows; ++i) {
            ptr[i] = new int[3];
            for (int j = 0; j < 3; ++j) {
                ptr[i][j] = arr[i][j];
            }
        }
        return ptr;
    }
    
    