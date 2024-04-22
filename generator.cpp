#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <cmath>
#include "generator.h"
#include "tasks.h"

// Generate task utilizations (Ui = Ci / Ti) using the UUnifast algorithm [1] (for unbiased distribution)

void generate_task_utilizations(std::vector<Task>& tasks, double max_util) {

    double sum_util = max_util;
    double rem_sum_util = 0.0;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    assert(!tasks.empty() && max_util > 0.0 && max_util < 1.0);

    for (size_t i = 0; i < tasks.size() - 1; i++) {
        rem_sum_util = sum_util * pow(dis(gen), 1.0 / (tasks.size() - i));
        tasks[i].utilization = sum_util - rem_sum_util;
        tasks[i].utilization = std::round(tasks[i].utilization * 100) / 100.0;
        sum_util = rem_sum_util;
        assert(rem_sum_util >= 0.0 && rem_sum_util <= max_util && sum_util >= 0.0 && sum_util <= max_util);
    }

    // Adjusting for the last task as well
    tasks.back().utilization = std::round(sum_util * 100) / 100.0;
    assert(sum_util >= 0.0 && sum_util <= max_util);
}

// Generate task periods Ti according as per log-uniform distribution [2]

void generate_task_periods(std::vector<Task>& tasks) {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(log(MIN_PERIOD), log(MAX_PERIOD + GRANULARITY));

    assert(!tasks.empty());

    for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
        double random_number = dist(gen);
        tasks[i].period = static_cast<int>(std::floor(std::exp(random_number) / GRANULARITY) * GRANULARITY);
        assert(tasks[i].period >= MIN_PERIOD && tasks[i].period <= (MAX_PERIOD + GRANULARITY));
    }
}

// Generates task deadlines Di according to log uniform distribution in the range [MIN_DEADLINE_FACTOR, MAX_DEADLINE_FACTOR]Ti

void generate_task_deadlines(std::vector<Task>& tasks) {
 
    std::random_device rd;
    std::mt19937 gen(rd());
    
    assert(!tasks.empty());

    for (size_t i = 0; i < tasks.size(); i++) {
        double log_min_deadline = std::log(MIN_DEADLINE_FACTOR * tasks[i].period);
        double log_max_deadline = std::log(MAX_DEADLINE_FACTOR * tasks[i].period + GRANULARITY);
        std::uniform_real_distribution<double> dist(log_min_deadline, log_max_deadline);

        double random_number = dist(gen);
        tasks[i].deadline = static_cast<int>(std::floor(std::exp(random_number) / GRANULARITY) * GRANULARITY);
        
        assert(tasks[i].deadline >= MIN_DEADLINE_FACTOR * tasks[i].period - 1 && tasks[i].deadline <= MAX_DEADLINE_FACTOR * tasks[i].period);
    }
}

// Determine the worst-case execution times of each task --> Ci = Ui * Ti

void generate_task_wcets(std::vector<Task>& tasks) {

    assert(!tasks.empty());

    for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
        tasks[i].wcet = tasks[i].period * tasks[i].utilization;
        assert(tasks[i].wcet >= 0.0 && tasks[i].wcet < tasks[i].period);
    }
}

// TODO: Finalize

int generate_num_phases (int p) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, p);
    return dist(gen);
}

double generate_cleanup_cost(double wcet, double q_fraction) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return wcet * q_fraction * dist(gen);
}




















