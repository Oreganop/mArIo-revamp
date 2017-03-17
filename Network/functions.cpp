#include "functions.h"


int sometimes_negative()
{
    return (rand()%2 == 0)?-1:1;
}

long double random_weight()
{
    return drand48() * (long double) sometimes_negative();
}


BRAIN_T breed(BRAIN_T &mom,BRAIN_T &dad, vector<int> &nodes_per_layer)
{
    BRAIN_T child;
    child.resize(nodes_per_layer.size());
    for(int cur_layer=0; cur_layer < nodes_per_layer.size()-1; cur_layer++)
    {
        child[cur_layer].resize( nodes_per_layer[cur_layer] );
        for( int next_layer=0; next_layer < nodes_per_layer[cur_layer]; next_layer++)
        {
            child[cur_layer].resize( nodes_per_layer[cur_layer] );

            //TODO Right about here
        }
    }

    for(int cur_layer=0; cur_layer < nodes_per_layer.size(); cur_layer++)
    {
        int splice_place = rand() % nodes_per_layer[cur_layer];
        for(int i=0; i<nodes_per_layer[cur_layer]; i++)
        {
            child.
        }
    }
}

BRAIN_T splice(BRAIN_T &mom,BRAIN_T &dad, vector<int> &nodes_per_layer)
{
    return mom;
}

void mutate(BRAIN_T &brain_net,vector<int> &nodes_per_layer)
{
}
