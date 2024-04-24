#include <iostream>
#include <vector>
#include <algorithm>
#include "multi_phase_tasks.h"

using namespace std;



double compute_max_blocking(vector<Tasks>& tasks, int td) {
    // Compute slack as per Eqn 7
    double dbf_task_set = 0.0;
    for(auto task:tasks) {
        dbf_task_set += compute_dbf(task, td);
    }
    double delta_td = td - dbf_task_set;

    return delta_td;

}

double compute_dbf(Tasks task, int td) {
    double dbf = std::max(std::floor((td - task.deadline) / task.period) + 1, 0.0) * task.wcet;
    return dbf;
}

bool scheduling_algorithm(vector<Tasks>& tasks) {
    int total_tasks = tasks.size();

    vector<vector<int>> intervals_per_task_phase (total_tasks, vector<int>(NUM_PHASES, 1)); //  To denote cnt(v(i,j)) the maximum number of contiguous time-intervals in which the j’th phase of a task executes
    vector<double> beta_per_task (total_tasks, 0.0); // To denote the maximum time for which a task will execute non-preemptively (beta)

    double max_testing_time = 0.0;
    for (int i=0; i<total_tasks; i++) {
        beta_per_task[i] = tasks[i].wcet, + tasks[i].cleanup; // We need to take the maximum value among all phases. However, as we assume same values for all phases, it is equivalent to this.
        max_testing_time = max(max_testing_time, tasks[i].deadline);
    }
    int max_testing_time_int  = int(max_testing_time); // Only need to consider integer values
    for (int td=1; td<=max_testing_time_int; td++) {
        double delta_td = compute_max_blocking(tasks, td);

        if (delta_td < 0) {
            cout << "THE SYSTEM IS NOT SCHEDULABLE" << endl;
            return false;
        }

        for (int i=0; i<total_tasks; i++) {
            if (tasks[i].deadline > td) {
                if (beta_per_task[i] - 1 > delta_td) {
                    beta_per_task[i] = delta_td + 1;
                }

                for (int j = 1; j <= NUM_PHASES; j++) {
                    if (beta_per_task[i] > tasks[i].cleanup) {
                        int min_chunks = 1;
                        while ((tasks[i].wcet/min_chunks) + tasks[i].cleanup > beta_per_task[i]) {
                            min_chunks++;
                        }
                        intervals_per_task_phase[i][j] = min_chunks;
                    } else {
                        cout << "THE SYSTEM IS NOT SCHEDULABLE" << endl;
                        return false;
                    }
                }
            }
        }
    }

    cout << "THE SYSTEM IS SCHEDULABLE" << endl;
    return true;
}

