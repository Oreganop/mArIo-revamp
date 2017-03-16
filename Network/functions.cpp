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
    return mom;
}

BRAIN_T splice(BRAIN_T &mom,BRAIN_T &dad, vector<int> &nodes_per_layer)
{
    return mom;
}

void mutate(BRAIN_T &brain_net,vector<int> &nodes_per_layer)
{
}
