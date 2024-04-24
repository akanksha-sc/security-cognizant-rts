#ifndef MULTI_PHASE_H
#define MULTI_PHASE_H

#include <vector>
#include "multi_phase_tasks.h"

bool scheduling_algorithm(std::vector<Tasks>& tasks);
double compute_max_blocking(std::vector<Tasks>& tasks, int td);
double compute_dbf(Tasks task, int td);


#endif