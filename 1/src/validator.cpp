#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
using namespace std;

class Task {
    public:
    int processingDuration, readinessTime, deadlineTime, weight;
    
    Task () {}

    int getCriterion(int taskFinishTime) {
        if (taskFinishTime <= this->deadlineTime) {
            return 0;
        } else {
            return this->weight;
        }
    }

    bool testInstance() {
        return this->processingDuration > 0 && this->readinessTime > 0 && this->deadlineTime > 0 && this->weight > 0;
    }

    void toString() {
        cout << this->processingDuration << " " << this->readinessTime << " " << this->deadlineTime << " " << this->weight << endl;
    }
};

class Result {
    public:
    int criterion;
    vector<int> jobs;

    Result() {}

    void toString() {
        cout << this->criterion << endl;
        for (auto job : this->jobs) cout << job << endl;
    }
};

void generateResults(string index) {
    string base(".txt"), outFileName("out_");

    for (int i = 50; i <= 500; i += 50) {
        ofstream outFile(outFileName + index + "_" + to_string(i) + base);
        outFile << 0 << endl;
        outFile << 1;
        for (int j = 2; j <= i; ++j) {
            outFile << " " << j; 
        }
        outFile.close();
    }
}

void validateResult(string index, int instanceSize) {
    string inputFilePrefix("in_"), outputFilePrefix("out_"), base(".txt");
    int n;
    ifstream instanceFile(inputFilePrefix + index + "_" + to_string(instanceSize) + base),
        resultFile(outputFilePrefix + index + "_" + to_string(instanceSize) + base);
    vector<Task> tasks;
    Task task = Task();
    
    // read instance
    instanceFile >> n;
    while (
        instanceFile 
        >> task.processingDuration
        >> task.readinessTime
        >> task.deadlineTime
        >> task.weight)
        tasks.push_back(task);

    // read result
    Result result = Result();
    resultFile >> result.criterion;
    int job;
    while (resultFile >> job) result.jobs.push_back(job);

    //test result
    int criterion = 0;
    int processedTime = 0;
    for (int job : result.jobs) {
        if (processedTime < tasks.at(job - 1).readinessTime)
            processedTime += tasks.at(job - 1).readinessTime - processedTime;

        processedTime += tasks.at(job - 1).processingDuration;
        criterion += tasks.at(job -1).getCriterion(processedTime);
    }

    cout << "Instance: " << instanceSize << endl
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

    // generateResults(index);

    for (int i = 50; i <= 500; i += 50) {
        validateResult(index, i);
    }    

    return 0;
}