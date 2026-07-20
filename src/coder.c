/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:59:01 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/20 19:56:33 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	compiling(t_sim *sim, t_coder *coder)
{
	if (!take_dongles(sim, coder))
		return ;
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = get_current_time_ms();
	coder->state = STATE_COMPILING;
	pthread_mutex_unlock(&coder->lock);
	// pthread_mutex_lock(&sim->log_lock);
	// coder->right_dongle->owner_id = 5;
	if (coder->right_dongle->owner_id != coder->id
		|| coder->left_dongle->owner_id != coder->id
	)
	{
		printf("The owner of this dongle is not the coder who is compiling\n");
		pthread_mutex_lock(&sim->sim_lock);
		sim->running = 0;
		pthread_mutex_unlock(&sim->sim_lock);
		return ;
	}
	log_message(sim, coder, "is compiling");

	// printf("Right dongle owner: %d\n", coder->right_dongle->owner_id);
	// printf("Left dongle owner: %d\n", coder->left_dongle->owner_id);

	// pthread_mutex_unlock(&sim->log_lock);
	usleep(sim->config.time_to_compile * 1000);
	pthread_mutex_lock(&coder->lock);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);
	release_both_dongles(sim, coder);
}

void	debeugging(t_sim *sim, t_coder *coder)
{
	log_message(sim, coder, "is debugging");
	coder->state = STATE_DEBUGGING;
	usleep(sim->config.time_to_debug * 1000);
}

void	refactoring(t_sim *sim, t_coder *coder)
{
	log_message(sim, coder, "is refactoring");
	coder->state = STATE_REFACTORING;
	usleep(sim->config.time_to_refactor * 1000);
}

void	*coder_routine(void *args)
{
	t_thread_args	*thread_args;
	t_sim			*sim;
	t_coder			*coder;

	thread_args = (t_thread_args *) args;
	sim = thread_args->sim;
	coder = thread_args->coder;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		compiling(sim, coder);
		if (!sim->running)
			break ;
		debeugging(sim, coder);
		if (!sim->running)
			break ;
		refactoring(sim, coder);
		if (!sim->running)
			break ;
		if (sim->config.number_of_compiles_required > 0
			&& coder->compile_count >= sim->config.number_of_compiles_required)
			break ;
	}
	sim = thread_args->sim;
	coder = thread_args->coder;
	return (NULL);
}