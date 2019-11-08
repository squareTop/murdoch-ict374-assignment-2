#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct JobStruct {
  char command[256];
  int  process_id;
  int  index;
  int  done;
} Job;

int  add_job(char * command, int process_id, int index, Job ** jobs);
int  mark_job_as_done(int process_id, int index, Job ** jobs);
int  free_job(int job_index, int current_index, Job ** jobs);
void print_jobs(int index, Job ** jobs);
int  print_jobs_done(int index, Job ** jobs);