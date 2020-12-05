#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
using namespace std;

string INPUT_PREFIX("\\in_"), OUTPUT_PREFIX("\\out_"), BASE(".txt"), PATH(".\\instances\\in_");

struct Task {
    int deadlineTime, weight;
    vector<int> processingDurations;
};

struct TaskMachine{};

struct Result {
    double criterion;
    vector<int> jobs;
};

vector<Task> tasks;
Result result;

void readInstance(string index, int size) {
    ifstream instanceFile(PATH + index + INPUT_PREFIX + index + "_" + to_string(size) + BASE);
    int n, job1, job2, job3;
    Task task = Task();
    vector<int> processingTimes;

    instanceFile >> n;
    while(instanceFile >>
        job1 >>
        job2 >>
        job3 >>
        task.deadlineTime >>
        task.weight) {
            task.processingDurations.clear();
            task.processingDurations.insert(task.processingDurations.end(), { job1, job2, job3 });
            tasks.push_back(task);
    }
    // for (Task task : tasks) {
    //     cout << task.processingDurations[0] << " " << task.processingDurations[1] << " " << task.processingDurations[2] << " " << task.deadlineTime << " " << task.weight << endl;
    // }
}

void readResult(string index, int size) {
    ifstream resultFile(PATH + index + OUTPUT_PREFIX + index + "_" + to_string(size) + BASE);
    int job;
    resultFile >> result.criterion;
    while(resultFile >> job) result.jobs.push_back(job);
    resultFile.close();
}

int validateResult() {
    int time = 0, criterion = 0;
    int end1stTime = 0, end2ndTime = 0, end3rdTime = 0;
    int sumWeights = 0;
    for (int index : result.jobs) {
        time = end1stTime;
        time += tasks[index - 1].processingDurations[0];
        end1stTime = time;
        
        if (end2ndTime > time) time = end2ndTime;
        time += tasks[index - 1].processingDurations[1];
        end2ndTime = time;

        if (end3rdTime > time) time = end3rdTime;
        time += tasks[index - 1].processingDurations[2];
        end3rdTime = time;
        
        sumWeights += tasks[index - 1].weight;
        criterion += tasks[index - 1].weight * max(0, end3rdTime - tasks[index - 1].deadlineTime);
    }
    return sumWeights * criterion / sumWeights;
}

void printSummary(int size, double criterion) {
    cout << "Instance: " << size << endl
        << "Received criterion: " << result.criterion << endl
        << "Calculated criterion: " << criterion << endl << endl; 
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
        result.jobs.clear();
        tasks.clear();
        readInstance(index, i);
        readResult(index, i);
        int criterion = validateResult();
        printSummary(i, criterion);
    }    

    return 0;
}