/* Garrett Swann
 * ann.h
*/
#ifndef ANN_H
#define ANN_H
#include<fstream>
#include<vector>
#include<cmath>
#include<stdlib.h>
#include<iostream>
#include<iomanip>      // std::setprecision

using namespace std;

const vector<long double> __DUMMY_EMPTY_VECTOR;

class ann
{
    private:
        friend class Agent;
        struct Node{ // Processing Unit

            /*
             Weights ---\
                         \   ~~~Neuron~~~
             Weights -----[ in | g(in) | a ] -> Output
                         /       ^^^^^
             Weights ---/        g(in) - Activation Function coded is logarithmic -- 1/(1+ exp(-in)
             */
            long double weight; //Weight to next node. Output nodes weights will be unset.
            long double a;
            long double in;
            long double delta;
            long double dummy;
        };


        /******************* *
         * Helpful functions *
         * ******************/

        /* Returns random (+ | -) */
        int sometimes_negative();    
        /* Get random value 0 <-> 1 */
        long double random_weight(); 

    protected:
        // nodes_per_layer ends up defining the number of nodes in a given layer (wow, amirite)
        vector<int>nodes_per_layer;
        
        // graph[ LAYER_NUM ][ LAYER_NUM.node_A ][ LAYER_NUM+1.node_B ] = Weight from node_A to node_B
        vector<vector<vector<Node>>> graph;
        
        long double alpha;

    public:
        ann(
                const vector<int>& structure,       /* Defines the size of each layer */
                const vector<long double> weights, /* NULL for random weights */
                const long double& alpha            /* Alpha value for calculations */
            );
        ann( 
                const ann& mommy   /* Mom's ANN */,
                const ann& daddy   /* Dad's ANN */,
                int& mommy_genes    /* Decide which Parent name to use*/,
                int& daddy_genes    /* Decide which Parent name to use*/,
                int& mutant_genes    /* for stats*/,
                const int mutation_rate /* Mutation happens 1:mutation_rate of the time*/
            );
        ann() {};
        ~ann();

        void output_as_binary(const vector<long double>& observations, vector<bool>& output);



        // With Test Files
        ann(const string&, const string&, const long double);
        void backprop(const string& train_inf, const string& train_outf, const int k);
        void classify(const string& classify_inf, const string& classify_outf);
        void printWeights(int precision);

};
#endif
