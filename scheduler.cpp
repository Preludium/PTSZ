#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
using namespace std;

class Task {
    public:
    int processingDuration, readinessTime, deadlineTime, weight, index;
    
    Task () {}

    int getCriterion(int taskFinishTime) {
        if (taskFinishTime <= this->deadlineTime) {
            return 0;
        } else {
            return this->weight;
        }
    }

    void toString() {
        cout << this->processingDuration << " " << this->readinessTime << " " << this->deadlineTime << " " << this->weight << endl;
    }
};

void sortByDeadline(vector<Task> &tasks) {
    sort(tasks.begin(), tasks.end(), [](const Task& lhs, const Task& rhs) {
        return lhs.deadlineTime == rhs.deadlineTime ? lhs.weight > rhs.weight : lhs.deadlineTime < rhs.deadlineTime;
    });
}

void swap(vector<Task> &tasks, int indexA, int indexB) {
    iter_swap(tasks.begin() + indexA, tasks.begin() + indexB);
}

void scheduleInstance(string index, int instanceSize) {
    string inputFilePrefix("in_"), outputFilePrefix("out_"), base(".txt");
    ifstream instanceFile(inputFilePrefix + index + "_" + to_string(instanceSize) + base);
    ofstream outputFile(outputFilePrefix + index + "_" + to_string(instanceSize) + base);
    int n;
    
    vector<Task> tasks, tasksCopy;
    Task task = Task();
    
    // read instance
    instanceFile >> n;
    int counter = 1;
    while (
        instanceFile 
        >> task.processingDuration
        >> task.readinessTime
        >> task.deadlineTime
        >> task.weight) {
            task.index = counter;
            tasks.push_back(task);
            counter++;
        }

    sortByDeadline(tasks);

    int processedTime = 0, criterionBeforeBigBrainAlgorithm = 0;
    for (auto task: tasks) {
        if (processedTime < tasks.at(task.index - 1).readinessTime)
            processedTime += tasks.at(task.index - 1).readinessTime - processedTime;

        processedTime += tasks.at(task.index - 1).processingDuration;
        criterionBeforeBigBrainAlgorithm += tasks.at(task.index - 1).getCriterion(processedTime);
    }

    int currentTime = 0,
        resultTime = tasks.at(0).deadlineTime,
        resultCriterion = tasks.at(0).getCriterion(tasks.at(0).processingDuration),
        currentCriterion,
        testCriterion;

    tasksCopy = tasks;
    for (int i = 1; i < n; ++i) {
        currentCriterion = 0;
        currentTime = 0;

        for (int j = 0; j < i; ++j) {
            if (tasksCopy.at(i).readinessTime > currentTime) { // jesli i-ty nie jest jeszcze gotowy to nie ma swapa i lecimy dali wuja 
                currentTime += tasksCopy.at(j).processingDuration;
                currentCriterion += tasksCopy.at(j).getCriterion(currentTime);
                continue;
            }
            testCriterion = currentCriterion;

            currentCriterion += tasksCopy.at(j).getCriterion(currentTime + tasksCopy.at(j).processingDuration); // kryterium przed swapem
            swap(tasksCopy, j, i);
            
            currentTime += tasksCopy.at(j).processingDuration;
            testCriterion += tasksCopy.at(j).getCriterion(currentTime);
            if (currentCriterion >= testCriterion) { // jesli poprawione zostalo kryterium
                currentCriterion = testCriterion;
                continue;
            } else {    // jak jednak dupa to trzeba odczarowac aktualny czas
                currentTime -= tasksCopy.at(j).processingDuration;
                currentTime += tasksCopy.at(i).processingDuration;
                swap(tasksCopy, j, i);
            }
        }
        tasks = tasksCopy;
        resultTime = currentTime;
        resultCriterion = currentCriterion;
    }

    if (criterionBeforeBigBrainAlgorithm < resultCriterion) { // zabezpieczenie przed chujowym algorytmem
        cout << "#Zjebales => before: " << criterionBeforeBigBrainAlgorithm << ", after: " << resultCriterion << endl;
    }

    outputFile << currentCriterion << endl;
    for (auto task: tasks) {
        outputFile << task.index << " ";
        // cout << task.index << endl;
    }
}
    

int main(int argc, char *argv[]) {
    string index;

    if (argc < 2) {
        cout << "Enter index: ";
        cin >> index;
    } else {
        index = argv[1];
    }

    for (int i = 50; i <= 500; i += 50) {
        scheduleInstance(index, i);
    }    

    return 0;
}