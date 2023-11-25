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
int POPULATION_SIZE=20;
int MAX_GENERATIONS=100;
int ELITISM_COUNT=5;
int CROSSOVER_PROBABILITY=0.7;
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
    adj.resize(n+1);
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
    vector<int> dsu_par(n+1, -1);
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
    vector<int> individual(n+1);
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
    vector<pair<int, vector<int>>> sortedPopulation;
    set<pair<int, vector<int>>> sortedPop;
    for (auto &individual : currentPopulation) {
        sortedPopulation.push_back({fitness(individual), individual});
        sortedPop.insert({fitness(individual), individual});
    }
    sort(sortedPopulation.begin(), sortedPopulation.end(), greater<>());
    // for(auto individual : sortedPop){
    //     cout<<individual.first<<": ";
    //     printVector(individual.second);
    //     cout<<endl;
    // }
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

// Function to perform one-point crossover between two parents
vector<int> crossover(vector<int>& parent1,  vector<int>& parent2) {
    // Ensure the parents are of the same size
    if (parent1.size() != parent2.size()) {
        cerr << "Error: Parents have different sizes for crossover." << endl;
        exit(1); // Or handle the error in an appropriate way
    }

    // Choose a random crossover point
    int crossoverPoint = rand() % (parent1.size() - 1) + 1; // Ensure crossover point is not at the beginning

    // Create a child by combining genetic material from both parents
    vector<int> child(parent1.begin(), parent1.begin() + crossoverPoint);
    child.insert(child.end(), parent2.begin() + crossoverPoint, parent2.end());

    return child;
}

// Function to perform one-point mutation in an individual
void mutate(vector<int>& individual) {
    // Choose a random gene to mutate
    int mutationPoint = rand() % individual.size();

    // Mutate the selected gene (you can customize the mutation logic)
    individual[mutationPoint] = rand() % 100; // Replace with your mutation logic
}

// Function to evolve the population through crossover and mutation
void evolvePopulation(vector<vector<int>> &currentPopulation, vector<vector<int>> &nextPopulation,
                      int elitismCount, double crossoverProbability, double mutationProbability) {
    // Perform elitism
    elitism(currentPopulation, nextPopulation, elitismCount);

    // Perform crossover and mutation
    int crossoverStartIndex = min(elitismCount, (int)nextPopulation.size());
    while (crossoverStartIndex < nextPopulation.size()) {
        // Select parents (you can use tournament selection, roulette wheel, etc.)
        vector<int> parent1 = currentPopulation[rand() % currentPopulation.size()];
        vector<int> parent2 = currentPopulation[rand() % currentPopulation.size()];

        // Perform crossover with a certain probability
        if (rand() / static_cast<double>(RAND_MAX) < crossoverProbability) {
            // Apply crossover
            vector<int> child = crossover(parent1, parent2);

            // Perform mutation with a certain probability
            if (rand() / static_cast<double>(RAND_MAX) < mutationProbability) {
                mutate(child);
            }

            nextPopulation[crossoverStartIndex++] = child;
        }
    }
}



void repair_individual(vector<int> &individual){

}

void repair_population(){

}

void genetic_algorithm(){
    vector<vector<int>> currentPopulation=generate_population();
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        // Evolve the population
        vector<vector<int>> nextPopulation;
        evolvePopulation(currentPopulation, nextPopulation, ELITISM_COUNT, CROSSOVER_PROBABILITY, MUTATION_PROBABILITY);
        // Replace the current population with the next generation
        currentPopulation = nextPopulation;
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    srand(static_cast<unsigned int>(time(0)));
    input();
    vector<vector<int>> population = generate_population();
    // n=population.size();
    for(auto individual : population){
        printVector(individual);
    }
    vector<vector<int>> nextPopulation;
    elitism(population, nextPopulation, ELITISM_COUNT);
    // evolvePopulation(population, nextPopulation, ELITISM_COUNT, CROSSOVER_PROBABILITY, MUTATION_PROBABILITY);
    for(auto individual : nextPopulation){
        for(auto node : individual){
            cout<<node<<" ";
        }
        cout<<endl;
    }

    return 0;   
}
