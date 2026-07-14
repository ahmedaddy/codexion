#include "./codexion.h"

static int ft_atoi(const char *str)
{
    long long res = 0;
    int sign = 1;

    while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        res = res * 10 + (*str - '0');
        if (sign == 1 && res > INT_MAX)
            return (-1);
        if (sign == -1 && -res < INT_MIN)
            return (-1);
        str++;
    }
    return res * sign;
}

int parse_args(int ac, char **av, t_config *config)
{
    if (ac != 9)
    {
        printf("Number of parameters is incorrect!!\n");
        return 0;
    }
    config->number_of_coders = ft_atoi(av[1]);
    config->time_to_burnout = ft_atoi(av[2]);
    config->time_to_compile = ft_atoi(av[3]);
    config->time_to_debug = ft_atoi(av[4]);
    config->time_to_refactor = ft_atoi(av[5]);
    config->number_of_compiles_required = ft_atoi(av[6]);
    config->dongle_cooldown = ft_atoi(av[7]);
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