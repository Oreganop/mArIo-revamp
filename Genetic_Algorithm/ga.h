#ifndef GA_H
#define GA_H

#include"../Network/ann.h"

using namespace std;

class GA
{
    private:
        int gen;
        int max_pop;
        int cur_pop;

        int num_parents;

        vector<int> nodes_per_layer;
        vector<ann> brains;

        /* ******* Functions ******* */

        //Genetic Algorithm functions
        void populate();
        void evaluate();
        void kill_off();

        void save();
        void load();
    public:
        GA(const int& pop, const int& p_pop, const vector<int>& layers);
        GA(const string& save_file);
        ~GA();
        void run();
};
#endif
