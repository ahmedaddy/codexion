/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaddy <aaddy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 12:26:53 by aaddy             #+#    #+#             */
/*   Updated: 2026/07/20 16:30:39 by aaddy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define STATE_START 0
# define STATE_COMPILING 1
# define STATE_DEBUGGING 2
# define STATE_REFACTORING 3
# define STATE_BURNED_OUT 4

// Priority heap queue
typedef struct s_heap_node
{
	int					coder_id;
	long				deadline;
}						t_heap_node;

typedef struct s_priority_queue
{
	t_heap_node			*heap;
	int					size;
	int					capacity;
}						t_priority_queue;

typedef struct s_config
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	char				*scheduler;
}						t_config;

typedef struct s_dongle
{
	int					id;
	int					owner_id;
	int					available;
	long				cooldown_until;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	t_priority_queue	*request_queue;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	long				last_compile_start;
	pthread_t			thread;
	int					compile_count;
	int					state;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	pthread_mutex_t		lock;
}						t_coder;

typedef struct s_simulation
{
	t_config			config;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			monitor_thread;
	long				start_time;
	int					running;
	pthread_mutex_t		log_lock;
	pthread_mutex_t		sim_lock;
}						t_sim;

typedef struct s_thread_args
{
	t_sim				*sim;
	t_coder				*coder;
}						t_thread_args;

// Parsing
int						parsing(int ac, char *av[], t_config *config);

// Simulation
int						init_sim(t_sim *sim);
void					*coder_routine(void *args);
void					clean_simulation(t_sim *sim);

// utils
long					get_current_time_ms(void);
void					log_message(t_sim *sim, t_coder *coder, char *message);
void					print_requests_queue(t_dongle *dongle);
void					clean_pq(t_priority_queue *pq);

// coder
int						get_left_dongle(int coder_id);
int						get_righ_dongle(int coder_id, int numbers_of_coders);

// priority queue (heap)
t_priority_queue		*create_pq_queue(int capacity);
void					pq_push(t_priority_queue *pq, int coder_id,
							long deadline);
int						pq_pop(t_priority_queue *pq);

// dongles
int						take_dongles(t_sim *sim, t_coder *coder);
void					release_both_dongles(t_sim *sim, t_coder *coder);

// monitor
void					*monitor_routine(void *arg);

#endif