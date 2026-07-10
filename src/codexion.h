#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define STATE_START 0
#define STATE_COMPILING 1
#define STATE_DEBUGGING 2
#define STATE_REFACTORING 3
#define STATE_BURNED_OUT 4


typedef struct s_config{

    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
}       t_config;

typedef struct s_coder
{
    int id;
    long last_compile_start;
    int compile_count;
    int state;
}       t_coder;

typedef struct s_dongle
{
    int id;
    int owner_id;
    int available;
    long cooldown_until;
    pthread_mutex_t lock;
    pthread_cond_t cond;
}       t_dongle;

typedef struct s_simulation
{
    t_config config;
    t_coder *coders;
    t_dongle *dongles;
    pthread_t *threads;
    pthread_t monitor_thread;
    long start_time;
    int running;
    pthread_mutex_t log_lock;
    pthread_mutex_t sim_lock;
}       t_sim;

typedef struct s_thread_args
{
    t_sim *sim;
    int coder_index;
}       t_thread_args;

// priority queue heap structs
typedef struct s_queue_node
{
    int coder_id;
    long deadline;
    int priority;
}   t_queue_node;

typedef struct s_priority_queue
{
    t_queue_node *heap;
    int size;
    int capacity;
}   t_priority_queue;

// parsing
int init_sim(t_sim *sim);
void cleanup_sim(t_sim *sim);
void *coder_routine(void *arg);

#endif