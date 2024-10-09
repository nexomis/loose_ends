// loose_ends.cpp
#include "loose_ends.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

void LooseEnds::process_line(const std::string &line) {
    std::istringstream ss(line);
    std::string chromosome, position, reference_base, coverage_str, read_bases, base_qualities;

    // Read the fields from the line
    if (!(ss >> chromosome >> position >> reference_base >> coverage_str >> read_bases >> base_qualities)) {
        std::cerr << "Error parsing line: " << line << std::endl;
        return;
    }
    
    int coverage = std::stoi(coverage_str);
    bool is_start = false;
    bool is_end = false;
    int index_bases = 0;
    int index_qualities = 0;
    std::string output_base = "";
    std::string output_bases = "";
    std::string output_qualities = "";

    int read_bases_length = read_bases.length();

    if (coverage == 0){
        std::cout << line << std::endl;
        return;
    }

    while (index_bases < read_bases_length) {
        char c = read_bases[index_bases];

        if (c == '^') {
            is_start = true;
            index_bases += 2; // skip mapping base
            continue;
        } else if (c == '+' || c == '-') {
            output_base += c;
            index_bases++;  // Move past '+' or '-'

            // Parse the indel length
            std::string indel_length_str = "";
            while (index_bases < read_bases_length && isdigit(read_bases[index_bases])) {
                indel_length_str += read_bases[index_bases];
                index_bases++;
            }
            output_base += indel_length_str;

            int indel_length = std::stoi(indel_length_str);

            // Read the indel sequence
            output_base += read_bases.substr(index_bases, indel_length);
            index_bases += indel_length;
        } else {
            output_base += c;
            index_bases++;
        }

        if (read_bases[index_bases] == '$') {
            is_end = true;
            index_bases++;
        }

        if (is_end || is_start) {
            coverage--;
        } else {
            output_bases += output_base;
            output_qualities += base_qualities[index_qualities];
        }

        is_start = false;
        is_end = false;
        output_base = "";
        index_qualities++;

    }
    
    if (coverage == 0) {
        output_bases = "*";
        output_qualities = "*";
    }

    // Output the filtered line
    std::cout << chromosome << '\t' << position << '\t' << reference_base << '\t'
              << coverage << '\t' << output_bases << '\t' << output_qualities << std::endl;
}
