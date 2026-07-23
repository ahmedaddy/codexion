/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:58:47 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/23 16:22:43 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <time.h>

int	get_left_dongle(int coder_id)
{
	return (coder_id - 1);
}

int	get_right_dongle(int coder_id, int numbers_of_coders)
{
	return (coder_id % numbers_of_coders);
}

long	get_request_key(t_sim *sim, t_coder *coder)
{
	if (strcmp(sim->config.scheduler, "fifo") == 0)
		return (get_current_time_ms());
	return (coder->last_compile_start + sim->config.time_to_burnout);
}

int	take_both_dangles(t_sim *sim, t_coder *coder, t_dongle *first, t_dongle *second)
{
	long	key;

	key = get_request_key(sim, coder);
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
	pq_push(first->request_queue, coder->id, key);
	pq_push(second->request_queue, coder->id, key);
	while (sim_runnning(sim) && (!(first->available && second->available)
		|| (first->request_queue->heap[0].coder_id != coder->id 
		|| second->request_queue->heap[0].coder_id != coder->id)
		|| (get_current_time_ms() < first->cooldown_until
		|| get_current_time_ms() < second->cooldown_until)
	))
	{
		pthread_mutex_unlock(&first->lock);
		pthread_mutex_unlock(&second->lock);
		usleep(100);
		pthread_mutex_lock(&first->lock);
		pthread_mutex_lock(&second->lock);	
	}
	if (!sim_runnning(sim))
	{
		pq_pop(first->request_queue);
		pq_pop(second->request_queue);
		pthread_mutex_unlock(&first->lock);
		pthread_mutex_unlock(&second->lock);
		return (0);
	}
	first->owner_id = coder->id;
	first->available = 0;
	second->owner_id = coder->id;
	second->available = 0;
	log_message(sim, coder, "has taken a dongle");
	log_message(sim, coder, "has taken a dongle");
	pq_pop(first->request_queue);
	pq_pop(second->request_queue);
	pthread_mutex_unlock(&first->lock);
	pthread_mutex_unlock(&second->lock);
	return (1);
}

void	release_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->available = 1;
	dongle->owner_id = -1;
	dongle->cooldown_until = get_current_time_ms()
		+ sim->config.dongle_cooldown;
	pthread_mutex_unlock(&dongle->lock);
}

int	take_dongles(t_sim *sim, t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (first == second)
	{
		pthread_mutex_lock(&first->lock);
		first->available = 0;
		first->owner_id = coder->id;

		log_message(sim, coder, "has taken a dongle");

		while (sim_runnning(sim))
			usleep(1000);

		first->available = 1;
		first->owner_id = -1;
		pthread_mutex_unlock(&first->lock);
		return (0);
	}
	if (!take_both_dangles(sim, coder, first, second))
		return (0);
	return (1);
}

void	release_both_dongles(t_sim *sim, t_coder *coder)
{
	release_dongle(sim, coder->right_dongle);
	release_dongle(sim, coder->left_dongle);
}