#include "./codexion.h"

int parse_args(int ac, char **av, t_config *config);

// debuggin func
void print_config(t_config *config);
void print_sim(t_sim *sim);

int main(int ac, char **av)
{
    t_sim sim;
    struct timeval tv;
    t_thread_args *thread_args;
    int i;

    memset(&sim, 0, sizeof(t_sim));

    if (!parse_args(ac, av, &sim.config))
        return 1;    
    // print_config(&sim.config);
    if (!init_sim(&sim))
    {
        printf("Failed to initialize simulation!!\n");
        return 1;
    }
    gettimeofday(&tv, NULL);
	sim.start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    print_sim(&sim);
    thread_args = malloc(sizeof(t_thread_args) * sim.config.number_of_coders);
    if (!thread_args)
    {
        cleanup_sim(&sim);
        return 1;
    }
    i = 0;
    while (i < sim.config.number_of_coders)
    {
        thread_args[i].sim = &sim;
        thread_args[i].coder_index = i;
        if (pthread_create(&sim.threads[i], NULL, coder_routine, &thread_args[i]) != 0)
        {
            printf("Failed to create the thread\n");
            cleanup_sim(&sim);
            free(thread_args);
            return 1;
        }
        i++;
    }
    
    // to test memset (unitialized sim)
    // if (sim.running)
    //     printf("Simulation is already running! %d\n", sim.running);
    // else
    //     printf("Simulation is stopped. %d\n", sim.running);
}
