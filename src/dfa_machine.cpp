#include "dfa_machine.h"

DFAMachine::DFAMachine() {
    _state = 0;
    _tf = nullptr;
}

DFAMachine::~DFAMachine() {
    if (_tf == nullptr) return;
    for (int i = 0; i < num_states; ++i) delete [] _tf[i];
    delete [] _tf;
}

void DFAMachine::step(const int& input) {
    _state = _tf[_state][input];
}

void DFAMachine::set_num_states(int n) {
    num_states = n;
}

int** DFAMachine::get_tf() {
    return _tf;
}

int DFAMachine::get_state() {
    return _state;
}

void DFAMachine::set_state(int state) {
    _state = state;
}

void DFAMachine::alloc_transition_func_mem(int n_states, int alphabet_size) {
    num_states = n_states;
    _tf = new int*[num_states];
    for (int i = 0; i < num_states; ++i) {
        _tf[i] = new int[alphabet_size];
        for (int j = 0; j < alphabet_size; ++j) {
            _tf[i][j] = -1;
        }
    }
}

