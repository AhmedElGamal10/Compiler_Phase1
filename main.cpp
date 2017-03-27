//
// Created by ahmed on 21/03/17.
//

#include "bits/stdc++.h"
#include "Graph.h"

using namespace std;

struct operatorSym {
    char symbol;
    int priority;
} orSym, concatSym, closureSym, posClosureSym, parenOpenSym, parenCloseSym;


class compare {
public:
    bool operator()(pair<string, vector<string>> a, pair<string, vector<string>> b) {
        return a.first.length() < b.first.length();
    }
};


vector<string> lines;
vector<pair<string, string>> regExpressions;
vector<pair<string, string>> postfixExpressions;

vector<string> keywords;
vector<string> punctuation;

vector<char> symbols;

priority_queue<pair<string, vector<string>>, vector<pair<string, vector<string>>>, compare> pq;

vector<State> states;
vector<pair<State, State>> allGraphs;

map<char, string> transitionType;

void setupOperators() {
    orSym.symbol = '|';
    orSym.priority = 1;

    concatSym.symbol = '~';
    concatSym.priority = 2;

    closureSym.symbol = '*';
    closureSym.priority = 3;

    posClosureSym.symbol = '+';
    posClosureSym.priority = 3;

    parenOpenSym.symbol = '(';
    parenOpenSym.priority = 4;

    parenCloseSym.symbol = ')';
    parenCloseSym.priority = 4;
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
    return (c == orSym.symbol || c == concatSym.symbol || c == closureSym.symbol || c == posClosureSym.symbol ||
            c == parenOpenSym.symbol
            || c == parenCloseSym.symbol);
}

bool isSymbol(char c) {
    return (std::find(symbols.begin(), symbols.end(), c) != symbols.end());
}

operatorSym selectOperator(char c) {
    if (c == orSym.symbol) return orSym;
    else if (c == concatSym.symbol) return concatSym;
    else if (c == closureSym.symbol) return closureSym;
    else if (c == posClosureSym.symbol) return posClosureSym;
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
    symbols.push_back('.');
    transitionType['.'] = EPS;
    symbols.push_back('#');
    transitionType['#'] = EPS;
    symbols.push_back('$');
    transitionType['$'] = EPS;

}

void identifyExp(vector<string> lines) {

    for (int i = 0; i < lines.size(); ++i) {
        size_t found = lines[i].find(":");
        if (found != std::string::npos) {
            regExpressions.push_back(
                    make_pair(lines[i].substr(0, found), lines[i].substr(found + 1, std::string::npos)));
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
            tokenReplace(2, -1, lines[i], "E", "$~");
        }

        if (lines[i].find(".") != string::npos) {
            tokenReplace(1, lines[i].find("."), lines[i], ".", "~.");
            cout << lines[i] << endl;
        }

        if (lines[i].find("\* |") != string::npos) {
            tokenReplace(1, lines[i].find("\* |"), lines[i], "\* |", "^ |");
            cout << lines[i] << endl;
        }

        lines[i].erase(remove(lines[i].begin(), lines[i].end(), ' '), lines[i].end());
        lines[i].erase(remove(lines[i].begin(), lines[i].end(), '\\'), lines[i].end());

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


std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

void extractKeywords() {

    for (int i = 0; i < lines.size(); ++i) {
        vector<string> keywordsTokens;
        if (lines[i][0] == '{') {
            keywordsTokens = split(lines[i], ' ');
            for (int j = 0; j < keywordsTokens.size(); ++j) {
                keywords.push_back(keywordsTokens[j]);
            }
        }
    }

}

void extractPunctuation() {
    for (int i = 0; i < lines.size(); ++i) {
        vector<string> puncTokens;
        if (lines[i][0] == '[') {
            lines[i].erase(remove(lines[i].begin(), lines[i].end(), '\\'), lines[i].end());

            puncTokens = split(lines[i], ' ');
            for (int j = 0; j < puncTokens.size(); ++j) {
                punctuation.push_back(puncTokens[j]);
            }
        }
    }

}


void constructPostfix() {

    for (int i = 0; i < regExpressions.size(); ++i) {
        stack<operatorSym> operators;
        queue<char> operands;

        for (int j = 0; j < regExpressions[i].second.length(); ++j) {

            char c = regExpressions[i].second[j];

            if (isOperator(c)) {
                operatorSym tempOperator = selectOperator(c);
                if (!operators.empty() && tempOperator.priority < operators.top().priority) {
                    operatorSym topOperator = operators.top();
                    if (topOperator.symbol != '(' && topOperator.symbol != ')') {
                        operands.push(topOperator.symbol);
                        operators.pop();
                    }
                    operators.push(tempOperator);
                } else
                    operators.push(tempOperator);
            } else {
                operands.push(c);
            }

            if (c == ')') {
                operators.pop();
                while (operators.top().symbol != parenOpenSym.symbol) {
                    char temp = operators.top().symbol;
                    if (temp != parenOpenSym.symbol)
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

        postfixExpressions.push_back(make_pair(regExpressions[i].first, makeString(operands)));
        cout << postfixExpressions[i].first << " --- " << postfixExpressions[i].second << endl;
    }

}

State addStart() {
    State start(false);
    start.set_start();

    return start;
}

State addAccepting() {
    State accepting(true);

    return accepting;
}

void makeOperation(stack<pair<State, State>> &graphsStack, char &i) {

    //dummy is to differ graph from single state

    /* grab graph 1 form stack*/
    pair<State, State> graph1;
    graph1 = graphsStack.top();
    graphsStack.pop();

    /*start and accepting for graph 1*/
    State start1 = graph1.first;
    State end1 = graph1.second;

    /* newly added terminals*/
    State start = addStart();
    State accepting = addAccepting();

    char EPS = '#';

    if (i == '*') {

        /* if single state*/
        if (end1.is_dummy_state()) {

            ///here it is not EPS, it is the accepting letter
            start.add_to_table(EPS, &start1);

            start1.add_to_table(EPS, &start);
            start1.add_to_table(EPS, &accepting);
            start.add_to_table(EPS, &accepting);
        } else {

            /* if full graph*/

            ///here it is also not EPS
            start.add_to_table(EPS, &start1);

            start.add_to_table(EPS, &accepting);
            start1.add_to_table(EPS, &start);
            end1.add_to_table(EPS, &accepting);

            end1.set_is_accepted(false);
        }
    } else if (i == '+') {

        /* if single state*/
        if (end1.is_dummy_state()) {
            ///here it is also not EPS
            start.add_to_table(EPS, &start1);

            start1.add_to_table(EPS, &start);
            start.add_to_table(EPS, &accepting);
        } else {
            /* if full graph*/

            //start.add_to_table(EPS, accepting);

            ///here it is also not EPS
            start.add_to_table(EPS, &start1);

            start1.add_to_table(EPS, &start);
            end1.add_to_table(EPS, &accepting);

            end1.set_is_accepted(false);
        }
    } else if (i == '|') {

        /*grab graph 2 form stack*/
        pair<State, State> graph2;
        graph2 = graphsStack.top();
        graphsStack.pop();

        /*start and accepting for graph 2*/
        State start2 = graph2.first;
        State end2 = graph2.second;

        /*construct OR graph*/
        start.add_to_table(EPS, &start1);
        start.add_to_table(EPS, &start2);

        /* graph 1 is single state */
        if (end1.is_dummy_state()) {
            start1.add_to_table(EPS, &accepting);
        } else {
            end1.add_to_table(EPS, &accepting);
            end1.set_is_accepted(false);
        }

        /* graph 2 is single state */
        if (end2.is_dummy_state()) {
            start2.add_to_table(EPS, &accepting);
        } else {
            end2.add_to_table(EPS, &accepting);
            end2.set_is_accepted(false);
        }

    } else if (i == '~') {

        pair<State, State> graph2;
        graph2 = graphsStack.top();
        graphsStack.pop();

        State start2 = graph2.first;
        State end2 = graph2.second;


        start = start1;
        /*if graph 1 is single state*/
        if (end1.is_dummy_state()) {
            start1.add_to_table(EPS, &start2);
        } else {
            end1.add_to_table(EPS, &start2);
            end1.set_is_accepted(false);
        }

        /*if graph 2 is single state*/
        if (end2.is_dummy_state()) {
            start2.add_to_table(EPS, &accepting);
            accepting = start2;
        } else {
            end2.add_to_table(EPS, &accepting);
            end2.set_is_accepted(false);
            accepting = end2;
        }
    }

    graphsStack.push(make_pair(start, accepting));
}

void evaluatePostfix() {

    for (int i = 0; i < postfixExpressions.size(); ++i) {
        cout << postfixExpressions[i].second << endl;

        if (i > 1)
            break;

        stack<pair<State, State>> graphsStack;
        for (int j = 0; j < postfixExpressions[i].second.length(); ++j) {

            char c = postfixExpressions[i].second[j];
            if (isSymbol(postfixExpressions[i].second[j])) {
                string type = transitionType[c];
                State state1(false);
                State dummy(false, true);
                graphsStack.push(make_pair(state1, dummy));

            } else if (isOperator(postfixExpressions[i].second[j])) {
                makeOperation(graphsStack, postfixExpressions[i].second[j]);
            }

        }
        pair<State, State> tempGraph;
        tempGraph.second.set_name(postfixExpressions[i].first);
        tempGraph = graphsStack.top();
        graphsStack.pop();

        allGraphs.push_back(tempGraph);
    }
}


pair<State, State> constructPuncGraph() {

    State start = addStart();
    State accepting = addAccepting();
    char EPS = '#';

    for (int i = 0; i < punctuation.size(); ++i) {
        State newState(false);
        char punc = punctuation[i][0];
        start.add_to_table(punc, &newState);
        newState.add_to_table(EPS, &accepting);
    }

    return make_pair(start, accepting);
}


pair<State, State> constructKWGraph() {
    char EPS = '#';

    State start = addStart();
    State accepting = addAccepting();

    vector<pair<State, State>> accumGraphs;

    for (int i = 0; i < keywords.size(); i++) {
        State oldState = addStart();
        for (int j = 0; j < keywords[i].length(); ++j) {
            State newState(false);
            oldState.add_to_table(keywords[i][j], &newState);
            oldState = newState;
        }
        oldState.add_to_table(EPS, &accepting);
        accumGraphs.push_back(make_pair(start, accepting));
    }

    State KWStart = addStart();
    State KWAccepting = addAccepting();
    for (int i = 0; i < accumGraphs.size(); i++) {
        KWStart.add_to_table(EPS, &accumGraphs[i].first);
        accumGraphs[i].second.add_to_table(EPS, &KWAccepting);
    }

    return make_pair(KWStart, KWAccepting);
}

pair<State, State> cosntructOpGraph() {

    vector<string> relopTokens;
    char EPS = '#';

    State start = addStart();
    State accepting = addAccepting();

    for (int i = 0; i < lines.size(); ++i) {

        int index = lines[i].find(":");

        string LHS = lines[i].substr(0, index);

        if (!LHS.compare("relop")) {
            relopTokens = split(lines[i].substr((index + 1)), '|');

            for (int j = 0; j < relopTokens.size(); ++j) {
                State newState(false);
                if (relopTokens[i][0] == '=' && relopTokens[i].length() == 1) {
                    newState.set_is_accepted(true);
                    newState.set_name("assign");
                }
                start.add_to_table(relopTokens[i][0], &newState);
            }

            for (int j = 0; j < relopTokens.size(); ++j) {
                if (relopTokens[i].length() > 1) {
                    State newState(false);
                    newState.set_name("relop");
                    newState.set_is_accepted(true);
                    start.add_to_table(relopTokens[i][0], &newState);

                    newState.add_to_table(EPS, &accepting);
                }
            }

        }
        if (!LHS.compare("addop")) {
            relopTokens = split(lines[i].substr((index + 1)), '|');

            State newState1(false);
            start.add_to_table(EPS, &newState1);

            State newState2(false);
            start.add_to_table(EPS, &newState2);

            State finalState(true);
            finalState.set_name("addop");
            newState1.add_to_table(EPS, &finalState);
            newState2.add_to_table(EPS, &finalState);

            finalState.add_to_table(EPS, &accepting);
        }

        if (!LHS.compare("mulop")) {
            relopTokens = split(lines[i].substr((index + 1)), '|');

            State newState1(false);
            start.add_to_table(EPS, &newState1);

            State newState2(false);
            start.add_to_table(EPS, &newState2);

            State finalState(true);
            finalState.set_name("mulop");
            newState1.add_to_table(EPS, &finalState);
            newState2.add_to_table(EPS, &finalState);

            finalState.add_to_table(EPS, &accepting);
        }
    }

    return make_pair(start, accepting);
}


pair<State, State> unionAllGraphs() {

    char EPS = '#';
    State globalStart = addStart();
    State globalAccepting = addAccepting();


    pair<State, State> puncGraph, KWGraph, operatorsGraph;
    puncGraph = constructPuncGraph();
    KWGraph = constructKWGraph();
    operatorsGraph = cosntructOpGraph();

    allGraphs.push_back(puncGraph);
    allGraphs.push_back(KWGraph);
    allGraphs.push_back(operatorsGraph);

    for (int i = 0; i < allGraphs.size(); ++i) {
        State subStart = allGraphs[i].first;
        State subAccepting = allGraphs[i].second;

        globalStart.add_to_table(EPS, &subStart);
        subAccepting.add_to_table(EPS, &globalAccepting);
    }


    ///add graphs of other lines(punc - keywords - operators)

    return make_pair(globalStart, globalAccepting);
}

pair<State, State> getNFA() {
    return unionAllGraphs();
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

void printGraph(State start) {
    map<char, vector<State *> > table = start.get_table();

    typedef std::map<char, vector<State *> >::iterator it_type;
    for (it_type iterator = table.begin(); iterator != table.end(); iterator++) {
        vector<State *> states = iterator->second;
        cout << iterator->first << endl;

        for (int j = 0; j < states.size(); ++j) {
            cout << "is acceptence " << states[j]->is_accept() << endl;

            if(states[j]->is_accept()){
                cout << "letter is " << endl;
            }

            if(iterator->first == '#'){
                printGraph(states[j]);
            }

        }
        // iterator->first = key
        // iterator->second = value
        // Repeat if you also want to iterate through the second map.
    }


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
    evaluatePostfix();
    State start = getNFA().first;
    printGraph(start);

    return 0;
}