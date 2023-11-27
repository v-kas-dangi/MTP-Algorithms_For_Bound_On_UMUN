#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
    input format:
        First line contains the integer n, m and q- the number of vertices and edges of the graph and the number of sessions.
        The next m lines: In each line we have two integers u and v representing an undirected edge between u and v.
        The next q lines: For every ith line we have two integers si and ti, representing the source and sink associated with the ith session.
        raints:
            0<=u, v<n
            For 0<=si, ti<n
*/

int n,m,q;
vector<pair<int,int>> edges, sessions;
vector<vector<int>> result, adj, adjMatrix;
int POPULATION_SIZE=100;
int MAX_GENERATIONS=100;
int ELITISM_COUNT=10;
int CROSSOVER_PROBABILITY=1.0;
int MUTATION_PROBABILITY=0.05;

int find_set(int v, vector<int> &dsu_par) 
{
  if (dsu_par[v]<0)
    return v;
  return dsu_par[v]=find_set(dsu_par[v], dsu_par);
}

void union_sets(int a, int b, vector<int> &dsu_par)
{
  a = find_set(a, dsu_par);
  b = find_set(b, dsu_par);
  if (a != b)
  {
    if(dsu_par[a]>dsu_par[b])
      swap(a, b);
    dsu_par[a]+=dsu_par[b];
    dsu_par[b] = a;
  }
}

void input(){
    cin >> n >> m >> q;
    edges.resize(m);
    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        edges[i] = make_pair(u, v);
    }
    sessions.resize(q);
    set<int> session_nodes;
    for(int i = 0; i < q; i++){
        int si, ti;
        cin >> si >> ti;
        sessions[i] = make_pair(si, ti);
        session_nodes.insert(si);
        session_nodes.insert(ti);
    }
    adj.resize(n);
    adjMatrix.resize(n,vector<int>(n,0));
    for(auto i:edges){
        if(session_nodes.count(i.first)&&session_nodes.count(i.second)){
            adjMatrix[i.first][i.second]=1;
            adj[i.first].push_back(i.second);
            adj[i.second].push_back(i.first);
        }
    }
}

bool checkEdgeExists(int source, int sink){
    if(adjMatrix[source][sink])
    return true;
    return false;
}

// given a node function will return a vector of nodes in the same partition.
set<int> retrieve_partition(int node, vector<int> &dsu_par){
    int root = find_set(node, dsu_par);
    set<int> nodes_in_set;
    for (int i = 0; i < dsu_par.size(); ++i) {
        if (find_set(i, dsu_par) == root) {
            nodes_in_set.insert(i);
        }
    }
    return nodes_in_set;
}
bool can_merge(set<int> &partition1, set<int> &partition2){
    for(auto u:partition1){
        for(auto v:adj[u]){
            if(partition2.find(v)!=partition2.end()){
                return false;
            }
        }
    }
    return true;
}
void encoding_individual(vector<int> &dsu_par, vector<int> &individual){
    int partition_id=0;
    for(int i=0; i<dsu_par.size(); i++){
        if(dsu_par[i]<0){
            set<int> nodes_in_set = retrieve_partition(i, dsu_par);
            for (int node : nodes_in_set) {
                individual[node] = partition_id;
            }
            partition_id++;
        }
    }
}
void printVector(vector<int> &arr){
    for(auto i:arr){
        cout<<i<<" ";
    }
        cout<<endl;
}
vector<int> generate_individual(){
    vector<int> dsu_par(n, -1);
    for(auto session:sessions){
        int s=session.first;
        int t=session.second;
        if(find_set(s, dsu_par)!=find_set(t, dsu_par)){
            set<int> partition1=retrieve_partition(s, dsu_par);
            set<int> partition2=retrieve_partition(t, dsu_par);
            if(can_merge(partition1, partition2)){
                union_sets(s, t, dsu_par);
            }
        }
    }
    vector<int> individual(n);
    encoding_individual(dsu_par, individual);
    return individual;
}

vector<vector<int>> generate_population(){
    random_device rd;
    mt19937 g(rd());
    vector<vector<int>> population;
    for(int i=0; i<POPULATION_SIZE; i++){
        shuffle(sessions.begin(), sessions.end(), g);
        population.push_back(generate_individual());
    }
    return population;
}

// fitness function
int fitness(vector<int> &individual){
    int ans=0;
    for(auto i:sessions){
        int u=i.first, v=i.second;
        if(individual[u]==individual[v]){
            ans++;
        }
    }
    return ans;
}

// Function to perform elitism by copying the best individuals to the next generation
void elitism(vector<vector<int>>& currentPopulation, vector<vector<int>>& nextPopulation, int elitismCount) {
    // Sort the current population by fitness
    set<pair<int, vector<int>>> sortedPop;
    for (auto &individual : currentPopulation) {
        sortedPop.insert({fitness(individual), individual});
    }
    int index=0;
    set<pair<int, vector<int>>>::reverse_iterator rit;
    for(rit=sortedPop.rbegin(); rit != sortedPop.rend(); rit++){
        if(index<elitismCount){
            pair<int, vector<int>> ritto = *rit;
            nextPopulation.push_back(ritto.second);
            index++;
        }
    }
}

bool shouldCrossoverMutate(int prob) {
    random_device rd;
    mt19937 gen(rd());

    // Generate a random probability between 0 and 100
    uniform_int_distribution<> distribution(0, 1);
    int probability = distribution(gen);

    // Check if the generated probability is greater than 80%
    return probability < prob;
}

// Function to perform one-point crossover between two parents
vector<int> crossover(vector<int>& parent1,  vector<int>& parent2) {
    // Ensure the parents are of the same size
    random_device rd;
    mt19937 gen(rd());
    // Choose a random index in the first vector
    uniform_int_distribution<> distribution(0, parent1.size() - 1);
    int index = distribution(gen);
    vector<int> result(parent1.begin(), parent1.begin() + index);
    // Add elements from vector B (i to end)
    result.insert(result.end(), parent2.begin() + index, parent2.end());
    return result;
}

// Function to perform one-point mutation in an individual
void mutate(vector<int>& individual) {
    // Choose two random genes to mutate
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribution(0, individual.size() - 1);
    int indexA = distribution(gen);
    int indexB = distribution(gen);
    swap(individual[indexA], individual[indexB]);
}

vector<pair<int, int>> get_defs(vector<int> &individual){
    set<pair<int, int>> defectives;
    for(int u=0; u<n; u++){
        for(auto v:adj[u]){
            if(individual[u]==individual[v]){
                defectives.insert({u, v});
            }
        }
    }
    vector<pair<int, int>> defs;
    for(auto it:defectives){
        defs.push_back(it);
    }
    return defs;
}

int reassign_partition_number(vector<int> &individual){
    map<int, vector<int>> partition_groups;
    for (int i = 0; i < individual.size(); i++)
        partition_groups[individual[i]].push_back(i);
    int partition_id=0;
    for(auto i:partition_groups){
        for(auto it:i.second){
            individual[it]=partition_id;
        }
        partition_id++;
    }
    return partition_id;
}

int can_assign(vector<int> &individual, int u){
    vector<int> valid_destinations;
    map<int, int> taken;
    for(int i=0; i<n; i++){
        if(!checkEdgeExists(u, i) && individual[u]!=individual[i]){
            if(taken.find(individual[i])!=taken.end()){
                valid_destinations.push_back(individual[i]);
                taken[individual[i]]=1;
            }
        }
    }
    int num_valids=valid_destinations.size();
    if(num_valids>0)
        return valid_destinations[rand()%num_valids];
    else return -1;
}

void repair_individual(vector<int> &individual){
    int partition_id=reassign_partition_number(individual);
    vector<pair<int, int>> defs=get_defs(individual);
    random_device rd;
    mt19937 g(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);
    mt19937 gen(rd());
    double randValue = dis(gen);
    if (randValue < 0.5) {
        shuffle(defs.begin(), defs.end(), g);
    }
    for(auto it:defs){
        int u=it.first;
        int v=it.second;
        if(individual[u]==individual[v]){
            int partition=can_assign(individual, u);
            if(partition!=-1) individual[u]=partition;
            else{
                int partition=can_assign(individual, v);
                if(partition!=-1) individual[v]=partition;
                else{
                    randValue=dis(gen);
                    if(randValue<0.5){
                        individual[u]=partition_id;
                    }else{
                        individual[v]=partition_id;
                    }
                    partition_id++;
                }
            }
        }
    }
}

// Function to evolve the population through crossover and mutation
void evolvePopulation(vector<vector<int>> &currentPopulation, vector<vector<int>> &nextPopulation,
                      int elitismCount, double crossoverProbability, double mutationProbability) {
    // Perform elitism
    elitism(currentPopulation, nextPopulation, elitismCount);
    // Perform crossover and mutation
    int crossoverStartIndex = min(elitismCount, (int)nextPopulation.size());
    while (crossoverStartIndex < POPULATION_SIZE) {
        // Select parents randomly from population
        vector<int> parent1 = currentPopulation[rand() % currentPopulation.size()];
        vector<int> parent2 = currentPopulation[rand() % currentPopulation.size()];
        if (shouldCrossoverMutate(CROSSOVER_PROBABILITY)) {
            // Apply crossover
            vector<int> individual = crossover(parent1, parent2);
            // Perform mutation with a certain probability
            if (shouldCrossoverMutate(MUTATION_PROBABILITY)) {
                mutate(individual);
            }
            repair_individual(individual);
            nextPopulation.push_back(individual);
            crossoverStartIndex++;
        }
    }
}

void genetic_algorithm(){
    vector<vector<int>> currentPopulation=generate_population();
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        // Evolve the population
        vector<vector<int>> nextPopulation;
        evolvePopulation(currentPopulation, nextPopulation, ELITISM_COUNT, CROSSOVER_PROBABILITY, MUTATION_PROBABILITY);
        // Replace the current population with the next generation
        currentPopulation.clear();
        currentPopulation = nextPopulation;
        cout<<"Fitness for generation "<<generation<<" : "<<fitness(currentPopulation[0])<<endl;
    }
    cout<<"Final Fitness: "<<fitness(currentPopulation[0])<<endl;
    cout<<"Final Partition: "<<endl;
    for(auto i:currentPopulation[0]){
        cout<<i<<" ";
    }cout<<endl;
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    srand(static_cast<unsigned int>(time(0)));
    input();
    genetic_algorithm();

    return 0;     
}
