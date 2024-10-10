// loose_ends.cpp
#include "loose_ends.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

// let us consider a case where we have a deletion:
// test	1	A	10	-1c-1c-1c-1c-1c$.-1c$-1c.$-1c$.	=========
// test	2	C	8	********	======

// We need to store in memory when we remove a detected deletion in on to eventually skip the corresponding * in the next positions.
// test	1	A	10	-1c-2c-2c-1c-1c$.-1c$-1c.$-1c$.	==========
// test	2	C	10	**********	==========
// test	2	C	2	**	==

// Could you please fix it 
// we shall keep a buffer of next positions where we store the expected gap ''.
// we need to create a buffer in which we can add r delete gap in a specific
// position and initiate the position if it does not exists.

// At a position each time we see a gap with the sign '*' we need to check if a gaps has been registered (first in first out).
// if yes we need to check if it need deletion and adapt coverage accordingly.
// we need to remove using the first in first out.

// could you adapt the code below ?

void LooseEnds::process_line(const std::string &line) {
    std::istringstream ss(line);
    std::string chromosome, position, reference_base, coverage_str, read_bases, base_qualities;

    // Read the fields from the line
    if (!(ss >> chromosome >> position >> reference_base >> coverage_str >> read_bases >> base_qualities)) {
        std::cerr << "Error parsing line: " << line << std::endl;
        return;
    }
    
    int coverage = std::stoi(coverage_str);
    int index_bases = 0;
    int index_qualities = 0;
    std::string output_bases = "";
    std::string output_qualities = "";

    int read_bases_length = read_bases.length();

    if (coverage == 0){
        std::cout << line << std::endl;
        return;
    }

    int current_position = std::stoi(position);

    while (index_bases < read_bases_length) {
        bool is_start = false;
        bool is_end = false;
        bool is_indel = false;
        std::string output_base = "";
        char c = read_bases[index_bases];

        if (c == '^') {
            is_start = true;
            index_bases += 2; // skip mapping base
            continue;
        } 
        
        output_base += c;
        index_bases++;  // Move past '+' or '-'
        char next_c = read_bases[index_bases];
        
        if (next_c == '+' || next_c == '-') {
            output_base += next_c;
            index_bases++;  // Move past '+' or '-'

            // Parse the indel length
            std::string indel_length_str = "";
            while (index_bases < read_bases_length && isdigit(read_bases[index_bases])) {
                indel_length_str += read_bases[index_bases];
                index_bases++;
            }

            output_base += indel_length_str;

            int indel_length = std::stoi(indel_length_str);

            // if next_c == '-'
            // feed the futur gap buffer based on current position, deletion length
            // and current position

            // Read the indel sequence
            output_base += read_bases.substr(index_bases, indel_length);
            index_bases += indel_length;
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
