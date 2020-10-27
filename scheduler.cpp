#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
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
    clock_t start, end;
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

    int resultCriterion = 0, resultTime = 0;
    start = clock();
    for (int i = 0; i < n; ++i) {
        int currentTime = resultTime,
            currentCriterion = resultCriterion;

        for (int j = i + 1; j < n; ++j) {
            Task rootTask = tasks[i],
                currentTask = tasks[j];

            if (currentTask.readinessTime > currentTime) {
                continue;
            }

            // before swap
            int beforeSwapTime = currentTime;
            int beforeSwapCriterion = currentCriterion;
            if (rootTask.readinessTime > beforeSwapTime) {
                beforeSwapTime = rootTask.readinessTime; 
            }
            beforeSwapTime += rootTask.processingDuration;
            beforeSwapCriterion += rootTask.getCriterion(beforeSwapTime);

            if (currentTask.readinessTime > beforeSwapTime) {
                beforeSwapTime = currentTask.readinessTime; 
            }
            beforeSwapTime += currentTask.processingDuration;
            beforeSwapCriterion += currentTask.getCriterion(beforeSwapTime);

            // after swap
            int afterSwapTime = currentTime;
            int afterSwapCriterion = currentCriterion;
            if (currentTask.readinessTime > afterSwapTime) {
                afterSwapTime = currentTask.readinessTime; 
            }
            afterSwapTime += currentTask.processingDuration;
            afterSwapCriterion += currentTask.getCriterion(afterSwapTime);

            if (rootTask.readinessTime > afterSwapTime) {
                afterSwapTime = rootTask.readinessTime; 
            }
            afterSwapTime += rootTask.processingDuration;
            afterSwapCriterion += rootTask.getCriterion(afterSwapTime);

            if (afterSwapCriterion < beforeSwapCriterion) {
                swap(tasks, i, j);
            }
        }

        if (tasks[i].readinessTime > resultTime) {
            resultTime = tasks[i].readinessTime; 
        }
        resultTime += tasks[i].processingDuration;
        resultCriterion += tasks[i].getCriterion(resultTime);
    }
    end = clock();
    cout << index << " " << instanceSize << ": " << resultCriterion << " | " << double(end - start) << " ms" << endl;

    outputFile << resultCriterion << endl;
    for (auto task: tasks) {
        outputFile << task.index << " ";
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
