#ifndef GA_H
#define GA_H

#include"../Network/ann.h"
#include<algorithm>

using namespace std;



class GA
{
    private:
        class Agent{
            public:
                int born;
                int fitness;
                ann* brain;

                Agent(int&, int, ann*);
                ~Agent(){delete brain;cout<< "d A\n";};
        };


        int gen;
        const int max_pop;
        int cur_pop;

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
        GA(const int& pop, const int& p_pop, const vector<int>& layers);
        GA(const string& save_file);
        ~GA();
        void run();
};
bool comp(GA::Agent* i, GA::Agent* j);
#endif
