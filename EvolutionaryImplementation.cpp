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
vector<int> parent, rank_set;

int find_set(int v) {
    if (parent[v]==-1)
        return v;
    return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank_set[a] < rank_set[b])
            swap(a, b);
        parent[b] = a;
        rank_set[a] += rank_set[b];
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

void make partitions(){
    for(auto it:sessions){
        if(!checkEdgeExists(it.first, it.second)){
            //add to partition
            int first_parent = find_set(it.first);
            int second_parent = find_set(it.second);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    input();
    parent.resize(n,-1);
    rank_set.resize(n,1);
    vector<int> encode(n+1);
    for(int u=0; u<n; u++){
        cout<<u<<": ";
        for(auto &v:adj[u]){
            cout<<v<<" ";
        }
        cout<<endl;
    }
    // generateArrays(n, 0, encode, result);
    // int finalAns=0;
    // vector<int> ansPartition;
    // for(auto partition:result){
    //     if(checkIndependence(partition)){
    //         if(finalAns<checkSessions(partition)){
    //             finalAns=checkSessions(partition);
    //             ansPartition=partition;
    //         }
    //     }
    // }
    // cout<<finalAns<<endl;
    // for(auto i:ansPartition){
    //     cout<<i<<" ";
    // }
    // cout<<endl;
    // return 0;
}
