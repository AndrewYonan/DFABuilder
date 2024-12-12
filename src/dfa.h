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

        void print_states();
        void print_init_state();
        void print_acc_states();
        void print_alphabet();
        void print_transition_function();
        void print_raw_transition_function();

        int state_to_int(std::string);
        int alphabet_to_int(char);
        char int_to_alphabet(int);
        std::string int_to_state(int);
        std::string dfa_cur_state();
        void set_dfa_state(std::string state);

        bool in_accepting_state();
        bool is_valid_state(std::string);
        bool is_valid_alphabet_char(char);
        int verbose_mode = false;

    public:

        void set_init_state(const std::string& state);
        void set_states(const std::set<std::string>&);
        void set_accept_states(const std::set<std::string>&);
        void set_alphabet(const std::set<char>&);
        void set_transition_function(int**&, int);

        bool compute(std::string input);
        int compute_from_file(std::string filename);
        void set_verbose_mode(int);
        void print();

        void configure(std::set<std::string>& states,
                    std::string& init_state,
                    std::set<std::string>& acc_states,
                    std::set<char>& alphabet,
                    int**& tf);
        
};