/* Garrett Swann
 * ann.h
*/
#ifndef ANN_H
#define ANN_H
#include<fstream>
#include<vector>
#include<iostream>
#include<iomanip>      // std::setprecision
#include"functions.h"

using namespace std;

class ann
{
    private:
        friend class Agent;
        struct Node{
            long double weight; //Weight to next node. Output nodes weights will be unset.
            long double a;
            long double in;
            long double delta;
            long double dummy;
        };

    protected:
        // nodes_per_layer ends up defining the number of nodes in a given layer (wow, amirite)
        vector<int>nodes_per_layer;
        
        // graph[ LAYER_NUM ][ LAYER_NUM.node_A ][ LAYER_NUM+1.node_B ] = Weight from node_A to node_B
        vector<vector<vector<Node>>> graph;
        
        long double alpha;

    public:
        ann(
                const vector<int>& structure,       /* Defines the size of each layer */
                const vector<long double>* weights, /* NULL for random weights */
                const long double& alpha            /* Alpha value for calculations */
            );
        ann( 
                const ann& mommy,  /* Mom's ANN */
                const ann& daddy   /* Dad's ANN */
            );
        ann() {};
        ~ann();

        // With Test Files
        ann(const string&, const string&, const long double);
        void backprop(const string& train_inf, const string& train_outf, const int k);
        void classify(const string& classify_inf, const string& classify_outf);
        void printWeights(int precision);

};
#endif
