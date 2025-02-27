#include "tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Check if the user provided a command.
    if (argc < 2) {
        printf("Usage: %s <command> [task_name]\n", argv[0]);
        printf("Commands:\n");
        printf("  start <task_name>   Start a new task.\n");
        printf("  stop                Stop the current task.\n");
        printf("  report              Generate a time report.\n");
        return EXIT_FAILURE;
    }

    // Determine which command was provided.
    if (strcmp(argv[1], "start") == 0) {
        // The start command requires a task name.
        if (argc < 3) {
            printf("Error: Missing task name for start command.\n");
            return EXIT_FAILURE;
        }
        start_task(argv[2]);
    } else if (strcmp(argv[1], "stop") == 0) {
        stop_task();
    } else if (strcmp(argv[1], "report") == 0) {
        report_tasks();
    } else {
        printf("Error: Unknown command '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
