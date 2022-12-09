#include <cstdlib>
#include <iostream>
#include <climits>
#include <ctime>
#include <random>
#include <vector>
#include <cassert>
#include "RedBlackTree.h"

using namespace std;

int main() {

// create a simulated 3.7 million ID number system.
int N = 3700000;
clock_t start = clock();
RedBlackTree rbt = RedBlackTree();
for (int i = 0; i < N; i++){
    rbt.Insert(i);
}
clock_t stop = clock();
double duration = (static_cast<double>(stop - start)) / CLOCKS_PER_SEC;
cout << "Collected " << rbt.Size() << " ID numbers in " << (duration) << " seconds." << endl;
// Takes between 1.5 and 1.6 seconds to fully insert

vector<int> jaded_people = {rand() % 3700000, rand() % 3700000, rand() % 3700000, rand() % 3700000, rand() % 3700000};

clock_t start2 = clock();
for (int i = 1; i <= 5; i++) {
    rbt.Insert(3700000+(rand()%1000000));
}
clock_t stop2 = clock();
duration = (static_cast<double>(stop2 - start2)) / CLOCKS_PER_SEC;
// Takes an insignificant amount of time to add 5 ID numbers (less than 0.001 seconds)
cout << "Added 5 ID numbers in " << (duration) << " seconds." << endl;


clock_t start3 = clock();
for (int i : jaded_people) {
    rbt.Remove(i);
}
clock_t stop3 = clock();
duration = (static_cast<double>(stop3 - start3)) / CLOCKS_PER_SEC;

// Takes an insignificant amount of time to remove 5 ID numbers (less than 0.001 seconds)
cout << "Removed 5 ID numbers in " << (duration) << " seconds." << endl;

return 0;
}