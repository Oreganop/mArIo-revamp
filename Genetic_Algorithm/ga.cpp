#include"ga.h"

GA::GA(const int& pop, const int& p_pop, const vector<int>& n_p_l)
    :
        gen(0),
        max_pop(pop),
        cur_pop(0),
        num_parents(p_pop),
        nodes_per_layer(n_p_l),
        brains()
{
    srand(time(NULL));
    srand48(time(NULL));
}

GA::GA(const string& save_file) // Load from a save-file
    :
        max_pop(0),
        num_parents(0),
        brains()
{
    //TODO: do this -- Garrett
}

GA:: ~GA()
{
}


GA::Agent::Agent(int& b, int f, ann* br)
    :
        born(b),
        fitness(f),
        brain(br)
{

}

void GA:: run()
{
    //while(1)
    { 
        populate();
        evaluate();
        kill_off();

        gen++;
    }
}

void GA:: populate()
{
    if( gen == 0 )
    {
        for(; cur_pop<max_pop; cur_pop++)
        {
            brains.push_back(
                    new Agent(
                        gen,
                        cur_pop,
                        new ann{nodes_per_layer, /*yes random*/ true, .01}
                    ));
            cout << cur_pop << endl;
        }
        return;
    }
}
void GA:: evaluate()
{
}
void GA:: kill_off()
{
    sort(brains.begin(), brains.end(), comp);
    for(;cur_pop>num_parents; cur_pop--)
    {
        delete brains[cur_pop-1];
        brains.erase(brains.end()-1);
    }
}

bool comp(GA::Agent* i, GA::Agent* j)
{ 
    return i->fitness > j->fitness; 
}
