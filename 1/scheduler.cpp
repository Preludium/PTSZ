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

    Task() {}

    int getCriterion(int taskFinishTime) {
        if (taskFinishTime <= this->deadlineTime) {
            return 0;
        }
        else {
            return this->weight;
        }
    }

    void toString() {
        cout << this->processingDuration << " " << this->readinessTime << " " << this->deadlineTime << " " << this->weight << endl;
    }
};

void sort(vector<Task>& tasks) {
    sort(tasks.begin(), tasks.end(), [](const Task& lhs, const Task& rhs) {
        return lhs.weight == rhs.weight ? lhs.readinessTime < rhs.readinessTime : lhs.weight > rhs.weight;
        });
}

void swap(vector<Task>& tasks, int indexA, int indexB) {
    iter_swap(tasks.begin() + indexA, tasks.begin() + indexB);
}

void scheduleInstance(string inputPath, string outputPath) {
    // string inputFilePrefix("in_"), outputFilePrefix("out_"), base(".txt"), path(".\\instances\\in_" + index + "\\");
    ifstream instanceFile(inputPath);
    ofstream outputFile(outputPath);
    // clock_t start, end;
    int n;

    vector<Task> tasks, tasksCopy;
    Task task = Task();

    // start = clock();
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

    sort(tasks);

    int resultCriterion = 0, resultTime = 0;
    for (int i = 0; i < n; ++i) {
        int currentTime = resultTime,
            currentCriterion = resultCriterion;

        for (int j = i + 1; j < n; ++j) {
            Task rootTask = tasks[i],
                currentTask = tasks[j];

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

            if (afterSwapCriterion < beforeSwapCriterion || (afterSwapCriterion == beforeSwapCriterion && afterSwapTime < beforeSwapTime)) {
                swap(tasks, i, j);
            }
        }

        if (tasks[i].readinessTime > resultTime) {
            resultTime = tasks[i].readinessTime;
        }
        resultTime += tasks[i].processingDuration;
        resultCriterion += tasks[i].getCriterion(resultTime);
    }

    outputFile << resultCriterion << endl;
    for (auto task : tasks) {
        outputFile << task.index << " ";
    }
}


int main(int argc, char* argv[]) {
    string index;

    string inputPath = argv[1];
    string outputPath = inputPath;

    int pathnameLastIndex = inputPath.find_last_of("/\\");
    outputPath.replace(pathnameLastIndex + 1, 2, "out");

    scheduleInstance(inputPath, outputPath);
    return 0;
}