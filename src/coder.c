#include "codexion.h"

// input: ./codexion 2 300 30 30 30 5 0 fifo
void compiling(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->log_lock);
    printf("Coder %d is compiling\n", coder->id);
    pthread_mutex_unlock(&sim->log_lock);

    usleep(sim->config.time_to_compile);
    coder->last_compile_start = get_time_ms();
    coder->compile_count++;
}

void debugging(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->log_lock);
    printf("Coder %d is debugging\n", coder->id);
    pthread_mutex_unlock(&sim->log_lock);

    usleep(sim->config.time_to_debug);
}

void refactoring(t_sim *sim, t_coder *coder)
{
    pthread_mutex_lock(&sim->log_lock);
    printf("Coder %d is refactoring\n", coder->id);
    pthread_mutex_unlock(&sim->log_lock);

    usleep(sim->config.time_to_refactor);
}

void *coder_routine(void *arg)
{
    t_thread_args *thread_args;
    t_sim *sim;

    thread_args = (t_thread_args *)arg;
    sim = thread_args->sim;
    compiling(sim, &sim->coders[thread_args->coder_index]);
    debugging(sim, &sim->coders[thread_args->coder_index]);
    refactoring(sim, &sim->coders[thread_args->coder_index]);
    return (NULL);
}