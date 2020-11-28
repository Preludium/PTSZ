#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
using namespace std;

int main(int argc, char *argv[]) {
    string base(".txt"), outFileName("out_");
    string index;

    if (argc < 2) {
        cout << "Enter index: ";
        cin >> index;
    } else {
        index = argv[1];
    }

    for (int i = 50; i <= 500; i += 50) {
        ofstream outFile(outFileName + index + "_" + to_string(i) + base);
        outFile << 0 << endl;
        outFile << 1;
        for (int j = 2; j <= i; ++j) {
            outFile << " " << j; 
        }
        outFile.close();
    }
    return 0;
}