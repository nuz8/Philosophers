/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:09:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/22 23:18:05 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_df	*get_df(void);
void	spawn_philo(t_phil *philo);
void	spawn_fork(t_fork *fork);

// Function to get the pointer to the dataframe struct (t_df *)
t_df	*get_df(void)
{
	static t_df	*df = NULL;

	if (!df)
	{
		df = malloc(sizeof(t_df));
		if (df)
		{
			df->max_meals = -1;
			df->forks = NULL;
			df->philos = NULL;
			df->mtx_init = false;
			df->sim_finished = false;
		}
	}
	return (df);
}

/*
Function to spawn each philo in the philos array with their initial default
field states
*/
void	spawn_philo(t_phil *philo)
{
	*philo = (t_phil){
		.mtx_init = false,
		.state = -1,
		.fork1 = NULL,
		.fork2 = NULL,
		.eat_count = 0,
		.last_phil = false
	};
}

/*
Function to spawn each fork in the forks array with their initial default field
states
*/
void	spawn_fork(t_fork *fork)
{
	*(fork) = (t_fork){
		.mtx_init = false,
		.state = FREE,
		.with_left = false,
		.with_right = false,
		.taker_id = 0
	};
}