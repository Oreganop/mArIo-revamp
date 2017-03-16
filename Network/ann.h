/* Garrett Swann
 * ann.h
*/
#ifndef ANN_H
#define ANN_H
#include<fstream>
#include<cmath>
#include<vector>
#include<iostream>
#include<iomanip>      // std::setprecision

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
        vector<int>nodes_per_layer;
        // nodes_per_layer ends up defining the number of nodes in a given layer (wow, amirite)
        
        // ANN :
        vector<vector<vector<Node>>> graph;
        // graph[ LAYER_NUM ][ LAYER_NUM.node_A ][ LAYER_NUM+1.node_B ] = Weight from node_A to node_B
        
        long double alpha;

        // helpful functions
        int sometimes_negative();    /* Returns random (+ | -) */
        long double random_weight(); /* Get random value 0 <-> 1 */

    public:
        ann(
                const vector<int>& structure,       /* Defines the size of each layer */
                const vector<long double>* weights, /* NULL for random weights */
                const long double& alpha            /* Alpha value for calculations */
            );
        ann( 
                vector<vector<vector<Node>>>& mommy,  /* Mom's ANN */
                vector<vector<vector<Node>>>& daddy,  /* Dad's ANN */
                vector<int>nodes_per_layer            /* structre of the ANN's */
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
