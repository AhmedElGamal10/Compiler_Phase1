//
// Created by ahmed on 21/03/17.
//

#include "FileParser.h"

std::vector<std::string> lines;

template<bool sortdir = true>
struct Sorter {
    bool operator()(const std::string &left, const std::string &right) {
        if (sortdir)
            return left < right;
        return left > right;
    }
};

std::map<std::string, std::vector, Sorter<>> defs;

FileParser() {

}

void trimAndSave(std::string line) {

    std::size_t firstSpaceOccur = line.find_first_not_of(" ");
    line = line.substr(firstSpaceOccur);

    std::size_t lastSpaceOccur = line.find_last_not_of(" ");
    line = line.substr(0, lastSpaceOccur + 1);

    lines.push_back(line);
}

//lack loop for all RHS values
std::vector<std::string> parseRHS(std::string RHS, char symbol) {
    std::vector<std::string> RHS_parsed;

    std::stringstream ss;
    std::string s;
    ss << symbol;
    ss >> s;
    RHS_parsed.push_back(s);

    return RHS_parsed;
}


void identifyDefs() {
    int linesCount = lines.size();

    for (int i = 0; i < linesCount; ++i) {
        std::string line = lines[i];
        std::size_t equalIndex = line.find_first_of("=");
        std::size_t colonIndex = line.find_first_of(":");

        if (equalIndex < colonIndex) {
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            equalIndex = line.find_first_of("=");
            std::string LHS = line.substr(0, equalIndex);
            std::string RHS = line.substr(equalIndex + 1, line.length() - 1);

            defs[LHS] = parseRHS(RHS, 'a' + i);
        }
    }

}


void replaceDefs() {

    for (std::map<std::string, std::vector>::iterator it = defs.begin(); it != defs.end(); ++it) {
        std::string originalDef = it->first;
        std::string singleChar = it->second[0];

        for (int i = 0; i < lines.size(); ++i) {
            std::string line = lines[i];

            if (line.find(originalDef) != std::string::npos) {
                std::cout << "found!" << '\n';

                size_t index = 0;
                while (index < line.length()) {
                    /* Locate the substring to replace. */
                    index = line.find(originalDef, index);
                    if (index == std::string::npos)
                        break;

                    /* Make the replacement. */
                    line.replace(index, 1, singleChar);

                    /* Advance index forward so the next iteration doesn't pick it up as well. */
                    index += 1;
                }
            }

            lines[i] = line;
        }
    }
}


void FileParser::parseFile() {
    std::string line;
    std::ifstream myfile("rules.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            trimAndSave(line);
        }
        myfile.close();
    } else
        std::cout << "Unable to open file";
}

