#include<iostream>
#include<bits/stdc++.h>
#include "State.h"

State::State() {

}

State::State(bool is_accepted, string name, map<char, vector<State *> > table) {
    this->is_accepted = is_accepted;
    this->name = name;
    this->table = table;
    this->is_start = false;
}

State::State(bool state) {
    this->is_accepted = state;
    this->is_start = false;
}

void State::set_start() {
    this->is_start = true;
}

bool State::is_dummy_state() {
    return this->is_dummy;
}

bool State::is_start_start() {
    return this->is_start;
}

State::State(bool is_accepted, bool is_dummy) {
    this->is_accepted = is_accepted;
    this->is_dummy = is_dummy;
}

State::~State() {
    //dtor
}


void State::set_id(int id) {
    this->id = id;
}

void State::set_name(string name) {
    this->name = name;
}

void State::set_is_accepted(bool is_accepted) {
    this->is_accepted = is_accepted;
}

int State::get_id() {
    return this->id;
}

string State::get_name() {
    this->name;
}

bool State::is_accept() {
    return this->is_accepted;

}

/*add next state can be reached form this state on input*/
void State::add_to_table(char input, State *next) {
    this->table[input].push_back(next);
}

/*get all next state that reached by this state on input*/
vector<State *> State::get_next_state(char input) {
    return this->table[input];
}

/*print all transtion it table*/
void State::print(void) {
    typedef map<char, vector<State *> >::const_iterator MapIterator;
    for (MapIterator iter = this->table.begin(); iter != this->table.end(); iter++) {
        cout << "Key: " << iter->first << endl << "Values:" << endl;
        vector<State *> v = iter->second;
        for (int i = 0; i < (int) v.size(); i++) {
            cout << v[i]->get_id() << " ";
        }
        cout << endl;
    }
}
