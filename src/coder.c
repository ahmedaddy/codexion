/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:59:01 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/23 17:05:09 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compiling(t_sim *sim, t_coder *coder)
{
	if (!take_dongles(sim, coder))
		return ;
	if (coder->right_dongle->owner_id != coder->id
		|| coder->left_dongle->owner_id != coder->id)
	{
		release_both_dongles(sim, coder);
		printf("The owner of this dongle is not the coder who is compiling\n");
		pthread_mutex_lock(&sim->sim_lock);
		sim->running = 0;
		pthread_mutex_unlock(&sim->sim_lock);
		return ;
	}
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = get_current_time_ms();
	coder->state = STATE_COMPILING;
	pthread_mutex_unlock(&coder->lock);
	log_message(sim, coder, "is compiling");
	usleep(sim->config.time_to_compile * 1000);
	release_both_dongles(sim, coder);
	pthread_mutex_lock(&coder->lock);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);
}

void	debeugging(t_sim *sim, t_coder *coder)
{
	log_message(sim, coder, "is debugging");
	pthread_mutex_lock(&coder->lock);
	coder->state = STATE_DEBUGGING;
	pthread_mutex_unlock(&coder->lock);
	usleep(sim->config.time_to_debug * 1000);
}

void	refactoring(t_sim *sim, t_coder *coder)
{
	log_message(sim, coder, "is refactoring");
	pthread_mutex_lock(&coder->lock);
	coder->state = STATE_REFACTORING;
	pthread_mutex_unlock(&coder->lock);
	usleep(sim->config.time_to_refactor * 1000);
}

int	run_debu_refac(t_sim *sim, t_coder *coder)
{
	debeugging(sim, coder);
	if (!sim_runnning(sim))
		return (0);
	refactoring(sim, coder);
	if (!sim_runnning(sim))
		return (0);
	return (1);
}

void	*coder_routine(void *args)
{
	t_thread_args	*thread_args;
	t_sim			*sim;
	t_coder			*coder;

	thread_args = (t_thread_args *)args;
	sim = thread_args->sim;
	coder = thread_args->coder;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		compiling(sim, coder);
		if (!sim_runnning(sim))
			break ;
		if (!run_debu_refac(sim, coder))
			break ;
		pthread_mutex_lock(&sim->sim_lock);
		if (sim->config.number_of_compiles_required > 0
			&& coder->compile_count >= sim->config.number_of_compiles_required)
		{
			pthread_mutex_unlock(&sim->sim_lock);
			break ;
		}
		pthread_mutex_unlock(&sim->sim_lock);
	}
	sim = thread_args->sim;
	coder = thread_args->coder;
	return (NULL);
}
