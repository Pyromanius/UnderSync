#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <stdexcept>
#include "main.h"

using namespace std;

void readFile(const string& filePath) {
    
    ifstream file(filePath);
    string line;
    string outputFilePath = filePath + "_newFile.srt";
    ofstream outFile(outputFilePath);


    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filePath << endl;
        return;
    }

    if (!outFile.is_open()) {
        cerr << "Error: Could not create the file " << outputFilePath << endl;
        return;
    }

    while (getline(file, line)) {
        if (!line.empty()) {
            if (is_valid_time(line)) {
                sub_from_time(line);
            }
            outFile << line << endl;
        }

        // Add empty line after each instance.
        else
        outFile << endl;
    }

    file.close();
    outFile.close();
}

bool is_valid_time(string const& line){

    if (line[0] <= '9' && line[2] == ':')
        return true;
    else
        return false;
}

string conv_to_string(int const h, int const m, int const s, int const ms){
    stringstream ts{};
    string text{};
    ts << setw(2) << setfill('0') << h << ':'
    << setw(2) << setfill('0') << m << ':'
    << setw(2) << setfill('0') << s << ','
    << setw(3) << setfill('0') << ms;
    ts >> text;
    
    return text;
}

Time conv_to_time(string const & time_s){
    Time time;
    stringstream time_strings{time_s};
    char space_char;
    
    time_strings >> time.h >> space_char >> time.m >> space_char >> time.s >> space_char >> time.ms;
    
    return time;
    
}

string sub_from_time(string& org_str){

    Time old_start_time, old_end_time, new_start_time, new_end_time;
    string new_start_str, new_end_str;

    old_start_time = conv_to_time(org_str.substr(0, 12));
    old_end_time = conv_to_time(org_str.substr(18, 11));

    new_start_time = old_start_time;
    new_end_time = old_end_time;

    new_end_time.s = new_end_time.s - time_to_add;
    new_start_time.s = new_start_time.s - time_to_add;

    if (new_start_time.s < 00){
        new_start_time.m = new_start_time.m - 1;
        new_start_time.s = new_start_time.s + 60;
    }
    if (new_start_time.m < 00){
        new_start_time.h = new_start_time.h - 1;
        new_start_time.m = new_start_time.m + 60;
    }

    if (new_end_time.s < 00){
        new_end_time.m = new_end_time.m - 1;
        new_end_time.s = new_end_time.s + 60;
    }
    if (new_end_time.m < 00){
        new_end_time.h = new_end_time.h - 1;
        new_end_time.m = new_end_time.m + 60;
    }
    
    new_start_str = conv_to_string(new_start_time.h, new_start_time.m, new_start_time.s, new_start_time.ms);
    new_end_str = conv_to_string(new_end_time.h, new_end_time.m, new_end_time.s, new_end_time.ms);

    org_str.replace(0, 12, new_start_str);
    org_str.replace(17, 12, new_end_str);

    return org_str;
}

string selectFile() {

    char fileName[MAX_PATH] = {0};
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Subtitle Files\0*.srt\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return string(fileName);
    } else {
        cerr << "No file selected or an error occurred." << endl;
        return "";
    }
}

int main() {
    string filePath;
    filePath = selectFile();
    
    if (filePath.empty()) {
        return 1;
    }

    cout << "File was chosen. Enter x seconds to subtract (-x to add): " << endl;
    cin >> time_to_add;

    readFile(filePath);
    
    return 0;
}