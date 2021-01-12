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

string INPUT_PREFIX("\\in_"), OUTPUT_PREFIX("\\out_"), BASE(".txt"), PATH(".\\instances\\in_");

struct Task {
    int deadlineTime, weight, id;
    vector<int> processingDurations;
};

struct Machine {
    int time1, time2, time3;
};

vector<Task> tasks;
Machine machine;

double round(double value) {
    double result = (int)(value * 100 + .5); 
    return (double)result / 100; 
}

void readInstance(string path) {
    ifstream instanceFile(path);
    int n, job1, job2, job3, counter = 1;
    Task task = Task();
    vector<int> processingTimes;

    instanceFile >> n;
    while(instanceFile >>
        job1 >>
        job2 >>
        job3 >>
        task.deadlineTime >>
        task.weight) {
            task.id = counter;
            task.processingDurations.clear();
            task.processingDurations.insert(task.processingDurations.end(), { job1, job2, job3 });
            tasks.push_back(task);
            counter++;
    }
}

void swap(int i, int j) {
    iter_swap(tasks.begin() + i, tasks.begin() + j);  
}

bool checkCriterion(int min, int j) {
    int timeMin = machine.time1, timeJ = machine.time1;
    int gapJ = 0, gapMin = 0;

    timeMin += tasks[min].processingDurations[0];
    timeJ += tasks[j].processingDurations[0];
    
    if (machine.time2 > timeMin) gapMin += machine.time2 - timeMin;
    if (machine.time2 > timeJ) gapJ += machine.time2 - timeJ;

    timeMin = machine.time2 + tasks[min].processingDurations[1];
    timeJ = machine.time2 + tasks[j].processingDurations[1];

    if (machine.time3 > timeMin) gapMin += machine.time3 - timeMin;
    if (machine.time3 > timeJ) gapJ += machine.time3 - timeJ;

    timeMin = machine.time3 + tasks[min].processingDurations[2];
    timeJ = machine.time3 + tasks[j].processingDurations[2];
    
    return (float)(tasks[j].deadlineTime + gapJ) / (float)tasks[j].weight
                                            <
        (float)(tasks[min].deadlineTime + gapMin) / (float)tasks[min].weight;
}

void sort() {  
    int min_idx;
  
    for (int i = 0; i < tasks.size()-1; i++) {
        min_idx = i;  
        for (int j = i+1; j < tasks.size(); j++) {
            if (checkCriterion(min_idx, j)) {
                min_idx = j;
            }  
        }
        swap(i, min_idx);
        machine.time1 += tasks[i].processingDurations[0];
        machine.time2 = machine.time1 + tasks[i].processingDurations[1];
        machine.time3 = machine.time2 + tasks[i].processingDurations[2];
    }  
}

void schedule() {
    sort();
}

void generateOutput(string path, double criterion) {
    ofstream outFile(path);
    outFile << criterion << endl;
    outFile << tasks[0].id;
    for (int j = 1; j < tasks.size(); ++j) {
        outFile << " " << tasks[j].id; 
    }
    outFile.close();
}

double validateResult() {
    int time = 0, criterion = 0;
    int end1stTime = 0, end2ndTime = 0, end3rdTime = 0;
    int sumWeights = 0;
    for (Task task : tasks) {
        time = end1stTime;
        time += task.processingDurations[0];
        end1stTime = time;
        
        if (end2ndTime > time) time = end2ndTime;
        time += task.processingDurations[1];
        end2ndTime = time;

        if (end3rdTime > time) time = end3rdTime;
        time += task.processingDurations[2];
        end3rdTime = time;
        
        sumWeights += task.weight;
        criterion += task.weight * max(0, end3rdTime - task.deadlineTime);
    }
    return round(criterion / double(sumWeights));
}

int main(int argc, char *argv[]) {
    string inputPath = argv[1];
    string outputPath = inputPath;

    int pathnameLastIndex = inputPath.find_last_of("/\\");
    outputPath.replace(pathnameLastIndex + 1, 2, "out");

    tasks.clear();
    machine.time1 = 0;
    machine.time2 = 0;
    machine.time3 = 0;

    readInstance(inputPath);
    schedule();
    double criterion = validateResult();
    generateOutput(outputPath, criterion);
    return 0;
}