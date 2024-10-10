// loose_ends.hpp
#ifndef LOOSE_ENDS_H
#define LOOSE_ENDS_H

#include <string>
#include <queue>
#include <map>

class LooseEnds {

private:
    std::map<int, std::queue<int>> gap_buffer;

public:
    static void process_line(const std::string &line);
};

#endif // LOOSE_ENDS_H