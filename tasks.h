#ifndef TASKS_H
#define TASKS_H

// =================
// MACRO DEFINITIONS
// =================

// Number of tasks and tasksets
#define NUM_TASKSETS 10
#define NUM_TASKS 4

// =============================
// ABSTRACT DATATYPE DEFINITIONS
// =============================

// Task structure
typedef struct { 
    int id;                              // To identify a given task structure
    int period;                          // Minimum inter-arrival time between any two successive jobs of this task
    int deadline;                        // Relative deadline 
    double wcet;                         // Worst-case execution times of the task at each criticality level [dual-criticality system]
    double utilization;                  // Task utilizations at each criticality level [dual-criticality system]
    int num_jobs;                        // Number of jobs in floor_upper_bound_t interval
} Tasks;

#endif
