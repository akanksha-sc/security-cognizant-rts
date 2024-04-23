#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <cmath>
#include "generator.h"
#include "tasks.h"

// Generate task utilizations (Ui = Ci / Ti) using the UUnifast algorithm [1] (for unbiased distribution)

void generate_task_utilizations(std::vector<Tasks>& tasks, double max_util) {
    
    assert(!tasks.empty() && max_util > 0.0 && max_util < 1.0);

    double sum_util = max_util;
    double rem_sum_util = 0.0;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (size_t i = 0; i < tasks.size() - 1; i++) {
        rem_sum_util = sum_util * pow(dist(gen), 1.0 / (tasks.size() - i));
        tasks[i].utilization = sum_util - rem_sum_util;
        tasks[i].utilization = std::round(tasks[i].utilization * 100) / 100.0;
        sum_util = rem_sum_util;
        assert(rem_sum_util >= 0.0 && rem_sum_util <= max_util && sum_util >= 0.0 && sum_util <= max_util);
    }

    tasks.back().utilization = std::round(sum_util * 100) / 100.0;
    assert(sum_util >= 0.0 && sum_util <= max_util);
}

// Generate task periods Ti according as per log-uniform distribution [2]

void generate_task_periods(std::vector<Tasks>& tasks) {
    
    assert(!tasks.empty());
    
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> dist(log(MIN_PERIOD), log(MAX_PERIOD + GRANULARITY));

    for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
        double random_number = dist(gen);
        tasks[i].period = std::floor(std::exp(random_number) / GRANULARITY) * GRANULARITY;
        assert(tasks[i].period >= MIN_PERIOD && tasks[i].period <= (MAX_PERIOD + GRANULARITY));
    }
}

// Determine the worst-case execution times and cleanup costs fore each task 
// --> Ci = Ui * Ti, Qi = QF * Ci (if p < ptrust), 0 (otherwise)

void generate_task_wcets(std::vector<Tasks>& tasks) {

    assert(!tasks.empty());

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double sum = 0.0;

    for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
        double random_number = dist(gen);
	tasks[i].ptrust = TRUST_PROBABILITY;

	if (random_number < tasks[i].ptrust) {
            sum = tasks[i].period * tasks[i].utilization;
	    tasks[i].cleanup = Q_FRACTION * sum;
	    tasks[i].wcet = sum - tasks[i].cleanup;
	}
	else {
	    tasks[i].cleanup = 0.0;
            tasks[i].wcet = tasks[i].period * tasks[i].utilization;
	}
        assert(tasks[i].wcet >= 0.0 && sum < tasks[i].period && tasks[i].cleanup >= 0.0 && tasks[i].cleanup <= tasks[i].wcet);
    }
}


// Generates task deadlines Di according to the uniform distribution in the range defined by [3]

void generate_task_deadlines(std::vector<Tasks>& tasks) {

    assert(!tasks.empty());
 
    std::random_device rd;
    std::mt19937_64 gen(rd());

    for (size_t i = 0; i < tasks.size(); i++) {

        // Generates task deadlines Di according to log uniform distribution in the range [MIN_DEADLINE_FACTOR, MAX_DEADLINE_FACTOR]Ti

	// double log_min_deadline = std::log(MIN_DEADLINE_FACTOR * tasks[i].period);
        // double log_max_deadline = std::log(MAX_DEADLINE_FACTOR * tasks[i].period + GRANULARITY);
        // std::uniform_real_distribution<double> dist(log_min_deadline, log_max_deadline);

        // double random_number = dist(gen);
        // tasks[i].deadline = static_cast<int>(std::floor(std::exp(random_number) / GRANULARITY) * GRANULARITY);
        
        // assert(tasks[i].deadline >= MIN_DEADLINE_FACTOR * tasks[i].period - 1 && tasks[i].deadline <= MAX_DEADLINE_FACTOR * tasks[i].period);

	double min_deadline = tasks[i].wcet * tasks[i].cleanup;
        double max_deadline = min_deadline + DEADLINE_FACTOR * (tasks[i].period - min_deadline);
        std::uniform_real_distribution<double> dist(min_deadline, max_deadline);

        double random_number = dist(gen);
	std::cout << "Max deadline: " << max_deadline << " d: " << random_number << std::endl;
	tasks[i].deadline = random_number;
	// assert(tasks[i].deadline >= min_deadline && tasks[i].deadline < max_deadline);
    }
}


// Driver function to generate task parameters
std::vector<Tasks> generate_tasks() {
    std::vector<Tasks> tasks(NUM_TASKS);

    // Generate task parameters
    generate_task_utilizations(tasks, MAX_UTILIZATION);
    generate_task_periods(tasks);
    generate_task_wcets(tasks);
    generate_task_deadlines(tasks);

    return tasks;
}

int main() {
    // Generate tasks
    std::vector<Tasks> tasks = generate_tasks();

    // Display generated tasks
    std::cout << "Generated Tasks:\n";
    for (int i = 0; i < NUM_TASKS; ++i) {
        std::cout << "Task " << i+1 << ":\n";
        std::cout << "Utilization: " << tasks[i].utilization << ", ";
        std::cout << "Period: " << tasks[i].period << ", ";
        std::cout << "WCET: " << tasks[i].wcet << ", ";
        std::cout << "Trust Probability: " << tasks[i].ptrust << ", ";
        std::cout << "Cleanup: " << tasks[i].cleanup << ", ";
        std::cout << "Deadline: " << tasks[i].deadline << "\n";
    }

    return 0;
}



