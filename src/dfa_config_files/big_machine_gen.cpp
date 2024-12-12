#include <iostream>


int main() {

    std::vector<char> alphabet = {'a', 'b', 'c'};
    const int ITERS = 100000;
    const int str_length = 20;
    
    std::ofstream out_file("strings4.txt");

    for (int i = 0; i < ITERS; ++i) {
        out_file << rand_string(alphabet, str_length) << std::endl;
    }

    out_file.close();
    return 0;
    return 0;
}