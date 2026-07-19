/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:58:47 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/19 17:23:10 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int		get_left_dongle(int coder_id)
{
	return (coder_id - 1);
}

int		get_righ_dongle(int coder_id, int numbers_of_coders)
{
	return (coder_id % numbers_of_coders);
}

long 	get_request_key(t_sim *sim, t_coder *coder)
{
	if (strcmp(sim->config.scheduler, "fifo") != 0)
		return get_current_time_ms();
	return (coder->last_compile_start + sim->config.time_to_burnout);
}

int		dongle_take(t_sim *sim, t_coder *coder, t_dongle *dongle)
{
	long	key;

	key = get_request_key(sim, coder);
	pthread_mutex_lock(&dongle->lock);
	pq_push(dongle->request_queue, coder->id, key);
	while (sim->running &&
			(
				!dongle->available ||
				dongle->request_queue->heap[0].coder_id != 	coder->id ||
				dongle->cooldown_until > get_current_time_ms()
			)
	)
	{
		// pthread_mutex_unlock(&dongle->lock);
		// usleep(1000);
		// pthread_mutex_lock(&dongle->lock);
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	pq_pop(dongle->request_queue);
	dongle->owner_id = coder->id;
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->lock);
	log_message(sim, coder, "has taken a dongle");
	return 1;
}

void	release_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->available = 1;
	dongle->owner_id = -1;
	dongle->cooldown_until = get_current_time_ms() + sim->config.dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);

}

int		take_dongles(t_sim *sim, t_coder *coder)
{
	if (!dongle_take(sim, coder, coder->left_dongle))
		return (0);
	if (!dongle_take(sim, coder, coder->right_dongle))
	{
		release_dongle(sim, coder->left_dongle);
		return (0);
	}
	return (1);
}

void	release_both_dongles(t_sim *sim, t_coder *coder)
{
	release_dongle(sim, coder->right_dongle);
	release_dongle(sim, coder->left_dongle);
}