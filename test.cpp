#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    vector<int> nodes(10);
    iota(nodes.begin(), nodes.end(), 0);
    random_shuffle(nodes.begin(), nodes.end());
    for(auto i:nodes){
        cout<<i<<" ";
    }cout<<"\n";
    return 0;
}