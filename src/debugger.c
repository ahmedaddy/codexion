#include "codexion.h"

void print_config(t_config *config)
{
    printf("Number of coders: %d\n", config->number_of_coders);
    printf("Time to burnout: %d\n", config->time_to_burnout);
    printf("Time to compile: %d\n", config->time_to_compile);
    printf("Time to debug: %d\n", config->time_to_debug);
    printf("Time to refactor: %d\n", config->time_to_refactor);
    printf("Number of compiles required: %d\n", config->number_of_compiles_required);
    printf("Dongle cooldown: %d\n", config->dongle_cooldown);
    printf("Scheduler: %s\n", config->scheduler);
}

void print_sim(t_sim *sim)
{
    print_config(&sim->config);
    printf("Start time: %ld\n", sim->start_time);
    printf("Simulation state:\n");
    printf("Running: %d\n", sim->running);
    printf("Number of coders: %d\n", sim->config.number_of_coders);
    for (int i = 0; i < sim->config.number_of_coders; i++)
    {
        printf("Coder %d: last compile start: %ld, compile count: %d, state: %d\n",
               sim->coders[i].id, sim->coders[i].last_compile_start,
               sim->coders[i].compile_count, sim->coders[i].state);
    }
}