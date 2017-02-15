/* Garrett Swann
 * ann.cpp
*/

#include"ann.h"
using namespace std;


ann::ann(const vector<int>& structure, const bool& as_random, const long double &a)
    : 
        nodes_per_layer(structure),
        alpha(a)
{
    for(unsigned int i=0; i < structure.size(); i++)
    {
        int num_nodes = structure[i];

        graph.push_back(vector< vector<Node> >());


        for( int y=0; y<num_nodes;y++)
        {
            graph[graph.size()-1].push_back(vector<Node>()); // Pushback y'th vector
            if(i != structure.size()-1){
                for( int j=0; j< nodes_per_layer[i+1]; j++)
                {
                    long double weight = (as_random) ? random_weight()/(long double)2 : 0;
                    graph[ /*x*/ i ][ y ].push_back( Node{ weight, 0, 0, 0, a});
                }
            }
        }
    }

    for( int j=0; j<nodes_per_layer.back(); j++)
    {
        // Push output layer nodes, which don't get pushed anywhere else.
        graph[graph.size()-1][j].push_back(Node{0,0,0,0,a});  
    }
    //cout << "ALIVE";
}

//ann Destructor: Nothing to really say here.
ann::~ann()
{
    //cout << "ouch!\n";
}

void ann::printWeights(int precision)
{
    for(unsigned int layer=0; layer < nodes_per_layer.size()-1; layer++)
    {
        cout << "Layer "<<layer << ": ";
        for(int i=0; i< nodes_per_layer[layer]; i++)
        {
            for( int j=0; j< nodes_per_layer[layer+1]; j++)
            {
                cout << showpoint << fixed << setprecision(precision) << graph[layer][i][j].weight << " ";
            }
            cout << endl << "\t\t ";
        }
        cout << endl ;
        if(layer < nodes_per_layer.size()-2)
            cout << "\t";
    }
}

int ann:: sometimes_negative()
{
    return (rand()%2 == 0)?-1:1;
}

long double ann:: random_weight()
{
    return drand48() * (long double) sometimes_negative();
}


/* ********************** *
 * TEST-FUNCTION VARIANTS * 
 * ********************** */


ann::ann(const string& struct_file, const string& weight_file, const long double a)
    : alpha(a)
{
    ifstream structf;
    ifstream weightf;

    structf.open(struct_file, ifstream::in);
    weightf.open(weight_file, ifstream::in);

    int num_nodes;
    string in;
    while(structf >> in) // Reads in one layer at a time.
    {
        num_nodes = atoi(in.c_str()); // Num nodes in a given layer
        nodes_per_layer.push_back(num_nodes); // There will be num_nodes in any given layer.
        cout << num_nodes << endl;

        // nodes_per_layer ends up defining the number of nodes in a given layer (wow, amirite)

        graph.push_back(vector< vector<Node> >());
        for( int i=0; i<num_nodes;i++)
        {
            getline(weightf, in); 
            graph[graph.size()-1].push_back(vector<Node>()); // Pushback 
            int pos=0, lpos;
            do
            { 
                lpos = pos;
                pos = in.find(' ', ++pos); //last_pos to pos-lpos, where pos is index of a ' ', is the length of a (string) weight
                if (pos>0)
                {
                    graph[ /*x*/ graph.size()-1 ][ /*y*/ graph[ graph.size()-1 ].size()-1 ].push_back( Node{ (long double) atof(in.substr(lpos, pos-lpos).c_str()), 0, 0, 0, a});
                    // x is size()-1 because we're handling the current layer weights to the next layer
                    // y is the size-1 of the array we are pushing to. It's a vector of nodes that have a path from the current node we're looking at... if that makes sense.
                }
            }while(pos>0); // find returns -1 if not found
        }
    }
    cout << "WTF" << endl;

    for( int j=0; j<nodes_per_layer.back(); j++)
    {
        cout << j << endl;
        // Push output layer nodes, which don't get pushed anywhere else.
        graph[graph.size()-1][j].push_back(Node{0,0,0,0,a});  
    }

    structf.close();
    weightf.close();
}



void ann::backprop(const string& train_inf, const string& train_outf, const int k)
{
    for (int iter =0; iter < k; iter++)
    {
        ifstream inf;
        inf.open(train_inf);

        ifstream outf;
        outf.open(train_outf);

        string in;
        while(getline(inf, in))
        {
            string expect;
            outf >> expect;
            int pos=0, lpos, count=0;
            do
            { 
                lpos = pos;
                pos = in.find(' ', ++pos);
                if (pos>0)
                {
                    string one = in.substr(lpos, pos-lpos);
                    // Step 1: For each node in inpute layer, set a-j = x-j
                    graph[0][count][0].a = (long double) atof(one.c_str()); // Add a-value to input nodes.
                    count ++;
                }
            }while(pos>0);
            // At this point, a-sub-j is set for all input nodes.

            //Step 2: For each layer l=2 ... L do
            for (unsigned int on_layer=1; on_layer<graph.size(); on_layer++) // 2 -> L
            {
                // Step 3: For each node in j do
                for( int j=0; j<nodes_per_layer[on_layer]; j++)
                {
                    graph[on_layer] [j] [0].in = graph[on_layer] [j] [0].dummy; // Initialize to dummy weight.
                    for( int i=0; i<nodes_per_layer[on_layer-1]; i++)
                    {
                        // add sumation of other weights
                        graph[on_layer] [j] [0].in += graph[on_layer-1][i][j].weight * graph[on_layer-1][i][0].a;
                    }
                    // before we move on, we need to calculate a-sub-l from in-sub-l (which we have)
                    graph[on_layer] [j] [0].a = 1.0l/(1.0l+exp(-graph[on_layer][j][0].in));
                }
            }
            // Now we have a-value for each node. We can calculate error.
            
            // Error for output layer:
            for ( int j = 0; j<nodes_per_layer[graph.size()-1]; j++)
            {
                long double y = j==(atoi(expect.c_str())) ? .1l : .9l;

                graph[graph.size()-1][j][0].delta = graph[graph.size()-1][j][0].a * (1- graph[graph.size()-1][j][0].a) * (y- graph[graph.size()-1][j][0].a);
            }
            for (int l=(signed)graph.size()-2; l>=0; l--) // Loop thru each other layer
            {
                for ( int i = 0; i<nodes_per_layer[l]; i++)
                {
                    graph[l][i][0].delta = 0;

                    // Loop thru nodes node-i goes to
                    for (int j=0; j<nodes_per_layer[l+1]; j++) 
                    {
                        graph[l][i][0].delta += graph[l+1][j][0].delta * graph[l][i][j].weight;
                    }
                    graph[l][i][0].delta *= graph[l][i][0].a * (1 - graph[l][i][0].a);
                }
            }
            // Now we update our weights, given our error
              
            //Dummy weights first, since I store them differently.
            for ( unsigned int layer=0; layer< graph.size(); layer++)
            {
                for ( int i=0; i<nodes_per_layer[layer]; i++)
                {
                    graph[layer] [i] [0].dummy = graph[layer] [i] [0].dummy + alpha * 1 * graph[layer] [i] [0].delta;
                    for (unsigned int j = 0; j <graph[layer][i].size()-1; j++)
                    {
                        graph[layer][i][j].weight = graph[layer][i][j].weight + (alpha * graph[layer][i][0].a * graph[layer+1][j][0].delta );
                    }
                }
            }
        }
        // A lot of comments were here.
        inf.close();
        outf.close();
    }
}

void ann::classify(const string& classify_inf, const string& classify_outf)
{
    ifstream inf;
    inf.open(classify_inf);

    ifstream outf;
    outf.open(classify_outf);

    string in;
    int total=0;
    int correctly_classified=0;

    while(getline(inf, in))
    {
        total++;
        string expect;
        outf >> expect;
        int pos=0, lpos, count=0;
        do
        { 
            lpos = pos;
            pos = in.find(' ', ++pos);
            if (pos>0)
            {
                string one = in.substr(lpos, pos-lpos);
                // Step 1: For each node in inpute layer, set a-j = x-j
                graph[0][count][0].a = (long double) atof(one.c_str()); // Add a-value to input nodes.
                count ++;
            }
        }while(pos>0);
        // At this point, a-sub-j is set for all input nodes.

        //Step 2: For each layer l=2 ... L do
        for ( unsigned int on_layer=1; on_layer<graph.size(); on_layer++) // 2 -> L
        {
            // Step 3: For each node in j do
            for( int j=0; j<nodes_per_layer[on_layer]; j++)
            {
                graph[on_layer] [j] [0].in = graph[on_layer] [j] [0].dummy; // Initialize to dummy weight.
                
                for( int i=0; i<nodes_per_layer[on_layer-1]; i++)
                {
                    // add sumation of other weights
                    graph[on_layer] [j] [0].in += graph[on_layer-1][i][j].weight * graph[on_layer-1][i][0].a;
                }
                // before we move on, we need to calculate a-sub-l from in-sub-l (which we have)
                graph[on_layer] [j] [0].a = 1.0l/(1.0l+exp(-graph[on_layer][j][0].in));
            }
        }
        //Time to do Euclidian distance on output a-values
        long double* cumulate= new long double[graph.size()]; 
        long double smallest = 1000000000000; // Initialized as a large number
        int small_i= -1;
        for(int i=0; i<nodes_per_layer[graph.size()-1]; i++)
        {
            cumulate[i] = 0;
            for(int j=0; j<nodes_per_layer[graph.size()-1]; j++)
            {
                long double y = i==j ? .1l : .9l; // if they equal, y should be .1, else .9
                cumulate[i] += pow(graph[graph.size()-1][j][0].a - y, 2.0l);
            }
            if (smallest > cumulate[i])
            {
                small_i = i;
                smallest = cumulate[i];
            }
        }
        if ( atoi(expect.c_str()) == small_i)
            correctly_classified++;
        cout << small_i << endl;
        delete [] cumulate;
    }
    long double accuracy = (long double) correctly_classified/total;
    cout << showpoint << fixed << setprecision(12) << accuracy << endl;
    inf.close();
    outf.close();
}
