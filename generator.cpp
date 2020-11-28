#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

int MAX_NUM = 15;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    string base(".txt"), inFileName("in_"), index;

    if (argc < 2) {
        cout << "Enter index: ";
        cin >> index;
    } else {
        index = argv[1];
    }

    vector<double> machines;
    machines.push_back(1;

    for (int i = 50; i <= 500; i += 50) {
        ofstream outFile(inFileName + index + "_" + to_string(i) + base);
        outFile << i << endl;

        for (int j = 1; j < 5; ++j) {
            machines.push_back((rand() % 399 + 101) / 100.0);
        }

        random_shuffle(machines.begin(), machines.end());

        outFile << machines[0];
        for (int j = 1; j < 5; ++j) {
            outFile << " " << machines[j];
        }

        outFile << endl;

        for (int j = 0; j < i; j++) {
            outFile << rand() % MAX_NUM + 1 << " " << rand() % (i + 1) << endl;
        }

        outFile.close();
    }
    return 0;
}