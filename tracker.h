#ifndef TRACKER_H
#define TRACKER_H

// Starts a task with the given name.
void start_task(const char *task_name);

// Stops the currently running task.
void stop_task(void);

// Generates a report of all logged tasks.
void report_tasks(void);

#endif // TRACKER_H
