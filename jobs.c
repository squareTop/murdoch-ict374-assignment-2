#include "jobs.h"

/**
 * Changes 'done' attribute of job at specified index of array.
 * Returns 1 if successfully changed, else 0.
 *
 * @param  {int}    process_id
 * @param  {int}    index
 * @param  {Job **} jobs
 * @return {int}
 */
int mark_job_as_done(int process_id, int index, Job ** jobs) {
  Job * job = NULL;
  int i;

  for (i = 0; i < index; i++) {
    job = jobs[i];

    if (job != NULL && job->process_id == process_id) {
      job->done = 1;
      return 1;
    }
  }

  return 0;
}

/**
 * Adds a job to the specified index of the array.
 * Returns an incremented value of the provided index.
 *
 * @param  {char *} command
 * @param  {int}    process_id
 * @param  {int}    index
 * @param  {Job **} jobs
 * @return {int}
 */
int add_job(char * command, int process_id, int index, Job ** jobs) {
  Job * new_job = calloc(1, sizeof(Job));

  strcpy(new_job->command, command);
  new_job->process_id = process_id;
  new_job->index      = index;
  new_job->done       = 0;

  printf("[%d] %d\n", index + 1, process_id);

  jobs[index] = new_job;

  return index + 1;
}

/**
 * Helper function that prints jobs in the array.
 *
 * @param {int}    index
 * @param {Job **} jobs
 */
void print_jobs(int index, Job ** jobs) {
  int i;
  Job * job = NULL;

  for (i = 0; i < index; i++) {
    if (jobs[i] != NULL) {
      job = jobs[i];
      printf("Job | %s | %d | %d\n", job->command, job->process_id, job->done);
    }
  }

  printf("print_jobs | index: %d\n", index);
};

/**
 * Frees a job from an array and memory.
 * Returns 0 if job_index == current_index - 1, which signifies that we freed
 * the last job in the array. Otherwise, we just return what was given to us.
 *
 * @param  {int}    job_index
 * @param  {int}    current_index
 * @param  {Job **} jobs
 * @return {int}
 */
int free_job(int job_index, int current_index, Job ** jobs) {
  if (jobs[job_index] != NULL) {
    jobs[job_index]->command[0] = '\0';
    free(jobs[job_index]);
    jobs[job_index] = NULL;
  }

  if (job_index == current_index - 1) {
    return 0;
  }

  return current_index;
}

/**
 * Prints the jobs that have been completed and then proceeds to free them.
 *
 * @param  {int}    index
 * @param  {Job **} jobs
 * @return {int}
 */
int print_jobs_done(int index, Job ** jobs) {
  int i;
  int return_index = index;
  Job * job = NULL;

  for (i = 0; i < index; i++) {
    job = jobs[i];

    if (job != NULL && job->done == 1) {
      printf("[%d]   %-23s %s\n", job->index + 1, "Done", job->command);
      return_index = free_job(job->index, index, jobs);
    }
  }

  return return_index;
}
/*
int main(void) {
  Job * job_list[100];
  int next_job_index = 0;

  next_job_index = add_job("sleep 1", 1111, next_job_index, job_list);
  next_job_index = add_job("sleep 2", 1112, next_job_index, job_list);
  next_job_index = add_job("sleep 3", 1113, next_job_index, job_list);

  print_jobs(next_job_index, job_list);

  mark_job_as_done(1112, next_job_index, job_list);
  mark_job_as_done(1113, next_job_index, job_list);

  next_job_index = add_job("sleep 20", 1114, next_job_index, job_list);

  print_jobs_done(next_job_index, job_list);

  mark_job_as_done(1111, next_job_index, job_list);
  mark_job_as_done(1114, next_job_index, job_list);

  print_jobs_done(next_job_index, job_list);

  next_job_index = add_job("sleep 5", 1115, next_job_index, job_list);

  print_jobs_done(next_job_index, job_list);

  print_jobs_done(next_job_index, job_list);

  return 0;
}*/