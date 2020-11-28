    #include <iostream>
    #include <fstream>
    #include <cstdlib>
    #include <vector>
    #include <string>
    #include <ctime>
    #include <windows.h>

    #include "settings.h"

    using namespace std;

    struct Task {
        int processingTime;
        int readinessTime;
        int deadlineTime;
        int weight;
    };

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

    void testSchedules(string index) {
        vector<vector<Task>> instances(10);

        for (int i = 0; i < INSTANCES; ++i) {
            vector<Task> instance;

            string filename = ".\\instances\\in_136814\\in_136814_" + to_string(INSTANCE_SIZES[i]) + ".txt";
            ifstream infile(filename);
            int size;

            infile >> size;

            for (int j = 0; j < size; ++j) {
                Task task;
                infile >> task.processingTime >> task.readinessTime >> task.deadlineTime >> task.weight;
                instance.push_back(task);
            }

            infile.close();
            instances[i] = instance;
        }

        vector<int> times;

        for (int i = 0; i < INSTANCES; ++i) {
            string inputCmd = ".\\al_" + index + ".exe .\\\\instances\\\\in_136814\\\\in_136814_" + to_string(INSTANCE_SIZES[i]) + ".txt";

            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            TimeGuard guard = TimeGuard();

            CreateProcess(NULL, (LPWSTR)inputCmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);

            times.push_back(guard.stop());
        }

        for (int i = 0; i < INSTANCES; ++i) {

            string filename = ".\\instances\\in_136814\\out_136814_" + to_string(INSTANCE_SIZES[i]) + ".txt";
            ifstream infile(filename);

            int presumedCriterion;
            int currentTime = 0;
            int criterion = 0;

            infile >> presumedCriterion;

            for (int j = 0; j < INSTANCE_SIZES[i]; ++j) {
                int taskNumber;
                infile >> taskNumber;
                Task task = instances[i][taskNumber - 1];

                if (currentTime < task.readinessTime) {
                    currentTime = task.readinessTime;
                }

                currentTime += task.processingTime;
                if (currentTime > task.deadlineTime) {
                    criterion += task.weight;
                }
            }

            cout << INSTANCE_SIZES[i] << ": " << criterion << " " << (presumedCriterion == criterion ? "valid" : "invalid") << ", t= " << times[i] << endl;

            infile.close();
        }
    }

    int main(int argc, char** argv) {

        if (argc != 2) {
            cout << "Please provide required args: index" << endl;
            return EXIT_FAILURE;
        }

        string index = argv[1];
        testSchedules(index);

        return EXIT_SUCCESS;
    }