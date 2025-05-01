/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:09:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/01 17:13:11 by pamatya          ###   ########.fr       */
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
			df->mtx_write_init = false;
			df->turn = ODD_PHILOS;
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
		.last_philo = false,
		.dead = false
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
		.taker_id = 0
	};
}
