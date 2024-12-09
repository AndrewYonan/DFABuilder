#include <vector>
#include <set>
#include "dfa_machine.h"


class DFA {

    private:

        DFAMachine dfa_machine;
        std::string init_state;
        std::set<std::string> states;
        std::set<std::string> acc_states;    
        std::set<char> alphabet;

        bool verbose_mode = false;

        void print_states();
        void print_init_state();
        void print_acc_states();
        void print_alphabet();
        void print_transition_function();
        void print_raw_transition_function();

        int extract_states(std::ifstream&);
        int extract_init_state(std::ifstream&);
        int extract_acc_states(std::ifstream&);
        int extract_alphabet(std::ifstream&);
        int extract_transition_function(std::ifstream&);

        int state_to_int(std::string);
        int alphabet_to_int(char);
        char int_to_alphabet(int);
        std::string int_to_state(int);
        std::string dfa_cur_state();
        void set_dfa_state(std::string state);

        bool in_accepting_state();
        bool is_valid_state(std::string);
        bool is_valid_alphabet_char(char);

    public:

        bool all_possible_transitions_defined();
        bool is_valid_dfa();
        int configure(std::string file_name);
        int compute(std::string input);
        int compute_from_file(std::string filename);
        void set_verbose_mode(bool);
        void print();
        
};