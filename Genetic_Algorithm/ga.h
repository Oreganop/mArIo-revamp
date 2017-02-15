#ifndef GA_H
#define GA_H

#include"../Network/ann.h"
#include<algorithm>
#include<deque>

using namespace std;



class GA
{
    private:
        class Agent{
            public:
                int born;
                int fitness;
                string name_f;
                string name_l;

                ann brain;

                Agent(int&, int, string fn, string ln, vector<int>&, bool, long double);
                ~Agent(){/*cout<< "d A\n";*/};

                string get_name() {return name_f+name_l;};
        };


        int gen;
        const int max_pop;
        int cur_pop;


        
        int agent_counter;
        deque<string> names_f; //Last names
        deque<string> names_l; //Fist names
        bool use_names;

        const int num_parents;

        vector<int> nodes_per_layer;
        vector<Agent*>brains;

        /* ******* Functions ******* */

        //Genetic Algorithm functions
        void populate();
        void evaluate();
        void kill_off();

        void save();
        void load();
        friend bool comp(Agent* i, Agent* j);


    public:
        GA(const int& pop, const int& p_pop, const vector<int>& layers, const string& name_file);
        GA(const string& save_file);
        ~GA();
        void run();
};
bool comp(GA::Agent* i, GA::Agent* j);
#endif
