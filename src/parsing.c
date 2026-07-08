#include "./codexion.h"

int parse_args(int ac, char **av, t_config *config)
{
    if (ac != 9)
    {
        printf("Number of parameters is incorrect!!\n");
        return 0;
    }
    config->number_of_coders = atoi(av[1]);
    config->time_to_burnout = atoi(av[2]);
    config->time_to_compile = atoi(av[3]);
    config->time_to_debug = atoi(av[4]);
    config->time_to_refactor = atoi(av[5]);
    config->number_of_compiles_required = atoi(av[6]);
    config->dongle_cooldown = atoi(av[7]);
    config->scheduler = av[8];

    if (config->number_of_coders <= 0 || config->time_to_burnout < 0
		|| config->time_to_compile < 0 || config->time_to_debug < 0 
		|| config->time_to_refactor < 0 || config->number_of_compiles_required < 0
		|| config->dongle_cooldown < 0)
    {
        printf("Invalid parameter values!!\n");
        return 0;
    }

    return 1;
}