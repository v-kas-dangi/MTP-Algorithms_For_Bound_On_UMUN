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

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int n, m, q;
    cin>>n>>m>>q;
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++){
        int u, v;
        cin>>u>>v;
        adj[u].push_back(v);
    }
    

    return 0;
}