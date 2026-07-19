/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:58:52 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/19 14:18:37 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char *av[])
{
	t_thread_args	*thread_args;
	t_sim			sim;
	int				i;

	i = 0;
	memset(&sim, 0, sizeof(t_sim));
	// printf("%lu, %lu\n", sizeof(config), sizeof(t_config)); same size
	if (!parsing(ac, av, &sim.config))
		return (1);
	if (!init_sim(&sim))
		return (1);
	thread_args = malloc(sizeof(t_thread_args) * sim.config.number_of_coders);
	if (!thread_args)
	{
		clean_simulation(&sim);
		return (1);
	}
    if (pthread_create(&sim.monitor_thread, NULL, monitor_routine, &sim))
	{
		printf("Failed to create monitor thread!!!!\n");
		return (1);
	}

	while (i < sim.config.number_of_coders)
	{
		thread_args[i].coder = &sim.coders[i];
		thread_args[i].sim = &sim;
		if (pthread_create(&sim.coders[i].thread, NULL, coder_routine, &thread_args[i]) != 0)
		{
			printf("Failed to create threads!!!!\n");
			clean_simulation(&sim);
			free(thread_args);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < sim.config.number_of_coders)
	{
		if (pthread_join(sim.coders[i].thread, NULL))
		{
			printf("Failed to join threads!!!!\n");
			clean_simulation(&sim);
			free(thread_args);
			return (1);
		}
		i++;
	}
	// printf("%d\n", sim.config.number_of_compiles_required);
}
