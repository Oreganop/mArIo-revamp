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
#
using namespace std;

class ann
{
    private:
        struct Node{
            long double weight; //Weight to next node. Output nodes weights will be unset.
            long double a;
            long double in;
            long double delta;
            long double dummy;
        };
        vector<int>nodes_per_layer;
        // nodes_per_layer ends up defining the number of nodes in a given layer (wow, amirite)

        vector<vector<vector<Node>>> graph;
        /* 
           Vector[ Identify Layer]  *(ie. input, hidden1, hidden2... hiddenN, output)
                    |
                    -> Node [ Identify i ]   *(ie. FROM which node. Path from i TO j)
                                |
                                -> Node[ Identify j ]   *(ie. TO which node. j FROM i)
          
        */
        const long double alpha;

        // helpful functions
        int sometimes_negative();
        long double random_weight();

    public:
        ann(const string&, const string&, const long double);
        ann(const vector<int>& structure, const bool& as_random, const long double alpha);
        ~ann();

        void backprop(const string& train_inf, const string& train_outf, const int k);
        void classify(const string& classify_inf, const string& classify_outf);
        void printOneInputWeights();

};
#endif
