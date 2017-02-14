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
    for(; cur_pop<max_pop; cur_pop++)
    {
        //brains.push_back(ann(nodes_per_layer, 
    }
}

GA::GA(const string& save_file) // Load from a save-file
    :
        brains()
{
    //TODO: do this -- Garrett
}

GA:: ~GA()
{
}
