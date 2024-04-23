#ifndef GENERATOR_H
#define GENERATOR_H

#include "tasks.h"

// =================
// MACRO DEFINITIONS
// =================

// Task period bounds
#define MIN_PERIOD 10000
#define MAX_PERIOD 1000000
#define GRANULARITY 1

// Task deadline = DEADLINE_FACTOR * task period
// #define MIN_DEADLINE_FACTOR 0.25
// #define MAX_DEADLINE_FACTOR 4.0

// Deadline factor
#define DEADLINE_FACTOR 1.0

// =====================
// FUNCTION DECLARATIONS
// =====================

// Task utilizations (Ui = Ci / Ti) are generated using UUnifast [1] providing an unbiased distribution
void generate_task_utilizations(std::vector<Task>& tasks, double max_util);

// Task periods Ti were generated according to a log-uniform distribution [2]
void generate_task_periods(std::vector<Task>& tasks);

// Task deadlines are generated according to a log-uniform distribution [2] in the range [0.25, 4.0]Ti
void generate_task_deadlines(std::vector<Task>& tasks);

// The worst-case execution time of each task is given by Ci = Ui · Ti
void generate_task_wcets(std::vector<Task>& tasks);

// Task parameter generator driver function
// Tasks* task_parameter_generator (Tasks *tasks, int num_tasks, double total_util);

// Fetch task parameters as input from file and stores it in the task structure array
// void fetch_task_parameters ( FILE* input_file, Tasks *tasks, int num_tasks);

#endif
