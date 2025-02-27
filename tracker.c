#include "tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Define file names for storing the current task and the log.
#define CURRENT_TASK_FILE "current_task.txt"
#define TIMELOG_FILE "timelog.txt"

void start_task(const char *task_name) {
    // Check if CURRENT_TASK_FILE already exists (a task is running).
    //   If yes:
    //     Print an error: "A task is already running." and return.
    FILE *filepointer;
    filepointer = fopen(CURRENT_TASK_FILE, "r");
    if(filepointer != NULL) 
    {
        fpclose(filepointer);
        perror("A task is already being recorded, please close it and try again.")
        exit(EXIT_FAILURE);
    }
    // Open CURRENT_TASK_FILE in write mode.
    //   If the file fails to open, print an error and exit.
    
    // Get the current time.
    // Write the task name and the current time to the file.
    // Close the file.
    
    // Print confirmation that the task has started with its start time.
}

// Pseudocode for stopping a task:
void stop_task(void) {
    // Try to open CURRENT_TASK_FILE in read mode.
    //   If the file does not exist:
    //     Print an error: "No task is currently running." and return.
    
    // Read the task name and the start time from CURRENT_TASK_FILE.
    // Close the file.
    
    // Get the current time.
    // Calculate the elapsed time (current time - start time).
    // Print a message showing the task name and its duration.
    
    // Open TIMELOG_FILE in append mode.
    // Append a log entry with: task name, start time, and stop time.
    // Close the log file.
    
    // Remove CURRENT_TASK_FILE to indicate that no task is active.
}

// Pseudocode for reporting logged tasks:
void report_tasks(void) {
    // Open TIMELOG_FILE in read mode.
    //   If the file does not exist, print "No logs available" and return.
    
    // Initialize a data structure (e.g., an array or list) to aggregate tasks.
    
    // For each record in the log file:
    //   - Read the task name, start time, and stop time.
    //   - Calculate the elapsed time (stop time - start time).
    //   - If the task is already in the data structure, add to its total elapsed time.
    //   - Otherwise, add a new entry for this task.
    
    // Close the file.
    
    // Print a summary report that lists each task and its total time spent.
}
