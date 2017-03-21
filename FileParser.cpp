//
// Created by ahmed on 21/03/17.
//

#include "FileParser.h"
class FileParser {

    const int KEYWORD = 1;
    const int PUNC = 2;
    const int DEF = 3;
    const int EXP = 4;
    const int ERR = 5;

    int classifyLine(std::string line) {
        int length = line.length();

        if (line.at(0) == '{')
            return KEYWORD;
        else if (line.at(1) == '[')
            return PUNC;


        for (int i = 0; i < length; i++)
            if (line.at(i) == ':')
                return DEF;
            else if (line.at(i) == '=')
                return EXP;

        return ERR;
    }

    bool trim (std::string &line, int &length){
        if( (line.at(0) == '{' && line.at(length-1) == '}') || (line.at(0) == '[' && line.at(length-1) == ']') ) {
            line = line.substr(1, length-2);
            return true;
        }

        return false;
    }

    bool parse(std::string line, int &length, int type) {
        if ((type == KEYWORD) || type == PUNC) {
            if(!trim(line, length))
                return false;

            splitAndFillTable(line);

        }else if(type == DEF){

        }else if(type == EXP){

        }

        return false;
    }

    void FileParser::parse(std::string line) {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        int type = classifyLine(line);

        bool parse(line, type);


    }
};