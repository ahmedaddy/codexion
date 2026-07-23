/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:58:46 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/22 20:05:05 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_simulation(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		clean_pq(sim->dongles[i].request_queue);
		pthread_mutex_destroy(&sim->coders[i].lock);
		pthread_mutex_destroy(&sim->dongles[i].lock);
		i++;
	}
	pthread_mutex_destroy(&sim->log_lock);
	pthread_mutex_destroy(&sim->sim_lock);
	free(sim->dongles);
	free(sim->coders);
}

int	init_sim(t_sim *sim)
{
	int i;

	i = 0;
	sim->dongles = malloc(sizeof(t_dongle) * sim->config.number_of_coders);
	if (!sim->dongles)
		return (0);
	sim->coders = malloc(sizeof(t_coder) * sim->config.number_of_coders);
	if (!sim->coders)
		return (0);
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->config.number_of_coders);
	memset(sim->coders, 0, sizeof(t_coder) * sim->config.number_of_coders);
	// dongles
	while (i < sim->config.number_of_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].available = 1;
		sim->dongles[i].owner_id = -1;
		sim->dongles[i].cooldown_until = 0;
		sim->dongles[i].request_queue = create_pq_queue(2);
		pthread_mutex_init(&sim->dongles[i].lock, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		i++;
	}
	// coders
	i = 0;
	sim->running = 1;
	sim->start_time = get_current_time_ms();

	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].state = STATE_START;
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].left_dongle = &sim->dongles[get_left_dongle(i + 1)];
		sim->coders[i].right_dongle = &sim->dongles[get_right_dongle(i + 1,
				sim->config.number_of_coders)];
		i++;
	}
	pthread_mutex_init(&sim->log_lock, NULL);
	pthread_mutex_init(&sim->sim_lock, NULL);

	return (1);
}