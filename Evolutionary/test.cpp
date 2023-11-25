#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

void assignLowestAvailableNumber(vector<int>& arr) {
    unordered_map<int, int> availableNumbers;
    
    for (int& num : arr) {
        // If num is not found in the map, assign the lowest available number
        if (availableNumbers.find(num) == availableNumbers.end()) {
            availableNumbers[num] = num;
        } else {
            // Update num to the lowest available number and increment the available number
            num = availableNumbers[num];
            availableNumbers[num]++;
        }
    }
    for(auto i:availableNumbers){
        cout<<i.first<<" "<<i.second<<"\n";
    }
}

int main() {
    vector<int> arr = {1, 2, 2, 5, 5, 6, 5, 5};

    // Call the function to assign the lowest available number
    assignLowestAvailableNumber(arr);

    // Print the modified vector
    // for (int num : arr) {
    //     cout << num << " ";
    // }

    return 0;
}
