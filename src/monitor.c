/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 12:22:07 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/20 18:34:24 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_sim *sim;
	long current_time;
	int i;
	int all_finished;

	sim = (t_sim *)arg;

	while (1)
	{
		pthread_mutex_lock(&sim->sim_lock);
		if (!sim->running)
		{
			pthread_mutex_unlock(&sim->sim_lock);
			break ;
		}

		pthread_mutex_unlock(&sim->sim_lock);
		current_time = get_current_time_ms();
		all_finished = 1;
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			if (sim->config.number_of_compiles_required > 0
				&& sim->coders[i].compile_count < sim->config.number_of_compiles_required)
				all_finished = 0;
			pthread_mutex_lock(&sim->coders[i].lock);
			if (sim->coders[i].state != STATE_COMPILING && current_time
				- sim->coders[i].last_compile_start > sim->config.time_to_burnout)
			{
				sim->coders[i].state = STATE_BURNED_OUT;
				pthread_mutex_unlock(&sim->coders[i].lock);
				// usleep(10000);
				log_message(sim, &sim->coders[i], "burned out");
				pthread_mutex_lock(&sim->sim_lock);
				sim->running = 0;
				pthread_mutex_unlock(&sim->sim_lock);
				i = 0;
				while (i < sim->config.number_of_coders)
				{
					pthread_mutex_lock(&sim->dongles[i].lock);
					pthread_cond_broadcast(&sim->dongles[i].cond);
					pthread_mutex_unlock(&sim->dongles[i].lock);

					i++;
				}
				return (NULL);
			}
			pthread_mutex_unlock(&sim->coders[i].lock);
			i++;
		}
		if (sim->config.number_of_compiles_required > 0 && all_finished)
		{
			pthread_mutex_lock(&sim->sim_lock);
			sim->running = 0;
			pthread_mutex_unlock(&sim->sim_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}