#include"ga.h"

GA::GA(const int& pop, const int& p_pop, const vector<int>& n_p_l)
    :
        m_gen(0),
        max_pop(pop),
        cur_pop(0),
        num_parents(p_pop),

        brains(),
        nodes_per_layer(n_p_l),
        agent_counter(0),
        use_names(0),

        just_dead(true),
        in_file("Lua/observations.txt"),
        out_file("Lua/reactions.txt"),

        prefix(""),
        save(0)
{
    srand(time(NULL));
    srand48(time(NULL));
    
}


void Agent::save(string save_file)
{
    ofstream out(save_file);
    cout << out.is_open() << endl;
    for(unsigned int i=0; i< brain.nodes_per_layer.size(); i++)
        out << brain.nodes_per_layer[i] << " ";
    out << endl;

    for(unsigned int layer=0; layer<brain.nodes_per_layer.size()-1; layer++)
    {
        for( int i=0; i<brain.nodes_per_layer[layer]; i++)
        {
            for( int j=0; j<brain.nodes_per_layer[layer+1]; j++)
            {
                out << brain.graph[layer][i][j].weight<<endl;
            }
        }
    }
}

void GA::set_save_prefix(string& pfix)
{
    prefix = pfix;
}

void GA::add_first(string name)
{
    names_f.push_back(name);
    use_names = true;
}
void GA::add_last(string name)
{
    names_l.push_back(name);
    use_names = true;
}

void GA::set_save_dir(string& dir)
{
    save_path = dir;
    save = true;
}

void GA::load(string& file)
{
    ifstream in(file);
    string input;
    int pos=0, lpos, count=0;

    getline(in, input);


    do
    { 
        lpos = pos;
        pos = input.find(' ', ++pos);
        if (pos>0)
        {
            string one = input.substr(lpos, pos-lpos);
            cout << atoi(one.c_str()) << " ";
            if(nodes_per_layer[count] != atoi(one.c_str()))
            {
                cout << endl;
                cerr << "Invalid load file. Uneven brain structure." << endl;
                exit(-1);
            }
            count ++;
        }
        cout << endl;
    }while(pos>0);


    vector<long double> weights;
    for(unsigned int layer=0; layer< nodes_per_layer.size()-1; layer++)
    {
        for( int i=0; i<nodes_per_layer[layer]; i++)
        {
            for( int j=0; j<nodes_per_layer[layer+1]; j++)
            {
                getline(in, input);
                weights.push_back(atof(input.c_str()));
            }
        }
    }

    brains.push_back(
            new Agent(
                m_gen,
                ((use_names) ? names_f.front() : string("Agent#")),
                ((use_names) ? names_l.front() : string(" "+to_string(agent_counter))),
                agent_counter,
                nodes_per_layer,
                weights, .01
            ));

    cur_pop++;
    if(use_names)
    {
        names_f.pop_front();
        names_l.pop_front();
    }
    agent_counter++;
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


Agent::Agent(int& b,
        string fn,
        string ln,
        int id,
        vector<int>& s,
        const vector<long double>&weights,
        long double a)
    :
        born(b),
        fitness{0,0,0,0},
        name_f(fn),
        name_l(ln),
        agent_id(id),
        mom_genes(0),
        dad_genes(0),
        brain(s, weights, a) // Idk why I bother with the dummy empty vector, but lol
{
}

Agent::Agent(int& d, string fn, string m_ln, string d_ln, int id, Agent& mommy, Agent& daddy, const int mutation_rate)
    : 
        born(d),
        fitness{0,0,0,0},
        name_f(fn),
        agent_id(id),
        mom_genes(0),
        dad_genes(0),
        mutant_genes(0),
        brain(mommy.brain, daddy.brain, mom_genes, dad_genes, mutant_genes, mutation_rate)
{
    int total = mutant_genes+mom_genes+dad_genes;
    cout << "\t\t"<<(float)mom_genes/total*100<<"% mom, "<<(float)dad_genes/total*100<<"% dad, and "<<(float)mutant_genes/total*100<<"% mutant.\n";
    if( mom_genes > dad_genes )
        name_l = m_ln;
    else
        name_l = d_ln;
    cout << "\t\tSay hello to "+get_name()<<endl;
    
}

void GA:: run()
{
    random_shuffle(names_f.begin(), names_f.end());
    random_shuffle(names_l.begin(), names_l.end());
    while(1)
    { 
        cout << "\nPOPULATE\n";
        populate();
        if( m_gen ){
            cout<<endl << "THE PARENTS OF GENERATION <" << m_gen << "> ARE:"; 
            printParents();
        }
        cout << "RUN\n";
        evaluate();
        kill_off();

        // Save
        if( save ){
            cout << "SAVING\n";
            for(int i=0; i< num_parents; i++)
            {
                string save_file = save_path + "/" + prefix + "_Agent_"+brains[i]->get_id()+".brain";
                cout << save_file << endl;
                brains[i]->save(save_file);
            }
        }

        m_gen++;


        cout << endl;
    }
}

void GA:: populate()
{
    if( cur_pop < num_parents )
    {
        for(; cur_pop<max_pop; cur_pop++)
        {
            brains.push_back(
                    new Agent(
                        m_gen,
                        ((use_names) ? names_f.front() : string("Agent#")),
                        ((use_names) ? names_l.front() : string(to_string(agent_counter))),
                        agent_counter,
                        nodes_per_layer, 
                        __DUMMY_EMPTY_VECTOR,
                        .01
                    ));
            cout << "   ->A new Agent moved in! Say \"Hi\" to "+brains.back()->get_name()<<endl;
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
        if( mommy != daddy )
        {
            cout << "   ->"+brains[mommy]->name_f+" "<<brains[mommy]->fitness[0]<<" and "+
                brains[daddy]->name_f+" "<<brains[daddy]->fitness[0]<<" had a child, "<<endl;
            brains.push_back(
                new Agent(
                    m_gen,
                    ((use_names) ? names_f.front() : "Agent#"+to_string(agent_counter)),
                    ((use_names) ? brains[mommy]->name_l : ""),
                    ((use_names) ? brains[daddy]->name_l : ""),
                    agent_counter, 
                    *brains.at(mommy),
                    *brains.at(daddy),
                    10+rand() % 40 // 1/50 chance at random mutation
                ));
        }
        else
        {
            //if( rand()%2 == 1 )
                brains.push_back(
                        new Agent(
                            m_gen,
                            ((use_names) ? names_f.front() : string("Agent#")),
                            ((use_names) ? names_l.front() : string(to_string(agent_counter))),
                            agent_counter,
                            nodes_per_layer, 
                            __DUMMY_EMPTY_VECTOR,
                            .01
                        ));
                cout << "   ->A new Agent moved in! Say \"Hi\" to "+brains.back()->get_name()<<endl;
            if(use_names)
                names_l.pop_front();
        }
        if(use_names)
        {
            names_f.pop_front();
            //names_l.pop_front();
        }
        agent_counter++;
    }
}
void GA:: evaluate()
{
    for(int i = m_gen?num_parents:0; i<max_pop; i++)
    {
        cout<< "  >We're on Mario #" << i << endl;

        int cur(-1);
        string got;

        if(brains[i]->born != m_gen)
        {
            cout<< "    Fitness was <"<<brains[i]->fitness[0];
            cout << m_gen - brains[i]->born << "] Generations old.";
        }
        cout << "\tCode name: " << brains[i]->get_name()<< " - ID: " << brains[i]->get_id() << endl;
        while(1)
        {
            fstream in(in_file, ios_base::in);
            getline(in, got); // Get first line, which is iteration number.
            if (got[0] == 'D' && !just_dead)
            {
                just_dead = 1;
                cout << "\tFitness: <";
                for(int fit_count = 0; getline(in, got); fit_count++)
                {
                    if( fit_count !=0)
                        cout << "|";
                    brains[i]->fitness[fit_count] = atof(got.c_str());/* fitness at end */
                    cout << brains[i]->fitness[fit_count] ;
                }
                cout << ">\n";
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
                vector<bool> binary_out; 
                brains[i]->brain.output_as_binary( values, binary_out ); 

                fstream out(out_file, ios_base::out);
                
                bool none = true;
                if (binary_out[0] == 1)
                {
                    out << "A ";
                    none = false;
                }
                if (binary_out[1] == 1)
                {
                    out << "B ";
                    none = false;
                }
                if (binary_out[2] == 1)
                {
                    out << "left ";
                    none = false;
                }
                if (binary_out[3] == 1)
                {
                    out << "right ";
                    none = false;
                }
                if (binary_out[4] == 1)
                {
                    out << "up ";
                    none = false;
                }
                if (binary_out[5] == 1)
                {
                    out << "down ";
                    none = false;
                }
                if (binary_out[6] == 1)
                {
                    out << "select ";
                    none = false;
                }
                if (binary_out[7] == 1 && false)
                {
                    out << "start ";
                    none = false;
                }
                if ( none )
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
        bool push_last = true;
        for(int i=0; i<cur_pop-1; i++)
        {
            if( brains[i]->name_l == brains[cur_pop-1]->name_l){
                push_last = false;
                break;
            }
        }
        if( push_last ){
            names_l.push_back(brains[cur_pop-1]->name_l);
        }
        names_f.push_back(brains[cur_pop-1]->name_f);

        delete brains[cur_pop-1];
        brains.erase(brains.end()-1);
    }

}

void GA:: printBrains()
{
    cout << endl;
    for (unsigned int i=0; i<brains.size(); i++)
    {
        cout << "\t" << brains[i]->get_name() << " has a fitness of " <<  brains[i]->fitness[0] << endl;
    }
}

void GA:: printParents()
{
    cout << endl;

    for (int i=0; i<num_parents; i++)
    {
        cout << "\t"<< brains[i]->get_name() << " <"<<brains[i]->fitness[0]<<"|"<<brains[i]->fitness[1]<<"> - Age: " << m_gen-brains[i]->born<< "\n" ;
    }
    cout << endl;
}

bool comp(Agent* i, Agent* j)
{ 
    for(int fitness_count= 0; fitness_count<1; fitness_count++){
        if( i->fitness[fitness_count] == j->fitness[fitness_count]){
            if( i->fitness[fitness_count+1] != j->fitness[fitness_count+1]){
                return i->fitness[fitness_count+1] > j->fitness[fitness_count+1]; 
            }
        }
        else{
            return i->fitness[fitness_count] > j->fitness[fitness_count]; 
        }
    }
}
