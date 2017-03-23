//
// Created by ahmed on 21/03/17.
//

#include "bits/stdc++.h"

using namespace std;

vector<string> lines;
vector<string> regExpressions;

template<bool sortdir = true>
struct Sorter {
    bool operator()(const string &left, const string &right) {
        if (sortdir)
            return left.length() > right.length();
        return left.length() < right.length();
    }
};

map<string, vector<string>, Sorter<>> defs;


class compare{
public:
    bool operator()(pair<string, vector<string>> a , pair<string, vector<string>> b){
        return a.first.length() < b.first.length();
    }
};

priority_queue< pair<string, vector<string>>,vector<pair<string, vector<string>>>,compare > pq;


void trimAndSave(string &line) {

    size_t firstSpaceOccur = line.find_first_not_of(" ");
    line = line.substr(firstSpaceOccur);

//    cout << line << endl;

    size_t lastSpaceOccur = line.find_last_not_of(" ");
    line = line.substr(0, lastSpaceOccur + 1);

//    cout << line << endl;

    lines.push_back(line);
}

//lack loop for all RHS values
vector<string> parseRHS(string RHS, char symbol) {
    vector<string> RHS_parsed;

    stringstream ss;
    string s;
    ss << symbol;
    ss >> s;
    RHS_parsed.push_back(s);

    return RHS_parsed;
}


void identifyDefs() {
    int linesCount = lines.size();

    for (int i = 0; i < linesCount; ++i) {
        string line = lines [i];
        size_t equalIndex = line.find_first_of("=");
        size_t colonIndex = line.find_first_of(":");

        if (equalIndex < colonIndex) {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            equalIndex = line.find_first_of("=");
            string LHS = line.substr(0, equalIndex);
            string RHS = line.substr(equalIndex + 1, line.length() - 1);

            pq.push(make_pair(LHS, parseRHS(RHS, 'a' + i)));
        }
    }

}


void replaceDefs() {
    
    while(pq.size() > 0){
        pair<string, vector<string> > record;

        record = pq.top();
        pq.pop();

        string originalDef = record.first;
        string singleChar = record.second[0];

        for (int i = 0; i < lines.size(); ++i) {
            string line = lines[i];

            if (line.find(originalDef) != string::npos) {

                size_t index = 0;
                while (index < line.length()) {
                    /* Locate the substring to replace. */
                    index = line.find(originalDef, index);
                    size_t firstEqual = line.find_first_of("=");
                    if (index == string::npos)
                        break;
                    
                    /* Make the replacement. */
                    line.replace(index, originalDef.length(), singleChar);

                    /* Advance index forward so the next iteration doesn't pick it up as well. */
                    index += 1;
                }
            }
            lines[i] = line;
        }
    }

    for (int i = 0; i < lines.size(); ++i) {
        cout << lines[i] << endl;
    }
}


void addConcatenation(){

    for(int i = 0 ; i < lines.size(); ++i){





    }

}



void identifyExp(vector<string> lines){

    for(int i = 0; i < lines.size() ; ++i) {
        size_t found = lines[i].find(":");
        if (found != std::string::npos)
            regExpressions.push_back(lines[i]);
    }

}


void parseFile() {
    string line;
    ifstream myfile("/home/ahmed/Compiler_Phase1/rules.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            trimAndSave(line);
        }
        myfile.close();
    } else
        cout << "Unable to open file";
}


int main() {
    string file[] = {"letter = a-z | A-Z",
                     "digit = 0-9",
                     "id: letter (letter | digit)*",
                     "digits = digit+",
                     "{boolean int float}",
                     "num: digit+ | digit+ . digits ( \L | E digits)",
                     "relop: \=\= | !\= | > | >\= | < | <\=",
                     "assign: =",
                     "{ if else while }",
                     "[; , \( \) { }]",
                     "addop: \+ | -",
                     "mulop: \* | /"
    };

    for (int i = 0; i < 12; ++i) {
        trimAndSave(file[i]);
    }


    identifyDefs();
    replaceDefs();
    return 0;
}


//int main() {
//
//    string line = "    letter = a-z | A-Z    ";
//    trimAndSave(line);
//    line =     "   digit = 0-9   ";
//    trimAndSave(line);
//    line = "   id: letter (letter | digit)* ";
//    trimAndSave(line);
//    line = "     digits = digit+   ";
//    trimAndSave(line);
//
//
//    identifyDefs();
//    replaceDefs();
//


//    char line [80];
//    FILE * pFile;
//
//    pFile = fopen ("/home/ahmed/Compiler_Phase1/rules.txt","r");
//
//
//    while ( fgets ( line, sizeof line, pFile ) != NULL ) /* read a line */
//    {
//        fputs ( line, stdout ); /* write the line */
//        string str2(line);
//        trimAndSave(str2);
//    }
//    fclose ( pFile );
//
//
//
//
//    return 0;
//}