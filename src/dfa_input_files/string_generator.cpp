#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>


std::string rand_string(const std::vector<char>& alphabet, int len) {
    std::string res;
    for (int i = 0; i < len; ++i) {
        res += alphabet[rand() % alphabet.size()];
    }
    return res;
}



int main() {

    long seed = std::time(nullptr);
    srand(seed * seed);
    
    std::vector<char> alphabet = {'a', 'b', 'c'};
    const int ITERS = 1000000;
    const int str_length = 20;
    
    std::ofstream out_file("strings4.txt");

    for (int i = 0; i < ITERS; ++i) {
        out_file << rand_string(alphabet, str_length) << std::endl;
    }

    out_file.close();
    return 0;
}