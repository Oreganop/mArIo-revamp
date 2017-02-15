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


GA::Agent::Agent(int& b, int f, string fn, string ln, vector<int>& s, bool r, long double a)
    :
        born(b),
        fitness(f),
        name_f(fn),
        name_l(ln),
        brain(s, r, a)
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
    }
}

void GA:: populate()
{
    if( gen == 0 )
    {
        for(; cur_pop<max_pop; cur_pop++)
        {
            brains.push_back(
                    new Agent(
                        gen,
                        0,
                        ((use_names) ? names_f.front() : string("Agent#")),
                        ((use_names) ? names_l.front() : string(to_string(agent_counter))),
                        nodes_per_layer, /*yes random*/ true, .01
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
        delete brains[cur_pop-1];
        brains.erase(brains.end()-1);
        cout << brains.back()->get_name() << endl;
    }
}

bool comp(GA::Agent* i, GA::Agent* j)
{ 
    return i->fitness > j->fitness; 
}
