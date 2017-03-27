#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class State
{
    public:
        State();
        State(bool,string,map<char , vector<State*> >);
        State(bool);
        State(bool, bool);

        virtual ~State();


    /*Setter & getter*/
    void set_id(int);
    void set_name(string);
    void set_is_accepted(bool);
    void set_start();

    int get_id();
    map<char , vector<State*> > get_table();

    string get_name();
    bool is_accept();
    bool is_dummy_state();
    bool is_start_start();


    /*add next state can be reached form this state on input*/
    void add_to_table(char input, State* next);

    /*get all next state that reached by this state on input*/
    vector<State*> get_next_state(char input);

    /*print all transtion it table*/
    void print(void);

    protected:

    private:

    /*attributes*/

    bool is_accepted;                       /*if this state is acceptes*/
    bool is_dummy;
    bool is_start;

    string name;                            /* state name use to know type*/
    int id;                                 /*state id auto generate*/
    map<char , vector<State*> > table;       /*transition table of state*/

};

#endif // STATE_H
