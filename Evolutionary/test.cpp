#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
int main() {
    vector<int> cards{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Seed the random number generator
    random_device rd;
    mt19937 g(rd());

    // Shuffle the vector using shuffle
    shuffle(cards.begin(), cards.end(), g);

    // Display the shuffled vector
    for (int card : cards) {
        cout << card << " ";
    }
    shuffle(cards.begin(), cards.end(), g);
    cout<<"\n";
    // Display the shuffled vector
    for (int card : cards) {
        cout << card << " ";
    }
    shuffle(cards.begin(), cards.end(), g);
    cout<<"\n";
    // Display the shuffled vector
    for (int card : cards) {
        cout << card << " ";
    }

    return 0;
}
