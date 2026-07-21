/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:58:42 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/21 17:59:24 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(long start_time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time);
}

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	log_message(t_sim *sim, t_coder *coder, char *message)
{
	long elapsed_time;

	pthread_mutex_lock(&sim->log_lock);
	elapsed_time = get_time_ms(sim->start_time);
	printf("%lu %d %s\n", elapsed_time, coder->id, message);
	pthread_mutex_unlock(&sim->log_lock);
}

int	sim_runnning(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->sim_lock);
	i = sim->running;
	pthread_mutex_unlock(&sim->sim_lock);
	return (i);
}