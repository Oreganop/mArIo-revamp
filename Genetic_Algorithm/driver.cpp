#include"ga.h"

using namespace std;
int main(int argc, char** argv)
{
    if(argc < 5) // A lot of argument logic cuz dum
    {
        if(argc == 3){ //TODO: do loading -Garrett
            if( string(argv[1]) == "load" ){
                cout << "Loading not implemented yet. lol" << endl;
                cout << "load" << argv[2] << endl;
            }
            return 0;
        }
        else{
            cerr << "\nNot enough commandline arguments\n\n";
            cerr << argv[0] << " <num_parents> <num_children> <name_file> <num_inputs> ... <num_outputs>\n\tor \n";
            cerr << argv[0] << " load <file_to_load> \n\n";


            cerr << "\t-num_parents -- Number of Agents kept after a generation ends.\n";
            cerr << "\t-num_children -- Number of Agents to spawn in each new generation.\n";

            cerr << "\t-name_file -- Name file for Agents. 'c' to use counts\n";

            cerr << "\t-num_inputs -- Number of nodes in the input layer.\n";
            cerr << "\t-<...> -- As many numbers for number of nodes in hidden layer(s).\n";
            cerr << "\t-num_outputs -- Number of nodes in the output layer.\n";
            return -1;
        }
    }
    int num_parents = atoi(argv[1]);
    int num_children = atoi(argv[2]);

    vector<int> nodes_per_layer;

    for(int i=4; i< argc; i++) // Fill up Nodes_per_layer
    {
        nodes_per_layer.push_back(atoi(argv[i]));
        cout << nodes_per_layer.back() << endl;
    }
    GA genetics(num_children+num_parents, num_parents, nodes_per_layer, string(argv[3]));
    genetics.run();
}
