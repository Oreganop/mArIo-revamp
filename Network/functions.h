#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<vector>
#include<cmath>
#include<stdlib.h>
#define BRAIN_T vector<long double>
using std::vector;


BRAIN_T breed(BRAIN_T & mom,BRAIN_T & dad, vector<int>& nodes_per_layer);
BRAIN_T splice(BRAIN_T & mom,BRAIN_T & dad, vector<int>& nodes_per_layer);
void mutate(BRAIN_T & brain_net,vector<int>& nodes_per_layer);


#endif
