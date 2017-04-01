#ifndef GA_H
#define GA_H

#include"../Network/ann.h"
#include<algorithm>
#include<deque>

using namespace std;



class Agent{
    private:
        int born;
        double fitness[4];
        string name_f;
        string name_l;

        int agent_id;

        int mom_genes;
        int dad_genes;
        int mutant_genes;
    protected:
        ann brain;
    public:
        Agent(int& date_born,
                string first,
                string last,
                int id,
                vector<int>& structure,
                const vector<long double>& weights,
                long double a);

        Agent(int& date_born,
                string first,
                string mom_last,
                string dad_last,
                int id,
                Agent& mommy,
                Agent& daddy,
                const int mutation_rate);

        ~Agent(){ };

        string get_name() {return "["+name_f + name_l+"]";};
        string get_id() { return to_string(agent_id);};

        void save(string path);

        friend bool comp(Agent* i, Agent* j);
        friend class GA;
};


class GA
{
    private:
        // For GA control
        int m_gen;
        const int max_pop;
        int cur_pop;
        const int num_parents;

        // For Agents
        vector<Agent*>brains;
        vector<int> nodes_per_layer;
        int agent_counter;
        deque<string> names_f; //Last names
        deque<string> names_l; //Fist names
        bool use_names;

        // For Lua-C++ communication
        bool just_dead;
        string in_file;
        string out_file;

        // For Saving
        string save_path;
        string prefix;
        bool save;


        /* ******* Functions ******* */

        //Genetic Algorithm functions
        void populate();
        void evaluate();
        void kill_off();

        //Functions
        void printBrains();
        void printParents();




    public:
        GA(const int& pop, const int& p_pop, const vector<int>& layers);
        ~GA();

        // Init functions
        void observe_from(const string& obs);
        void reactions_to(const string& react);

        void load(string& file);
        void set_save_dir(string& dir);
        void set_save_prefix(string& pfix);
        void add_first(string name);
        void add_last(string name);

        void run();
};
bool comp(Agent* i, Agent* j);
#endif
