#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
    int p, r, d, w, max_range;
    string base(".txt"), outFileName("in_136814_");
    srand(time(NULL));
    
    if (argc == 1) {
        cout << "Enter max range: ";
        cin >> max_range;
    } else {
        max_range = atoi(argv[1]);
    }

    for (int i = 50; i < 501; i += 50) {
        ofstream outFile(outFileName + to_string(i) + base);
        outFile << i << endl;
        for (int j = 0; j < i; ++j) {
            p = rand() % max_range + 1;
            r = rand() % i + 1;
            d = rand() % max_range + p + r + 1;
            w = rand() % max_range + 1;
            outFile << p << " " << r << " " << d << " " << w << endl;
        }    
        outFile.close();
    }

    return 0;
}