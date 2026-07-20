/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 16:54:57 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/20 16:57:31 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_atoi(char *str)
{
	long long	num;
	int			i;

	if (!str || str[0] == '\0')
		return (-1);
	i = 0;
	num = 0;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		return (-1);
	if (str[i] == '\0')
		return (-1);

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		num = num * 10 + (str[i] - '0');
		if (num > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)num);
}

int	parsing(int ac, char *av[], t_config *config)
{
	if (ac < 9)
	{
		printf("Number of arguments is not correct");
		return (0);
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
		|| config->time_to_refactor < 0
		|| config->number_of_compiles_required < 0
		|| config->dongle_cooldown < 0)
	{
		printf("One of arguments is not correct\n");
		return (0);
	}
	if (strcmp(config->scheduler, "fifo") != 0 && strcmp(config->scheduler,
			"edf") != 0)
	{
		printf("The last argument must be fifo or edf you entered: %s\n",
			config->scheduler);
		return (0);
	}
	return (1);
}
