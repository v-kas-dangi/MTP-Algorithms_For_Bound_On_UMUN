#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
    input format:
        First line contains the integer n, m and q- the number of vertices and edges of the graph and the number of sessions.
        The next m lines: In each line we have two integers u and v representing an undirected edge between u and v.
        The next q lines: For every ith line we have two integers si and ti, representing the source and sink associated with the ith session.
        Constraints:
            0<=u, v<n
            For 0<=si, ti<n
*/

int n,m,q;
vector<pair<int,int>> edges, sessions;
vector<vector<int>> result, adj, adjMatrix;
int POPULATION_COUNT=20;

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
    for(auto i: dsu_par){
        if(i<0){
            set<int> nodes_in_set = retrieve_partition(i, dsu_par);
            for (int node : nodes_in_set) {
                individual[node] = partition_id;
            }
            partition_id++;
        }
    }
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
    for(int i=0; i<POPULATION_COUNT; i++){
        shuffle(sessions.begin(), sessions.end(), g);
        population.push_back(generate_individual());
    }
    return population;
}

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

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    input();

    return 0;   
}
