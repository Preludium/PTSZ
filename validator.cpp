#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
using namespace std;

struct Task {
    int processingTime, readinessTime;
};

struct Machine {
    double factor;
    vector<int> tasks;

    // Machine(double factor) {
    //     this->factor = factor;
    // }
};

int main(int argc, char *argv[]) {
    string base(".txt"), inFileName("in_"), outFileName("out_"), index;

    if (argc < 2) {
        cout << "Enter index: ";
        cin >> index;
    } else {
        index = argv[1];
    }

    for (int i = 50; i <= 500; i += 50) {
        ifstream outFile(outFileName + index + "_" + to_string(i) + base),
                 inFile(inFileName + index + "_" + to_string(i) + base);

        int n;
        string token;
        Task task;
        vector<Task> tasks;
        vector<Machine> machines;
        machines.resize(5);

        inFile >> n;
        inFile >> machines[0].factor >> machines[1].factor >> machines[2].factor >> machines[3].factor >> machines[4].factor;
        while (
            inFile 
            >> task.processingTime
            >> task.readinessTime)
        tasks.push_back(task);


        int receivedCriterion;
        string tmpLine, tmpCrit;
        int machineCounter = 0;
        getline(outFile, tmpCrit);
        receivedCriterion = stoi(tmpCrit);
        while (getline(outFile, tmpLine)) {
            stringstream line(tmpLine);
            while (getline(line, token, ' ')) {
                machines[machineCounter].tasks.push_back(stoi(token));
            }
            machineCounter++;
        }

        int resultCriterion = 0, localCriterion, time;
        for (auto machine: machines) {
            time = 0;
            localCriterion = 0;
            for (auto task: machine.tasks) {
                if (tasks[task - 1].readinessTime > time) time = tasks[task - 1].readinessTime;

                time += tasks[task - 1].processingTime;
                localCriterion += time - tasks[task - 1].readinessTime;
            }
            resultCriterion += localCriterion * machine.factor;
        }

        resultCriterion /= double(i);

        cout << "Received criterion: " << receivedCriterion << ", calculated criterion: " << resultCriterion << endl;

        // cout << "tasks" << endl;
        // for(auto task : tasks) {
        //     cout << "(" << task.processingTime << ", " << task.readinessTime << ") ";
        // }

        // cout << endl << endl << "Machines" << endl;
        // for (auto machine: machines) {
        //     cout << machine.factor << endl;
        //     for (auto task : machine.tasks) {
        //         cout << task << " ";
        //     }
        //     cout << endl;
        // }


        inFile.close();
        outFile.close();
    }
    return 0;
}