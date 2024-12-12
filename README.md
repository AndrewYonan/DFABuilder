General finite state machine simulator (discrete finite automaton - DFA) in C++

Example Usage Instructions:

Compilation:
Must run:
$ make


this produces an executable called "build_dfa", 
which should be run as follows:

$ ./build_dfa <dfa_config_file>

where "dfa_config_file" is a text file containing specific dfa parameters (states, alphabet, transition function, ...)



For instance, as an example run the following:

$ ./build_dfa dfa_config_files/dfa.txt

This command produces an executable dfa object called "dfa"
which can be run as follows:

$ ./dfa <string_input_file> <verbosity>

where "string_input_file" is a .txt list of strings to input into the dfa and obtain outputs.

The "verbosity" argument can be specified as either 0, 1, or 2, depending on how much detail should 
be shown in the dfa's computations.

As an example, run

$ ./dfa dfa_input_files/strings1.txt 2