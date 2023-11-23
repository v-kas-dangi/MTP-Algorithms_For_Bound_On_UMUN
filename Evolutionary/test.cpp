#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

int random_number(){
    srand(static_cast<unsigned>(time(0)));
    return rand();
}

// Example functions using rand()
void functionA() {
    for (int i = 0; i < 5; ++i) {
        std::cout << rand() << " ";
    }
    std::cout << std::endl;
}

void functionB() {
    for (int i = 0; i < 5; ++i) {
        std::cout << rand() << " ";
    }
    std::cout << std::endl;
}
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
    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    
    // Call functions that use rand()
    functionA();
    functionB();
    return 0;
}

