#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
// Function to generate a random graph with independent partitions
void generateGraph(int N, int Q, int num_partitions, double edgeProbability, double sessionProbability, string inputFileName, string outputFileName) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribution(0, 1);
    srand(static_cast<unsigned int>(time(0)));
    // Randomly assign partitions to nodes
    vector<int> nodePartitions(N);
    map<int, int> partition_count;
    for (int i = 0; i < N; ++i) {
        int randNum=rand()%num_partitions;
        nodePartitions[i] = randNum;
        partition_count[randNum]++;
    }
    ofstream partitionDetailsFile("PartitionDetails.txt");
    partitionDetailsFile << "Number of partition formed: "<< partition_count.size()<<"\n";
    for(auto i:nodePartitions){
        partitionDetailsFile<<i<<" ";
    }
        partitionDetailsFile<<endl;
    partitionDetailsFile.close();
    vector<vector<bool>> graph(N, vector<bool>(N, false));
    int M=0;
    // Connect nodes across partitions with probability edgeProbability
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            int probability = distribution(gen);
            if (probability < edgeProbability && nodePartitions[i] != nodePartitions[j]) {
                if(!graph[i][j]){
                    graph[i][j] = graph[j][i] = true;
                    M++;
                }
            }
        }
    }
    // Create sessions and sinks for each partition
    vector<pair<int, int>> sessions;
    vector<int> nodes(N);
    iota(nodes.begin(), nodes.end(), 0);
    while(sessions.size()<Q){
        // Randomly shuffle partitionNodes
        random_shuffle(nodes.begin(), nodes.end());
        int probability = distribution(gen);
        if (probability < sessionProbability){
            int idx1=rand()%nodes.size();
            int idx2=rand()%nodes.size();
            if(idx1!=idx2)
                sessions.push_back({nodes[idx1], nodes[idx2]});
        }
    }

    // Shuffle sessions
    random_shuffle(sessions.begin(), sessions.end());

    // Create input file
    ofstream inputFile(inputFileName);
    inputFile << N << " " << M << " " << sessions.size() << endl;

    // Write edges to the input file
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (graph[i][j]) {
                inputFile << i << " " << j << endl;
            }
        }
    }
    // Write sessions to the input file
    for (const auto& session : sessions) {
        inputFile << session.first << " " << session.second << endl;
    }
    inputFile.close();

    // Create output file
    ofstream outputFile(outputFileName);
    outputFile << sessions.size() << endl;
    outputFile.close();
}

int main() {
    int N, Q, num_partitions;
    double edgeProbability, sessionProbability;
    cout << "Enter the number of nodes (N): ";
    cin >> N;
    cout << "Enter the number of sessions (Q): ";
    cin >> Q;
    cout << "Enter the number of independent sets (num_partitions): ";
    cin >> num_partitions;
    cout << "Enter the probability of having an edge between two nodes of different independent sets: ";
    cin >> edgeProbability;
    cout << "Enter the probability of having session between two nodes of a same independent set: ";
    cin >> sessionProbability;

    string inputFileName = "input.txt";
    string outputFileName = "output.txt";

    generateGraph(N, Q, num_partitions, edgeProbability, sessionProbability, inputFileName, outputFileName);

    cout << "Test case generated successfully!" << endl;

    return 0;
}
