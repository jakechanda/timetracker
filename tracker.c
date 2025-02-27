#include "tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

// Global variables for the base directory and current project.
// The current project will be persisted across runs.
static char base_dir[512] = "";
static char current_project[256] = "default";

// Initializes the base directory to "<home>/timetrack"
static void init_base_dir(void) {
    char *home = NULL;
#ifdef _WIN32
    home = getenv("USERPROFILE");
    if (!home) {
        home = ".";
    }
    // Build base_dir using backslashes on Windows.
    snprintf(base_dir, sizeof(base_dir), "%s\\timetrack", home);
#else
    home = getenv("HOME");
    if (!home) {
        home = ".";
    }
    snprintf(base_dir, sizeof(base_dir), "%s/timetrack", home);
#endif

    // Create the base directory if it does not exist.
    if (MKDIR(base_dir) == 0) {
        printf("Base directory created at %s\n", base_dir);
    }
    // (If MKDIR fails because the directory exists, that's fine.)
}

// Loads the current project from a configuration file in the base directory.
static void load_current_project(void) {
    char config_path[512];
#ifdef _WIN32
    snprintf(config_path, sizeof(config_path), "%s\\current_project.txt", base_dir);
#else
    snprintf(config_path, sizeof(config_path), "%s/current_project.txt", base_dir);
#endif

    FILE *fp = fopen(config_path, "r");
    if (fp != NULL) {
        if (fgets(current_project, sizeof(current_project), fp) != NULL) {
            // Remove any trailing newline.
            size_t len = strlen(current_project);
            if (len > 0 && current_project[len - 1] == '\n') {
                current_project[len - 1] = '\0';
            }
        }
        fclose(fp);
    }
}

// Saves the current project to the configuration file.
static void save_current_project(void) {
    char config_path[512];
#ifdef _WIN32
    snprintf(config_path, sizeof(config_path), "%s\\current_project.txt", base_dir);
#else
    snprintf(config_path, sizeof(config_path), "%s/current_project.txt", base_dir);
#endif

    FILE *fp = fopen(config_path, "w");
    if (fp != NULL) {
        fprintf(fp, "%s\n", current_project);
        fclose(fp);
    }
}

// Builds a full file path in the current project directory.
// The final path will be: base_dir/current_project/filename
static void get_project_filepath(const char *filename, char *out_path, size_t len) {
#ifdef _WIN32
    snprintf(out_path, len, "%s\\%s\\%s", base_dir, current_project, filename);
#else
    snprintf(out_path, len, "%s/%s/%s", base_dir, current_project, filename);
#endif
}

// Builds the full path for a project directory.
// The path will be: base_dir/project_name
static void get_project_directory(const char *project_name, char *out_path, size_t len) {
#ifdef _WIN32
    snprintf(out_path, len, "%s\\%s", base_dir, project_name);
#else
    snprintf(out_path, len, "%s/%s", base_dir, project_name);
#endif
}

// Initializes the tracker: sets the base directory, loads the current project,
// and creates the current project's directory if needed.
void init_tracker(void) {
    init_base_dir();
    load_current_project();

    // Create the current project directory if it doesn't exist.
    char proj_path[512];
    get_project_directory(current_project, proj_path, sizeof(proj_path));
    if (MKDIR(proj_path) == 0) {
        printf("Project '%s' created at %s.\n", current_project, proj_path);
    }
}

//----------------------
// Time Tracker Functions
//----------------------

void start_task(const char *task_name) {
    char path[512];
    get_project_filepath("current_task.txt", path, sizeof(path));

    // Check if a task is already running.
    FILE *fp = fopen(path, "r");
    if (fp != NULL) {
        fclose(fp);
        printf("Error: A task is already running in project '%s'.\n", current_project);
        return;
    }

    fp = fopen(path, "w");
    if (fp == NULL) {
        perror("Error opening current task file");
        exit(EXIT_FAILURE);
    }

    time_t now = time(NULL);
    fprintf(fp, "%s\n%ld\n", task_name, (long)now);
    fclose(fp);
    printf("Task \"%s\" started in project '%s' at %s", task_name, current_project, ctime(&now));
}

void stop_task(void) {
    char current_path[512];
    get_project_filepath("current_task.txt", current_path, sizeof(current_path));

    FILE *fp = fopen(current_path, "r");
    if (fp == NULL) {
        printf("Error: No task is currently running in project '%s'.\n", current_project);
        return;
    }

    char task_name[100];
    long start_time;
    if (fscanf(fp, "%99[^\n]\n%ld", task_name, &start_time) != 2) {
        printf("Error reading current task file.\n");
        fclose(fp);
        return;
    }
    fclose(fp);

    time_t now = time(NULL);
    double elapsed = difftime(now, start_time);
    printf("Task \"%s\" stopped in project '%s'. Duration: %.0f seconds.\n", task_name, current_project, elapsed);

    char log_path[512];
    get_project_filepath("timelog.txt", log_path, sizeof(log_path));

    fp = fopen(log_path, "a");
    if (fp == NULL) {
        perror("Error opening timelog file");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%s %ld %ld\n", task_name, start_time, (long)now);
    fclose(fp);

    // Remove the current task file.
    if (remove(current_path) != 0) {
        perror("Error removing current task file");
    }
}

void report_tasks(void) {
    char log_path[512];
    get_project_filepath("timelog.txt", log_path, sizeof(log_path));
    FILE *fp = fopen(log_path, "r");
    if (fp == NULL) {
        printf("No logs available in project '%s'.\n", current_project);
        return;
    }

    // Aggregate tasks (supporting up to 100 unique tasks).
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

    printf("\nTime Tracker Report for project '%s':\n", current_project);
    for (int i = 0; i < task_count; i++) {
        printf("Task: %s, Total Time: %.0f seconds\n", tasks[i].name, tasks[i].total_seconds);
    }
}

//----------------------
// Project Management Functions
//----------------------

void create_project(const char *project_name) {
    char proj_path[512];
    get_project_directory(project_name, proj_path, sizeof(proj_path));
    int ret = MKDIR(proj_path);
    if (ret == 0) {
        printf("Project '%s' created successfully at %s.\n", project_name, proj_path);
    } else {
        printf("Error: Unable to create project '%s'. It may already exist.\n", project_name);
    }
}

void switch_project(const char *project_name) {
    // Update the global variable.
    strcpy(current_project, project_name);
    printf("Switched to project '%s'.\n", current_project);

    // Save the new current project.
    save_current_project();

    // Optionally, create the project directory if it doesn't exist.
    char proj_path[512];
    get_project_directory(current_project, proj_path, sizeof(proj_path));
    MKDIR(proj_path);
}

