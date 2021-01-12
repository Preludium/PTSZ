#include "settings.h"

using namespace std;

class TimeGuard {
    clock_t timeMemory;
public:
    TimeGuard() {
        this->timeMemory = clock();
    }

    double stop() {
        return double(clock() - this->timeMemory);
    }
};

vector<Task> readInstance(ifstream &infile, int instanceSize) {
    vector<Task> instance;

    for (int i = 0; i < instanceSize; ++i) {
        Task task;
        task.number = i;
        infile >> task.duration[0] >> task.duration[1] >> task.duration[2] >> task.deadline >> task.weight;
        instance.push_back(task);
    }

    return instance;
}

vector<int> readSchedule(ifstream &infile, int size) {
    vector<int> schedule;

    for (int i = 0; i < size; ++i) {
        int task;
        infile >> task;
        schedule.push_back(task);
    }

    return schedule;
}

void generateSchedules(vector<int> &times, string index) {
    for (int i = 0; i < INSTANCES; ++i) {
        int instanceSize = INSTANCE_SIZES[i];
        string instancePath = ".\\instances\\in_136814\\out_136814_" + to_string(instanceSize) + ".txt";
        string inputCmd = ".\\al_" + index + ".exe .\\\\instances\\\\in_136814\\\\in_136814_" + to_string(INSTANCE_SIZES[i]) + ".txt";

        remove(instancePath.c_str());

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        DWORD exit_code;

        TimeGuard guard = TimeGuard();

        CreateProcess(NULL, (LPSTR) inputCmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exit_code);

        times.push_back(guard.stop());
    }
}

void validate(string index) {
    vector<int> times;

    cout << index << endl;
    generateSchedules(times, index);

    for (int i = 0; i < INSTANCES; ++i) {
        int instanceSize = INSTANCE_SIZES[i];

        string instancePath = ".\\instances\\in_136814\\in_136814_" + to_string(instanceSize) + ".txt";
        string schedulePath = ".\\instances\\in_136814\\out_136814_" + to_string(instanceSize) + ".txt";

        ifstream instanceFile(instancePath);
        if (!instanceFile.good()) {
            cout << "No instace file found" << endl;
            return;
        }

        int size;

        instanceFile >> size;
        vector<Task> instance = readInstance(instanceFile, size);
        instanceFile.close();

        ifstream scheduleFile(schedulePath);
        if (!scheduleFile.good()) {
            cout << "No schedule file found" << endl;
            return;
        }

        float presumedCriterion;

        scheduleFile >> presumedCriterion;
        vector<int> schedule = readSchedule(scheduleFile, size);
        scheduleFile.close();

        if (schedule.size() != size) {
            cout << "Not all tasks got scheduled" << endl;
            return;
        }

        float criterion = 0, weightSum = 0;
        vector<Machine> machines(MACHINES);

        for (int i = 0; i < size; ++i) {
            int taskNumber = schedule[i] - 1;
            
            for (int m = 0; m < MACHINES; ++m) {
                if (m > 0) {
                    machines[m].time = max(machines[m].time, machines[m - 1].time);
                }
                machines[m].time += instance[taskNumber].duration[m];
            }

            criterion += instance[taskNumber].weight * max(0, machines[MACHINES - 1].time - instance[taskNumber].deadline);
            weightSum += instance[taskNumber].weight;
        }

        criterion = (float) criterion / (float) weightSum;
        cout << instanceSize << ": " << fixed << setprecision(2) << criterion << " ";
        cout << presumedCriterion << " " << times[i] << endl;
    }
    cout << endl;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    rand();

    for (int idx = 0; idx < INDEXES; ++idx) {
        validate(INDEX_ARR[idx]);
    }

    return EXIT_SUCCESS;
}