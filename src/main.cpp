// main.cpp
#include "loose_ends.hpp"
#include <iostream>
#include <string>

int main() {
    std::string line;
    LooseEnds le;
    while (std::getline(std::cin, line)) {
        le.process_line(line);
    }
    return 0;
}