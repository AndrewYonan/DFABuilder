#include <iostream>

#ifndef DFA_MACHINE_H
#define DFA_MACHINE_H

class DFAMachine {
    private:
        int num_states;
        int _state;
        int** _tf;
        
    public:
        DFAMachine();
        ~DFAMachine();
        int** get_tf();
        int get_state();
        void set_num_states(int);
        void set_state(int);
        int** alloc_transition_func_mem(int, int);
        void step(const int&);
        void set_tf(int**&, int);

};

#endif