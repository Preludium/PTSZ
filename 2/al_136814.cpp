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
int medTaskDuration;

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

        tasks.push_back(task);
        id++;
    }


    inFile.close();
}

void calculateMedian() {
    // avgTaskDuration = accumulate (tasks.begin(), tasks.end(), 0, 
    //     [](int i, const Task& o){ return o.processingTime + i; }) / double(tasks.size());

    vector<Task> tasksCopy = tasks;
    sort(tasksCopy.begin(), tasksCopy.end(), [](const Task& lhs, const Task& rhs) {
        return lhs.processingTime < rhs.processingTime;
    });

    medTaskDuration = tasksCopy[tasksCopy.size() / 2 - 1].processingTime;

    // cout << "MEDIAN: " << medTaskDuration << endl << "MEAN: " << avgTaskDuration << endl << endl;
}

bool selectMachine(Machine a, Machine b, Task task) {
    double aTime = a.time, aCriterion = a.criterion, bTime = b.time, bCriterion = b.criterion;

    if (task.readinessTime > a.time) aTime = task.readinessTime;
    if (task.readinessTime > b.time) bTime = task.readinessTime;

    aTime += task.processingTime * a.factor;
    bTime += task.processingTime * b.factor;

    aCriterion += aTime - task.readinessTime;
    bCriterion += bTime - task.readinessTime;

    if (task.processingTime >= medTaskDuration - 2) {
        return (aCriterion < bCriterion)
            && (aTime < bTime);
    } else {
        return (aCriterion < bCriterion)
            || (aTime < bTime);
    }
}

int getMinMachineIndex(Task task) {
    auto minCriterionElement = min_element( machines.begin(), machines.end(),
        [&task](const Machine &a, const Machine &b) {
            return selectMachine(a, b, task);
        });
    return distance(machines.begin(), minCriterionElement);
}

void schedule() {
    calculateMedian();

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

double getResult() {
    double result = 0;

    for (Machine machine : machines) {
        result += machine.criterion;
    }

    result = round(result / double(tasks.size()));

    // cout << "RESULT: " << result << endl << endl;
    return result;
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

    string inputPath = argv[1];
    string outputPath = inputPath;

    int pathnameLastIndex = inputPath.find_last_of("/\\");
    outputPath.replace(pathnameLastIndex + 1, 2, "out");
    
    machines.clear();
    tasks.clear();
    machines.resize(5);

    readFiles(inputPath);

    schedule();

    generateOut(outputPath, getResult());
    return 0;
}
