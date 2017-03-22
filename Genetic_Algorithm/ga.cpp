#include"ga.h"

GA::GA(const int& pop, const int& p_pop, const vector<int>& n_p_l, const string& name_file)
    :
        gen(0),
        max_pop(pop),
        cur_pop(0),
        agent_counter(0),
        just_dead(true),
        in_file("Lua/observations.txt"),
        out_file("Lua/reactions.txt"),
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

void GA::observe_from(const string& obs)
{
    in_file = obs;
}

void GA::reactions_to(const string& react)
{
    out_file = react;
}


GA:: ~GA()
{
}


Agent::Agent(int& b, string fn, string ln, vector<int>& s, long double a)
    :
        born(b),fitness(0),name_f(fn),name_l(ln), brain(s, __DUMMY_EMPTY_VECTOR, a)
{
}

Agent::Agent(int& d, string fn, string ln, Agent& mommy, Agent& daddy, const int mutation_rate)
    : 
        born(d),
        fitness(0),
        name_f(fn),
        name_l(ln),
        brain(mommy.brain, daddy.brain, mutation_rate)// is done in a "sexier" way below
{
}

void GA:: run()
{
    while(1)
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
                        nodes_per_layer, .01
                    ));
            if(use_names)
            {
                names_f.pop_front();
                names_l.pop_front();
            }
            agent_counter++;
        }
        return;
    }
    for(; cur_pop<max_pop; cur_pop++)
    {
        int mommy = rand() % num_parents;
        int daddy = rand() % num_parents;
        brains.push_back(
                new Agent(
                    gen,
                    //agent_counter, TODO maybe? -- Garrett
                    ((use_names) ? names_f.front() : string("Agent#")),
                    ((use_names) ? names_l.front() : string(to_string(agent_counter))),
                    *brains.at(mommy),
                    *brains.at(daddy),
                    50 // 1/50 chance at random mutation
                ));
        if(use_names)
        {
            names_f.pop_front();
            names_l.pop_front();
        }
        agent_counter++;
    }
}
void GA:: evaluate()
{
    
    for(int i = gen?num_parents:0; i<max_pop; i++)
    {
        cout<< "We're on Mario #" << i << endl;

        int cur(-1);
        string got;

        if(brains[i]->born != gen)
        {
            cout<< " Fitness was <"<<brains[i]->fitness;
            cout << gen - brains[i]->born << "] Generations old.";
        }
        cout << " Code name: " << brains[i]->get_name() << endl;
        while(1)
        {
            fstream in(in_file, ios_base::in);
            getline(in, got); // Get first line, which is iteration number.
            if (got[0] == 'D' && !just_dead)
            {
                just_dead = 1;
                getline(in, got);
                brains[i]->fitness = atoi(got.c_str());/* fitness at end */
                cout << "\tMario's fitness: " << brains[i]->fitness << endl;
                break;
            }
            else if (got[0] == 'D' && just_dead)
                continue;
            else
                just_dead =0;

            vector<long double> values;

            if( cur != atoi(got.c_str()) ) 
            {
                int temp = atoi(got.c_str());
                in.flush();
                long double val;
                while( in >> val )
                {
                    // TODO parse a line and store it to be sent to something else...
                    // Line delimited by type. Otherwise space delimited.
                    if (val == temp)
                    {
                        cur = temp;
                    }
                    else
                        values.push_back( val/100 );

                }
                if( cur != temp )
                {
                    continue;
                }
                // Read in everything we need.
                in.close();


                //Pass everything needed to GA, and wait to receive corresponding game input. 

                /* TODO replace this
                   ann myann(rand_weights);
                   vector<int>* myints = myann.get_output( values ); 
                   */
                vector<bool> binary_out; 
                brains[i]->brain.output_as_binary( values, binary_out ); 

                fstream out(out_file, ios_base::out);

                if (binary_out[0] == 1)
                {
                    out << "A ";
                    //cout << "A " << endl;
                }
                if (binary_out[1] == 1)
                {
                    out << "Right ";
                    //cout << "Right " << endl;
                }
                if ( binary_out[0] == 0 && binary_out[1] == 0)
                {
                    out << "None ";
                }

                out.close();

            }
        }
    }
}
void GA:: kill_off()
{
    random_shuffle(brains.begin(), brains.end());
    sort(brains.begin(), brains.end(), comp);
    for(;cur_pop>num_parents; cur_pop--)
    {
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
