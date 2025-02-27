#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CURRENT_TASK_FILE "current_task.txt"
#define TIMELOG_FILE "timelog.txt"

// Starts a task by saving its name and the current time to a temporary file.
void start_task(const char *task_name) {
    // Check if a task is already running.
    FILE *fp = fopen(CURRENT_TASK_FILE, "r");
    if (fp != NULL) {
        fclose(fp);
        printf("Error: A task is already running. Stop it before starting a new one.\n");
        exit(EXIT_FAILURE);
    }

    fp = fopen(CURRENT_TASK_FILE, "w");
    if (fp == NULL) {
        perror("Error opening current task file");
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    // Save the task name and start time.
    fprintf(fp, "%s\n%ld\n", task_name, (long)now);
    fclose(fp);
    printf("Task \"%s\" started at %s", task_name, ctime(&now));
}

// Stops the currently running task, calculates its duration, and logs it.
void stop_task() {
    FILE *fp = fopen(CURRENT_TASK_FILE, "r");
    if (fp == NULL) {
        printf("Error: No task is currently running.\n");
        exit(EXIT_FAILURE);
    }

    char task_name[100];
    long start_time;
    if (fscanf(fp, "%99[^\n]\n%ld", task_name, &start_time) != 2) {
        printf("Error reading current task file.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    time_t now = time(NULL);
    double elapsed = difftime(now, start_time);
    printf("Task \"%s\" stopped. Duration: %.0f seconds.\n", task_name, elapsed);

    // Append the task details to the log file.
    fp = fopen(TIMELOG_FILE, "a");
    if (fp == NULL) {
        perror("Error opening timelog file");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%s %ld %ld\n", task_name, start_time, (long)now);
    fclose(fp);

    // Remove the temporary file.
    if (remove(CURRENT_TASK_FILE) != 0) {
        perror("Error removing current task file");
    }
}

// Reads the log file, aggregates the durations for each task, and prints a report.
void report() {
    FILE *fp = fopen(TIMELOG_FILE, "r");
    if (fp == NULL) {
        printf("No logs available. Start tracking some tasks first.\n");
        return;
    }

    // Simple aggregation: support up to 100 unique tasks.
    struct {
        char name[100];
        double total_seconds;
    } tasks[100];
    int task_count = 0;

    char task_name[100];
    long start_time, end_time;
    while (fscanf(fp, "%99s %ld %ld", task_name, &start_time, &end_time) == 3) {
        double elapsed = difftime(end_time, start_time);
        int found = 0;
        // Check if we've already seen this task.
        for (int i = 0; i < task_count; i++) {
            if (strcmp(tasks[i].name, task_name) == 0) {
                tasks[i].total_seconds += elapsed;
                found = 1;
                break;
            }
        }
        if (!found && task_count < 100) {
            strcpy(tasks[task_count].name, task_name);
            tasks[task_count].total_seconds = elapsed;
            task_count++;
        }
    }
    fclose(fp);

    printf("\nTime Tracker Report:\n");
    for (int i = 0; i < task_count; i++) {
        printf("Task: %s, Total Time: %.0f seconds\n", tasks[i].name, tasks[i].total_seconds);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [task_name]\n", argv[0]);
        printf("Commands:\n");
        printf("  start <task_name>   Start tracking a new task.\n");
        printf("  stop                Stop the current task.\n");
        printf("  report              Show a report of logged tasks.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "start") == 0) {
        if (argc < 3) {
            printf("Please provide a task name to start.\n");
            return EXIT_FAILURE;
        }
        start_task(argv[2]);
    } else if (strcmp(argv[1], "stop") == 0) {
        stop_task();
    } else if (strcmp(argv[1], "report") == 0) {
        report();
    } else {
        printf("Unknown command: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
