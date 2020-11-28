#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <numeric>
using namespace std;


struct Task {
    int index, processingTime, readinessTime;
};

struct Machine {
    double factor;
    vector<Task> tasks;
    double criterion;
    double time;

    void update() {
        if (tasks.size() == 1) {
            this->time = this->tasks[0].readinessTime + this->tasks[0].processingTime * this->factor;
            this->criterion = this->time - this->tasks[0].readinessTime;
        } else {
            if (this->tasks.back().readinessTime > this->time) this->time = this->tasks.back().readinessTime;
            this->time += this->tasks.back().processingTime * this->factor;
            this->criterion += this->time - this->tasks.back().readinessTime;
        }
    }
};

//GLOBALS
vector<Task> tasks;
vector<Machine> machines;

double round(double value) {
    double result = (int)(value * 100 + .5); 
    return (double)result / 100; 
}

void readFiles(string index, int i) {
    string base(".txt"), inFileName("in_");
    ifstream inFile(".\\instances\\in_" + index + "\\" + inFileName + index + "_" + to_string(i) + base);

    int n, id = 1;
    Task task;

    inFile >> n;
    inFile >> machines[0].factor >> machines[1].factor >> machines[2].factor >> machines[3].factor >> machines[4].factor;
    while (
        inFile 
        >> task.processingTime
        >> task.readinessTime) {

        task.index = id;       
        tasks.push_back(task);
        id++;
    }

    inFile.close();
}

bool selectMachine(Machine a, Machine b, Task task) {
    double aTime = a.time, aCriterion = a.criterion, bTime = b.time, bCriterion = b.criterion;

    if (task.readinessTime > a.time) aTime = task.readinessTime;
    if (task.readinessTime > b.time) bTime = task.readinessTime;

    aTime += task.processingTime * a.factor;
    bTime += task.processingTime * b.factor;

    aCriterion += aTime - task.readinessTime;
    bCriterion += bTime - task.readinessTime;

    return (aCriterion < bCriterion && aTime < bTime)
        || (aCriterion < bCriterion)
        || (aTime < bTime);
}

int getMinMachineIndex(Task task) {
    auto minCriterionElement = min_element( machines.begin(), machines.end(),
        [&task](const Machine &a, const Machine &b) {
            return selectMachine(a, b, task);
        });
    return distance(machines.begin(), minCriterionElement);
}

void schedule() {
    // Sort tasks by processsing time and readiness
    sort(tasks.begin(), tasks.end(), [](const Task& lhs, const Task& rhs) {
        return lhs.readinessTime == rhs.readinessTime
            ? lhs.processingTime < rhs.processingTime
            : lhs.readinessTime < rhs.readinessTime;
    });

    // Schedule
    for (auto task : tasks) {
        int index = getMinMachineIndex(task);
        machines[index].tasks.push_back(task);
        machines[index].update();
    }
}

double getResult(int instanceSize) {
    double result = 0;

    for (Machine machine : machines) {
        result += machine.criterion;
    }

    result = round(result / double(instanceSize));

    // cout << "RESULT: " << result << endl << endl;
    return result;
}

void printResult(int instanceSize, double result) {
    cout << endl;
    for (auto machine: machines) {
        cout << endl << "MACHINE: "<< machine.factor << endl;
        for (auto task : machine.tasks) {
            cout << task.index << " ";
        }
        cout << endl << "CRITERION: " << machine.criterion << endl
        << "TIME: " << machine.time << endl << endl;
    }

    cout << "RESULT: " << result << endl << endl;
}

void generateOut(string index, int i, double result) {
    string base(".txt"), outFileName("out_");
    ofstream outGenFile(".\\instances\\in_" + index + "\\"+ outFileName + index + "_" + to_string(i) + base);
    outGenFile << result << endl;
    for (auto machine: machines) {
        for (auto task : machine.tasks) {
            outGenFile << task.index << " ";
        }
        outGenFile << endl;
    }
    outGenFile.close();
}

int main(int argc, char *argv[]) {
    // string index;
    string indexes[] = {"136774", "136785", "136812", "136815", "132336", "136803", "132639", "136814", "136807", "136798", "132337", "132321", "136808", "136691"};
    ofstream allResults("all.txt");
    // if (argc < 2) {
    //     cout << "Enter index: ";
    //     cin >> index;
    // } else {
    //     index = argv[1];
    // }
    for (string index : indexes) {
        allResults << endl << endl << index << endl;
        for (int i = 50; i <= 500; i += 50) {
            machines.clear();
            tasks.clear();
            machines.resize(5);

            readFiles(index, i);

            schedule();

            double result = getResult(i);
            
            allResults << result << endl;
            // printResult(i, result);

            generateOut(index, i, result);

        }
    }
    allResults.close();
    return 0;
}
