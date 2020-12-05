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
};

double round(double value) {
    double result = (int)(value * 100 + .5); 
    return (double)result / 100; 
}

int main(int argc, char *argv[]) {
    string base(".txt"), inFileName("in_"), outFileName("out_"), index;
    string indexes[] = {"136814"};//{"136774", "136785", "136812", "136815", "132336", "136803", "132639", "136814", "136807", "136798", "132337", "132321", "136808", "136691"};
    // if (argc < 2) {
    //     cout << "Enter index: ";
    //     cin >> index;
    // } else {
    //     index = argv[1];
    // }
for (string index : indexes) {
    cout <<endl<<endl<< index << endl;
    for (int i = 50; i <= 500; i += 50) {
        // Generator
        // ofstream outGenFile(".\\instances\\in_" + index + "\\"+ outFileName + index + "_" + to_string(i) + base);
        // outGenFile << 0 << endl;
        // outGenFile << 1;
        // int counter = 1, machine = 1;

        // for (int j = 2; j <= i; ++j) {
        //     if (machine == 5 || counter < i / 5) {
        //         outGenFile << " " << j; 
        //         counter++;
        //     } else {
        //         outGenFile << endl << j;
        //         counter = 1;
        //         machine++;
        //     }
        // }
        // outGenFile.close();

        // Validator

        ifstream outFile(".\\instances\\in_" + index + "\\" + outFileName + index + "_" + to_string(i) + base),
                 inFile(".\\instances\\in_" + index + "\\" + inFileName + index + "_" + to_string(i) + base);

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


        double receivedCriterion;
        string tmpLine, tmpCrit;
        int machineCounter = 0;
        getline(outFile, tmpCrit);
        receivedCriterion = stod(tmpCrit);
        while (getline(outFile, tmpLine)) {
            stringstream line(tmpLine);
            while (getline(line, token, ' ')) {
                machines[machineCounter].tasks.push_back(stoi(token));
            }
            machineCounter++;
        }

        double resultCriterion = 0, localCriterion, time;
        for (auto machine: machines) {
            // cout << "MASZYNA: " << machine.factor << endl << endl;
            time = 0;
            localCriterion = 0;
            for (auto task: machine.tasks) {
                // cout << "TASK: (" + to_string(tasks[task - 1].processingTime) + ", " + to_string(tasks[task - 1].readinessTime) + ")" << endl;
                if (tasks[task - 1].readinessTime > time) time = tasks[task - 1].readinessTime;

                time += tasks[task - 1].processingTime * machine.factor;
                localCriterion += time - tasks[task - 1].readinessTime;
            }
            resultCriterion += localCriterion;
        }

        resultCriterion /= double(i);
        resultCriterion = round(resultCriterion);

        if (resultCriterion != receivedCriterion) {
            cout << i << ": (" << resultCriterion << ", " << receivedCriterion << ")"<<endl;
        }
        cout << "Received criterion: " << receivedCriterion << ", calculated criterion: " << round(resultCriterion) << endl;

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
}
    return 0;
}

// 132337 blad nie ma nic po przecinku