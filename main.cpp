//
// Created by ahmed on 21/03/17.
//

#include "bits/stdc++.h"
#include "Graph.h"
#include "State.h"

using namespace std;

struct operatorSym {
    char symbol;
    int priority;
}orSym, concatSym, closureSym, parenOpenSym, parenCloseSym;

class compare {
public:
    bool operator()(pair<string, vector<string>> a, pair<string, vector<string>> b) {
        return a.first.length() < b.first.length();
    }
};


const string EPS = "#";

vector<string> lines;
vector<string> regExpressions;
vector<string> postfixExpressions;

vector<char> symbols;

priority_queue<pair<string, vector<string>>, vector<pair<string, vector<string>>>, compare> pq;

vector<State> states;
map <char, string> transitionType;

void setupOperators() {
    orSym.symbol = '|';
    orSym.priority = 1;

    concatSym.symbol = '~';
    concatSym.priority = 2;

    closureSym.symbol = '*';
    closureSym.priority = 3;

    parenOpenSym.symbol = '(';
    parenOpenSym.priority = 0;

    parenCloseSym.symbol = ')';
    parenCloseSym.priority = 0;
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

bool isOperator(char c) {
    return (c == orSym.symbol || c == concatSym.symbol || c == closureSym.symbol || c == parenOpenSym.symbol
            || c == parenCloseSym.symbol);
}

bool isSymbol(char c) {
    return (std::find(symbols.begin(), symbols.end(), c) != symbols.end());
}

operatorSym selectOperator(char c) {
    if (c == orSym.symbol) return orSym;
    else if (c == concatSym.symbol) return concatSym;
    else if (c == closureSym.symbol) return closureSym;
    else if (c == parenOpenSym.symbol) return parenOpenSym;
    else if (c == parenCloseSym.symbol) return parenCloseSym;
}

string makeString(queue<char> queue) {
    stringstream ss;
    string s;
    while (queue.size() > 0) {
        ss << queue.front();
        queue.pop();
    }
    ss >> s;
    return s;
}

bool tokenReplace(int type, int pointer, string &line, string originalDef, string newToken) {

    if (type == 1) {
        int index = line.find(originalDef);
        line.replace(pointer, originalDef.length(), newToken);

        return (index != string::npos);
    } else {
        size_t index = 0;
        if (line.find(originalDef) != string::npos) {

            while (index < line.length()) {
                /* Locate the substring to replace. */
                index = line.find(originalDef, index);

                if (index == string::npos)
                    break;

                /* Make the replacement. */
                line.replace(index, originalDef.length(), newToken);

                /* Advance index forward so the next iteration doesn't pick it up as well. */
                index += 1;
            }
        }
        return (index > 0);
    }
}



//*******************************************************************************************
void trimAndSave(string &line) {

    size_t firstSpaceOccur = line.find_first_not_of(" ");
    line = line.substr(firstSpaceOccur);

    size_t lastSpaceOccur = line.find_last_not_of(" ");
    line = line.substr(0, lastSpaceOccur + 1);

    lines.push_back(line);
}

void identifyDefs() {
    int linesCount = lines.size();

    for (int i = 0; i < linesCount; ++i) {
        string line = lines[i];
        size_t equalIndex = line.find_first_of("=");
        size_t colonIndex = line.find_first_of(":");

        if (equalIndex < colonIndex) {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            equalIndex = line.find_first_of("=");
            string LHS = line.substr(0, equalIndex);
            string RHS = line.substr(equalIndex + 1, line.length() - 1);

            pq.push(make_pair(LHS, parseRHS(RHS, 'a' + i)));
            symbols.push_back('a' + i);
            transitionType['a' + i] = LHS;
        }
    }

}

void identifyExp(vector<string> lines) {

    for (int i = 0; i < lines.size(); ++i) {
        size_t found = lines[i].find(":");
        if (found != std::string::npos) {
            regExpressions.push_back(lines[i].substr(found+1,std::string::npos));
//            cout << regExpressions[regExpressions.size()-1] << endl;
        }
    }

}

void replaceDefs() {

    while (pq.size() > 0) {
        pair<string, vector<string> > record;

        record = pq.top();
        pq.pop();

        string originalDef = record.first;
        string singleChar = record.second[0];

        for (int i = 0; i < lines.size(); ++i) {
            string line = lines[i];

            if (tokenReplace(2, -1, line, originalDef, singleChar))
                lines[i] = line;
        }
    }
}



void addConcatenation() {

    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].find("\L") != string::npos) {
            tokenReplace(2, -1, lines[i], "\L", "#");
        }

        if (lines[i].find("E") != string::npos) {
            tokenReplace(2, -1, lines[i], "E", "$");
        }

        lines[i].erase(remove(lines[i].begin(), lines[i].end(), ' '), lines[i].end());

        int startIndex = lines[i].find(":");

        if (startIndex != string::npos) {
            for (int j = startIndex; j < lines[i].length() - 1; ++j) {

                if (isSymbol(lines[i].at(j)) && (isSymbol(lines[i].at(j + 1)) || lines[i].at(j + 1) == '(')) {
                    stringstream ss1;
                    string s1;
                    ss1 << lines[i].at(j);
                    ss1 >> s1;

                    stringstream ss2;
                    string s2;
                    ss2 << lines[i].at(j);
                    ss2 << "~";
                    ss2 >> s2;

                    tokenReplace(1, j, lines[i], s1, s2);
                    cout << lines[i] << endl;
                }
            }
        }
    }
}

void extractKeywords() {
}


void extractPunctuation() {

}


void constructPostfix() {

    for (int i = 0; i < regExpressions.size(); ++i) {
        stack<operatorSym> operators;
        queue<char> operands;

        for (int j = 0; j < regExpressions[i].length(); ++j) {

            char c = regExpressions[i][j];

            if (isOperator(c)) {
                operatorSym tempOperator = selectOperator(c);
                if (!operators.empty() && tempOperator.priority == operators.top().priority)
                    operands.push(c);
                else
                    operators.push(tempOperator);
            } else {
                operands.push(c);
            }

            if (c == ')'){
                operators.pop();
                while (operators.top().symbol != parenOpenSym.symbol) {
                    char temp = operators.top().symbol;
                    if(temp != parenOpenSym.symbol)
                        operands.push(temp);
                    operators.pop();
                }
                operators.pop();
            }
        }
        while (operators.size() > 0) {
            operands.push(operators.top().symbol);
            operators.pop();
        }

        postfixExpressions.push_back(makeString(operands));
        cout << postfixExpressions[i] << endl;
    }

}

vector<State> graph;
stack<Graph> graphsStack;


Graph applyConcatenation(Graph graph1, Graph graph2) {
    graph1.accepting.type = 1;
    graph2.start.type = 1;

    graph1.accepting.next[graph2.start.onEntringEdge].push_back(graph2.start);

    //handle kleene closure
    if(graph2.closureApplied == true){
        graph1.accepting.next[EPS].push_back(graph2.accepting);
    }

}

Graph applyOr(Graph graph1, Graph graph2){




}

void applyKleeneClosure(Graph &tempGraph){

    tempGraph.closureApplied = true;

    tempGraph.accepting.next[EPS].push_back(tempGraph.start);
}

void applyPositiveClosure(Graph &tempGraph){
    tempGraph.accepting.next[EPS].push_back(tempGraph.start);
}


void makeOperation(stack<Graph> &graphsStack, char &i) {

    if(i == '*'){
        Graph tempGraph = graphsStack.top();
        graphsStack.pop();
        applyKleeneClosure(tempGraph);
        graphsStack.push(tempGraph);
    }else if(i == '|'){
        applyOr();
    }else if(i == '~'){
        //aab and I wanna concate a and b --> graph1: a, graph2: b

        //graph2: b
        Graph tempGraph2 = graphsStack.top();
        graphsStack.pop();

        //graph1: a
        Graph tempGraph1 = graphsStack.top();
        graphsStack.pop();

        Graph resultGraph = applyConcatenation(tempGraph1, tempGraph2);
        graphsStack.push(resultGraph);
    }else if(i == '+'){
        Graph tempGraph = graphsStack.top();
        graphsStack.pop();
        applyPositiveClosure(tempGraph);
    }



}

void evaluatePostfix(){

    for (int i = 0; i < postfixExpressions.size(); ++i) {
        for(int j = 0; j < postfixExpressions[i].length(); ++j){
            if(isSymbol(postfixExpressions[i][j])){
                State startState;
                State intermediateState;
                State acceptingState;

                startState.type = 0;
                intermediateState.type = 1;
                acceptingState.type = 2;

                startState.onEntringEdge = transitionType[regExpressions[i][j]];
                intermediateState.onEntringEdge = EPS;
                acceptingState.onEntringEdge = EPS;

                Graph singleStateGraph;
                singleStateGraph.start = startState;
                singleStateGraph.intermediate.push_back(intermediateState);
                singleStateGraph.accepting = acceptingState;

                graphsStack.push(singleStateGraph);
            }
            else if(isOperator(postfixExpressions[i][j])){
                makeOperation(graphsStack, postfixExpressions[i][j]);
            }
        }


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
                     "id: letter letter (letter | digit)*",
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

    setupOperators();

    identifyDefs();
    replaceDefs();
    addConcatenation();
    identifyExp(lines);
    constructPostfix();
    return 0;
}