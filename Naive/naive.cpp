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
vector<vector<int>> result, adj;

void input(){
    cin >> n >> m >> q;
    edges.resize(m);
    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        edges[i] = make_pair(u, v);
    }
    sessions.resize(q);
    for(int i = 0; i < q; i++){
        int si, ti;
        cin >> si >> ti;
        sessions[i] = make_pair(si, ti);
    }
    adj.resize(n+1);
    for(auto i:edges){
        adj[i.first].push_back(i.second);
        adj[i.second].push_back(i.first);
    }
}

void generateArrays(int n, int currentIndex, vector<int>& currentArray, vector<vector<int>>& result) {
    if (currentIndex == n) {
        result.push_back(currentArray);
        return;
    }
    for (int i = 1; i <= n; ++i) {
        currentArray[currentIndex] = i;
        generateArrays(n, currentIndex + 1, currentArray, result);
    }
}

bool checkIndependence(vector<int> &partition) {
    for(int u = 0; u < n; ++u){
        for(auto &v : adj[u]){
            if(partition[u]==partition[v]){
                return false;
            }
        }
    }
    return true;
}

int checkSessions(vector<int> &partition){
    int ans=0;
    for(auto i:sessions){
        int u=i.first, v=i.second;
        if(partition[u]==partition[v]){
            ans++;
        }
    }
    return ans;
}


int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    input();
    vector<int> encode(n+1);
    generateArrays(n, 0, encode, result);
    int finalAns=0;
    for(auto partition:result){
        if(checkIndependence(partition)){
            finalAns=max(finalAns,checkSessions(partition));
        }
    }
    cout<<finalAns<<endl;
    return 0;
}