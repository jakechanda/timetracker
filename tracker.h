#ifndef TRACKER_H
#define TRACKER_H

// Initializes the tracker (sets the base directory and loads current project)
void init_tracker(void);

// Time tracker functions:
void start_task(const char *task_name);
void stop_task(void);
void report_tasks(void);

// Project management functions:
void create_project(const char *project_name);
void switch_project(const char *project_name);

#endif // TRACKER_H

