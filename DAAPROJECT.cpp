#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int weight;
    int profit;
    float ratio;  // For Greedy algorithm, store profit-to-weight ratio
};

// Function to calculate the ratio of profit/weight for the Greedy approach
void calculateRatio(vector<Item>& items) {
    for (int i = 0; i < items.size(); i++) {
        items[i].ratio = (float)items[i].profit / items[i].weight;
    }
}

// 0/1 Knapsack (Dynamic Programming)
int knapsack(int capacity, vector<Item>& items, vector<int>& selectedItems) {
    int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (items[i - 1].weight <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - items[i - 1].weight] + items[i - 1].profit);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int totalProfit = dp[n][capacity];
    int w = capacity;
    for (int i = n; i > 0 && totalProfit > 0; i--) {
        if (totalProfit != dp[i - 1][w]) {
            selectedItems.push_back(i - 1);
            w -= items[i - 1].weight;
            totalProfit -= items[i - 1].profit;
        }
    }

    return dp[n][capacity];
}

// Calculate the upper bound for the Branch and Bound approach
float calculateBound(int level, int profit, int weight, int W, vector<Item>& items) {
    if (weight >= W) return 0;

    float profitBound = profit;
    int j = level + 1;
    int totalWeight = weight;

    while (j < items.size() && totalWeight + items[j].weight <= W) {
        totalWeight += items[j].weight;
        profitBound += items[j].profit;
        j++;
    }

    if (j < items.size()) {
        profitBound += (W - totalWeight) * items[j].profit / (float)items[j].weight;
    }

    return profitBound;
}

// Branch and Bound for 0/1 Knapsack Problem
void branchAndBound(int W, vector<Item>& items, vector<int>& selectedItems) {
    int n = items.size();
    vector<bool> isSelected(n, false);
    vector<int> Q;

    int maxProfit = 0;
    int totalProfit = 0;
    int totalWeight = 0;

    Q.push_back(0);  // Push the initial level into the queue

    while (!Q.empty()) {
        int level = Q.back();
        Q.pop_back();

        if (level >= n) continue;

        int profit = totalProfit + items[level].profit;
        int weight = totalWeight + items[level].weight;

        if (weight <= W && profit > maxProfit) {
            maxProfit = profit;
            isSelected[level] = true;
            selectedItems.push_back(level);
        }

        // Calculate bound for the node
        float bound = calculateBound(level, profit, weight, W, items);
        if (bound > maxProfit) {
            Q.push_back(level + 1);
        }
    }

    cout << "Total Profit Value: " << maxProfit << endl;
    cout << "Items selected (index, weight, profit):" << endl;
    for (int i = 0; i < selectedItems.size(); i++) {
        int itemIndex = selectedItems[i];
        cout << "Item " << itemIndex + 1 << ": "
             << "Weight = " << items[itemIndex].weight << ", "
             << "Profit = " << items[itemIndex].profit << endl;
    }
}

// Greedy Knapsack Algorithm
int greedyKnapsack(int capacity, vector<Item>& items, vector<int>& selectedItems) {
    int n = items.size();
    calculateRatio(items);

    sort(items.begin(), items.end(), [](Item& a, Item& b) {
        return a.ratio > b.ratio;
    });

    int totalProfit = 0;
    int totalWeight = 0;
    for (int i = 0; i < n; i++) {
        if (totalWeight + items[i].weight <= capacity) {
            totalProfit += items[i].profit;
            totalWeight += items[i].weight;
            selectedItems.push_back(i);
        }
    }

    return totalProfit;
}

int main() {
    int n, capacity;

    // Ask the user for the number of items
    cout << "Enter the number of items: ";
    cin >> n;

    vector<Item> items(n);

    // Ask the user for the weight and profit of each item
    for (int i = 0; i < n; i++) {
        cout << "Enter weight and profit for item " << i + 1 << ": ";
        cin >> items[i].weight >> items[i].profit;
    }

    // Ask the user for the capacity of the knapsack
    cout << "Enter the capacity of the knapsack: ";
    cin >> capacity;

    vector<int> selectedItems;

    int choice;
    cout << "Choose Algorithm (1 = 0/1 Knapsack, 2 = Branch and Bound, 3 = Greedy): ";
    cin >> choice;

    if (choice == 1) {
        int totalProfit = knapsack(capacity, items, selectedItems);
        cout << "Total Profit Value: " << totalProfit << endl;
        cout << "Items selected (index, weight, profit):" << endl;
        for (int i = 0; i < selectedItems.size(); i++) {
            int itemIndex = selectedItems[i];
            cout << "Item " << itemIndex + 1 << ": "
                 << "Weight = " << items[itemIndex].weight << ", "
                 << "Profit = " << items[itemIndex].profit << endl;
        }
    } else if (choice == 2) {
        branchAndBound(capacity, items, selectedItems);
    } else if (choice == 3) {
        int totalProfit = greedyKnapsack(capacity, items, selectedItems);
        cout << "Total Profit Value: " << totalProfit << endl;
        cout << "Items selected (index, weight, profit):" << endl;
        for (int i = 0; i < selectedItems.size(); i++) {
            int itemIndex = selectedItems[i];
            cout << "Item " << itemIndex + 1 << ": "
                 << "Weight = " << items[itemIndex].weight << ", "
                 << "Profit = " << items[itemIndex].profit << endl;
        }
    } else {
        cout << "Invalid choice!" << endl;
    }

    cout << "Number of items selected: " << selectedItems.size() << endl;
    return 0;
}
