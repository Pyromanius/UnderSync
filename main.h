#ifndef MAIN_H
#define MAIN_H

#include <vector>

    struct Time{ 
        int h, m, s, ms;
    };


    int time_to_add;

    void readFile(const std::string& filePath);
    
    bool is_valid_time(std::string const& line);
    
    std::string conv_to_string(int const h, int const m, int const s, int const ms);
    
    Time conv_to_time(std::string const & t);
    
    std::string sub_from_time(std::string& str);
    
    std::string selectFile();
    
#endif