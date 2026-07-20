/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:58:42 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/18 15:28:15 by aaddy            ###   ########.fr       */
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

	elapsed_time = get_time_ms(sim->start_time);
	pthread_mutex_lock(&sim->log_lock);
	printf("%lu %d %s\n", elapsed_time, coder->id, message);
	pthread_mutex_unlock(&sim->log_lock);
}