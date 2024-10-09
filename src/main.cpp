// main.cpp
#include "loose_ends.hpp"
#include <iostream>
#include <string>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        LooseEnds::process_line(line);
    }
    return 0;
}