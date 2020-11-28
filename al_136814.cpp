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
    int index, processingTime, readinessTime;
    double endTime, criterion;
};

struct Machine {
    double factor;
    vector<Task> tasks;
    double criterion;
    double time;

    void update() {
        this->time = 0;
        this->criterion = 0;
        for (int i = 0; i < this->tasks.size() - 1; ++i) {
            if (this->time < this->tasks[i].readinessTime) this->time = this->tasks[i].readinessTime;

            this->time += this->tasks[i].processingTime * this->factor;
            this->tasks[i].endTime = this->time;

            this->criterion += this->time - this->tasks[i].readinessTime;
            this->tasks[i].criterion = this->criterion;
        }
    }

    bool testSchedule(Task task, int index) {
        double currentTime = index == 0 ? 0.0 : this->tasks[index - 1].endTime;
        Task original = this->tasks[index];
        if (currentTime < task.readinessTime) currentTime = task.readinessTime;
        double end = currentTime + task.processingTime;
        double criterion = end - task.readinessTime;
        return end < original.endTime && criterion < original.criterion;
    }
};

//GLOBALS
vector<Task> tasks;
vector<Machine> machines;

double round(double value) {
    double result = (int)(value * 100 + .5); 
    return (double)result / 100; 
}

void readFiles(string inputPath) {

    ifstream inFile(inputPath);

    int n, id = 1;
    Task task;

    inFile >> n;
    inFile >> machines[0].factor >> machines[1].factor >> machines[2].factor >> machines[3].factor >> machines[4].factor;
    while (
        inFile 
        >> task.processingTime
        >> task.readinessTime) {

        task.index = id;       
        task.endTime = 0;
        task.criterion = 0; 
        tasks.push_back(task);
        id++;
    }

    inFile.close();
}

void insertTask(Task task, int machineIndex) {
    if (machines[machineIndex].tasks.size() == 0) {
        machines[machineIndex].tasks.push_back(task);
        return;
    }

    for (int i = machines[machineIndex].tasks.size() - 1; i >= 0; --i) {
        Task taskToSwap = machines[machineIndex].tasks[i];
        if (i > 0) {
            Task prevTask = machines[machineIndex].tasks[i - 1];
            
            double time = task.readinessTime > prevTask.endTime
                ? task.readinessTime + task.processingTime * machines[machineIndex].factor
                : prevTask.endTime + task.processingTime * machines[machineIndex].factor;
            double criterion = prevTask.criterion + time - task.readinessTime;

            if ((time < taskToSwap.endTime && criterion < taskToSwap.criterion) || criterion < taskToSwap.criterion) {
                machines[machineIndex].tasks.insert(machines[machineIndex].tasks.begin() + i, task);
                return;
            }
        } else {
            double time = task.readinessTime + task.processingTime * machines[machineIndex].factor;
            double criterion = task.processingTime * machines[machineIndex].factor;

            if ((time < taskToSwap.endTime && criterion < taskToSwap.criterion) || criterion < taskToSwap.criterion) {
                machines[machineIndex].tasks.insert(machines[machineIndex].tasks.begin() + i, task);
                return;
            }
        }
        
        machines[machineIndex].tasks.push_back(task);
        return;
    }
} 

int getMinMachineIndex(Task task) {
    // vector<Machine> tmpMachines = machines;
    // vector<double> criterionsBack;
    // vector<double> criterions;
    
    // for (int i = 0; i < tmpMachines.size(); ++i) {
    //     tmpMachines[i].tasks.push_back(task);
    //     criterionsBack.push_back(tmpMachines[i].update());
    //     criterions.push_back(insertTask(task, tmpMachines[i]));
    // }
    auto minCriterionElement = min_element( machines.begin(), machines.end(),
    [](const Machine &a, const Machine &b) {
        return a.criterion < b.criterion;
    });
    // auto min = min_element(criterionsBack.begin(), criterionsBack.end());
    return distance(machines.begin(), minCriterionElement);
}

void schedule() {
    for (auto task : tasks) {
        int index = getMinMachineIndex(task);
        // cout << "MIN MACHINE: " << machines[index].factor << endl;
        machines[index].tasks.push_back(task);
        // insertTask(task, index);
        machines[index].update();
        sort(machines[index].tasks.begin(), machines[index].tasks.end(), [](const Task& lhs, const Task& rhs) {
            return lhs.readinessTime == rhs.readinessTime ? lhs.criterion < rhs.criterion : lhs.readinessTime < rhs.readinessTime;
        });
    }
}

double getResult() {
    double result = 0;

    for (Machine machine : machines) {
        result += machine.criterion;
    }

    result = round(result / double(tasks.size()));

    // cout << "RESULT: " << result << endl << endl;
    return result;
}

void printResult(int instanceSize, double result) {
    cout << endl;
    for (auto machine: machines) {
        cout << endl << "MACHINE: "<< machine.factor << endl;
        for (auto task : machine.tasks) {
            cout << "(" << task.index << ", " << task.criterion << ")" << " ";
        }
        cout << endl << "CRITERION: " << machine.criterion << endl
        << "TIME: " << machine.time << endl << endl;
    }

    cout << "RESULT: " << result << endl << endl;
}

void generateOut(string outputPath, double result) {
    ofstream outGenFile(outputPath);
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

    string index;

    string inputPath = argv[1];
    string outputPath = inputPath;

    int pathnameLastIndex = inputPath.find_last_of("/\\");
    outputPath.replace(pathnameLastIndex + 1, 2, "out");

    readFiles(inputPath);

    schedule();

    // printResult(i, result);

    generateOut(outputPath, getResult());
    cin >> index;
    return 0;
}
