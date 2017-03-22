#ifndef GA_H
#define GA_H

#include"../Network/ann.h"
#include<algorithm>
#include<deque>

using namespace std;



class Agent{
    private:
        int born;
        //int agent_num; // TODO maybe? -- Garrett
        int fitness;
        string name_f;
        string name_l;
    protected:
        ann brain;
    public:
        Agent(int& date_born, string first, string last, vector<int>& structure, long double a);
        Agent(int& date_born, string first, string last, Agent& mommy, Agent& daddy, const int mutation_rate);

        ~Agent(){/*cout<< "d A\n";*/};

        string get_name() {return  "["+name_f+ " " +name_l+"]";};

        friend bool comp(Agent* i, Agent* j);
        friend class GA;
};


class GA
{
    private:
        int gen;
        const int max_pop;
        int cur_pop;


        
        int agent_counter;
        deque<string> names_f; //Last names
        deque<string> names_l; //Fist names
        bool use_names;


        bool just_dead;
        string in_file;
        string out_file;

        const int num_parents;

        vector<int> nodes_per_layer;
        vector<Agent*>brains;

        /* ******* Functions ******* */

        //Genetic Algorithm functions
        void populate();
        void evaluate();
        void kill_off();

        //Functions
        void printBrains();

        void save();
        void load();



    public:
        GA(const int& pop, const int& p_pop, const vector<int>& layers, const string& name_file);
        GA(const string& save_file);
        ~GA();

        // Init functions
        void observe_from(const string& obs);
        void reactions_to(const string& react);

        void run();
};
bool comp(Agent* i, Agent* j);
#endif
