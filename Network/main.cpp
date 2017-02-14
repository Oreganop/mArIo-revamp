/*
 * Garrett Swann
 * Generic Driver file for Artificial Neural Network implementation
*/

#include"ann.h"
#include<iostream>

int main(int argc, char** argv)
{
    ann myann(argv[5], argv[6], .01l);
    myann.backprop(argv[1], argv[2], atoi(argv[7]));
    myann.printOneInputWeights();
    myann.classify(argv[3], argv[4]);
}
