//
// Created by ahmed on 21/03/17.
//

#include "bits/stdc++.h"
#ifndef CPP_FILEPARSER_H
#define CPP_FILEPARSER_H

class FileParser{


public:
    std::map<std::string, std::string> symbolTable;
    FileParser();


private:
    void parse(std::string);
    int classifyLine(std::string);
    const int KEYWORD;
    const int PUNC;
    const int DEF;
    const int EXP;
    const int ERR;
};

#endif //CPP_FILEPARSER_H
