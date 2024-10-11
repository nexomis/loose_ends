// loose_ends.hpp
#ifndef LOOSE_ENDS_H
#define LOOSE_ENDS_H

#include <string>
#include <queue>
#include <map>

void CheckValidRef(const std::string& str);

class LooseEnds {

   private:
       std::map<int, std::queue<bool>> gap_buffer;

   public:
       void process_line(const std::string &line);
};

#endif // LOOSE_ENDS_H