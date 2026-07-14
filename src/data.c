#include "codexion.h"

int init_sim(t_sim *sim)
{
    int i;

    i = 0;
    sim->coders = malloc(sizeof(t_coder) * sim->config.number_of_coders);
    if (!sim->coders)
        return 0;
    sim->dongles = malloc(sizeof(t_dongle) * sim->config.number_of_coders);
    if (!sim->coders)
        return 0;
    sim->threads = malloc(sizeof(pthread_t) * sim->config.number_of_coders);
    if (!sim->threads)
        return 0;
    memset(sim->coders, 0, sizeof(t_coder) * sim->config.number_of_coders);
    memset(sim->dongles, 0, sizeof(t_coder) * sim->config.number_of_coders);
    while (i < sim->config.number_of_coders)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].last_compile_start = 0;
        sim->coders[i].compile_count = 0;
        sim->coders[i].state = STATE_START;
        i++;
    }
    i = 0;
    while (i < sim->config.number_of_coders)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].owner_id = -1;
        sim->dongles[i].available = 1;
        sim->dongles[i].cooldown_until = 0;
        pthread_mutex_init(&sim->dongles[i].lock, NULL);
        pthread_cond_init(&sim->dongles[i].cond, NULL);
        i++;   
    }
    pthread_mutex_init(&sim->sim_lock, NULL);
    pthread_mutex_init(&sim->log_lock, NULL);

    sim->running = 1;
    return 1;
}

void cleanup_sim(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        pthread_mutex_destroy(&sim->dongles[i].lock);
        pthread_cond_destroy(&sim->dongles[i].cond);
        i++;
    }
    pthread_mutex_destroy(&sim->log_lock);
    pthread_mutex_destroy(&sim->sim_lock);

    free(sim->coders);
    free(sim->dongles);
    free(sim->threads);
    // free(sim->coders);
}