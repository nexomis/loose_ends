// loose_ends.cpp
#include "loose_ends.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

void LooseEnds::process_line(const std::string &line) {
    std::istringstream ss(line);
    std::string chromosome, position_str, reference_base, coverage_str, read_bases, base_qualities;

    // Read the fields from the line
    if (!(ss >> chromosome >> position_str >> reference_base >> coverage_str >> read_bases >> base_qualities)) {
        std::cerr << "Error parsing line: " << line << std::endl;
        return;
    }
    
    int position = std::stoi(position_str);
    int coverage = std::stoi(coverage_str);
    
    int index_bases = 0;
    int index_qualities = 0;
    std::string output_bases = "";
    std::string output_qualities = "";

    int read_bases_length = read_bases.length();
    int base_qualities_length = base_qualities.length();

    if (base_qualities_length != coverage) {
        if (!(read_bases == "*" && base_qualities == "*" && coverage == 0)) {
            std::cerr << "coverage and qualities sizes issue" << std::endl;
        }
    }

    if (coverage == 0){
        std::cout << line << std::endl;
        return;
    }

    int adjusted_coverage = coverage;

    while (index_bases < read_bases_length) {
        bool is_start = false;
        bool is_end = false;
        int deletion_length = 0;
        bool discard_gap = false;
        std::string output_base = "";
        char start_c = read_bases[index_bases];

        if (start_c == '^') {
            is_start = true;
            index_bases += 2; // skip mapping base
        }

        char base_c = read_bases[index_bases];

        if (base_c == '*') {
            auto &deletion_queue = gap_buffer[position];
            if (!deletion_queue.empty()) {
                discard_gap = deletion_queue.front();
                deletion_queue.pop();
            }
            if (! discard_gap) {
                output_base += base_c;
            }
        } else {
            // add match or mismatch
            output_base += base_c;
        }

        index_bases++;
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

            if (next_c == '-') {
                deletion_length = indel_length;
            }

            // Read the indel sequence
            output_base += read_bases.substr(index_bases, indel_length);
            index_bases += indel_length;
        }

        if (read_bases[index_bases] == '$') {
            is_end = true;
            index_bases++;
        }

        if (deletion_length > 0) {
            for (int i = 1; i <= deletion_length; ++i) {
                int del_pos = position + i;
                gap_buffer[del_pos].push(is_end || is_start);
            }
        }

        if (is_end || is_start || discard_gap) {
            adjusted_coverage--;
        } else {
            output_bases += output_base;
            output_qualities += base_qualities[index_qualities];
        }
        index_qualities++;

        // TODO destroy queue for this position (check if empty)
    }

    if (gap_buffer.count(position) == 1) {
        if (!gap_buffer[position].empty()){
            std::cerr << "gap buffer not empty" << std::endl;
        }
        gap_buffer.erase(position);
    }
    
    if (adjusted_coverage == 0) {
        output_bases = "*";
        output_qualities = "*";
    }

    if (base_qualities_length != index_qualities) {
        std::cerr << "quality end not reached" << std::endl;
    }

    int output_qualities_length = output_qualities.length();

    if ( output_qualities_length != adjusted_coverage) {
        if (!(adjusted_coverage == 0 && output_bases == "*" && output_qualities == "*")) {
            std::cerr << "output coverage issue" << std::endl;
        }
    }

    // Output the filtered line
    std::cout << chromosome << '\t' << position << '\t' << reference_base << '\t'
              << adjusted_coverage << '\t' << output_bases << '\t' << output_qualities << std::endl;

}
