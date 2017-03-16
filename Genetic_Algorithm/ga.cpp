#include"ga.h"

GA::GA(const int& pop, const int& p_pop, const vector<int>& n_p_l, const string& name_file)
    :
        gen(0),
        max_pop(pop),
        cur_pop(0),
        agent_counter(0),
        num_parents(p_pop),
        nodes_per_layer(n_p_l),
        brains()
{
    srand(time(NULL));
    srand48(time(NULL));

    fstream in(name_file, fstream::in);
    if (in.is_open() ){
        string name;
        for (int i=0; getline(in, name); i++)
        {
            if(i<28)
                names_l.push_back(name);
            else
                names_f.push_back(name);
        }
        use_names=true;
    }
    else {
        use_names=false;
    }
    
}

GA::GA(const string& save_file) // Load from a save-file
    :
        max_pop(0),
        num_parents(0),
        brains()
{
    //TODO: do this -- Garrett
}

GA:: ~GA()
{
}


Agent::Agent(int& b, string fn, string ln, vector<int>& s, vector<long double>* w , long double a)
    :
        born(b),
        fitness(0),
        name_f(fn),
        name_l(ln),
        brain(s, w, a)
{
}

Agent::Agent(int& d, string fn, string ln, Agent& mommy, Agent& daddy)
    : 
        born(d),
        fitness(0),
        name_f(fn),
        name_l(ln),
        brain() //  is done in a "sexier" way below
{
}

void GA:: run()
{
    //while(1)
    { 
        populate();
        evaluate();
        kill_off();

        gen++;


        cout << "The parents of Generation <" << gen << "> are:"; 
        printBrains();
    }
}

void GA:: populate()
{
    cout << endl;
    if( gen == 0 )
    {
        for(; cur_pop<max_pop; cur_pop++)
        {
            brains.push_back(
                    new Agent(
                        gen,
                        //agent_counter, TODO maybe? -- Garrett
                        ((use_names) ? names_f.front() : string("Agent#")),
                        ((use_names) ? names_l.front() : string(to_string(agent_counter))),
                        nodes_per_layer, /*yes random*/ NULL, .01
                    ));
            if(use_names)
            {
                names_f.pop_front();
                names_l.pop_front();
            }
            cout << agent_counter << endl;
            agent_counter++;
        }
        return;
    }
}
void GA:: evaluate()
{
}
void GA:: kill_off()
{
    sort(brains.begin(), brains.end(), comp);
    for(;cur_pop>num_parents; cur_pop--)
    {
        cout << brains.back()->get_name() << endl;
        delete brains[cur_pop-1];
        brains.erase(brains.end()-1);
    }

}

void GA:: printBrains()
{
    cout << endl;
    for (unsigned int i=0; i<brains.size(); i++)
    {
        cout << "\t" << brains[i]->get_name() << " has a fitness of " <<  brains[i]->fitness << endl;
    }
}

bool comp(Agent* i, Agent* j)
{ 
    return i->fitness > j->fitness; 
}
