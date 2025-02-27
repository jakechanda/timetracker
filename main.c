#include "tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Initialize tracker (sets base directory and loads current project)
    init_tracker();

    if (argc < 2) {
        printf("Usage: %s <command> [arguments]\n", argv[0]);
        printf("Commands:\n");
        printf("  create <project_name>    Create a new project.\n");
        printf("  switch <project_name>    Switch to an existing project.\n");
        printf("  start <task_name>        Start a new task.\n");
        printf("  stop                     Stop the current task.\n");
        printf("  report                   Generate a time report.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "create") == 0) {
        if (argc < 3) {
            printf("Error: Missing project name for create command.\n");
            return EXIT_FAILURE;
        }
        create_project(argv[2]);
    } else if (strcmp(argv[1], "switch") == 0) {
        if (argc < 3) {
            printf("Error: Missing project name for switch command.\n");
            return EXIT_FAILURE;
        }
        switch_project(argv[2]);
    } else if (strcmp(argv[1], "start") == 0) {
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

