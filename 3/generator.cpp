#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;

const int MAX_WEIGHT_RANGE = 15; 
const int MAX_TASK_RANGE = 5; 

int main(int argc, char *argv[]) {
    int d, w;
    vector<int> p;
    string base(".txt"), outFileName("in_136814_");
    srand(time(NULL));
    
    for (int i = 50; i <= 500; i += 50) {
        ofstream outFile(outFileName + to_string(i) + base);
        outFile << i << endl;
        for (int j = 0; j < i; ++j) {
            p.clear();
            for (int k = 0; k < 3; ++k) {
                p.push_back(rand() % MAX_TASK_RANGE + 1);
            }
            int sumP = accumulate(p.begin(), p.end(), 0);

            d = sumP * (rand() % 3 + 2);
            w = rand() % MAX_WEIGHT_RANGE + 1;
            outFile << p[0] << " " << p[1] << " " << p[2] << " " << d << " " << w << endl;
        }    
        outFile.close();
    }

    return 0;
}