#ifndef MULTI_PHASE_TASKS_H
#define MULTI_PHASE_TASKS_H

// =================
// MACRO DEFINITIONS
// =================

// Number of tasks and tasksets
#define NUM_TASKSETS 10
#define NUM_TASKS 4
#define NUM_PHASES 4
// =============================
// ABSTRACT DATATYPE DEFINITIONS
// =============================

// Task structure
typedef struct Tasks{ 
    int id = 0;                 // To identify a given task structure
    double period = 0.0;        // Minimum inter-arrival time between any two successive jobs of this task
    double deadline = 0.0;      // Relative deadline 
    double wcet = 0.0;          // Worst-case execution time
    double utilization = 0.0;   // Task utilization
    double cleanup = 0.0;       // Clean up cost = (Q fraction * WCET)
    double ptrust = 0.0;        // Probability that this task is trusted
                                // (i.e., this is a potential victim task,needing to perform clean-up operations).
    int num_jobs = 0;           // Number of jobs in floor_upper_bound_t interval
    int phases = NUM_PHASES;             // Number of security phases in the task - Currently assume all phases have same wcet and cleanup
} Tasks;

#endif
