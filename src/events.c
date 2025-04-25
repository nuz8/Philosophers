/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:18:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/25 14:58:25 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	philo_pickup_forks();
void	philo_eat(t_df *df, t_phil *philo);
void	philo_sleep(t_df *df, t_phil *philo);
void	philo_think(t_df *df, t_phil *philo);

static void	update_fork(t_fork *fork, t_phil *philo, e_fstates state);


/*
Events to be created:
eat()
sleep()
think()
*/

/*
Function to simulate eating
	- Lock fork1 (only for odd philos or even philos depending on the turn)
	- Update fork fields
	- Lock fork2
	- Update fork fields
	- Write the log for eating			// might need a mutex for this too
	- Update philo fields to indicate eating state
	- Update no. of meals left to be eaten by the philo (meals_left--)
*/
void	philo_eat(t_df *df, t_phil *philo)
{
	pthread_mutex_lock(&philo->fork1->mtx);
	update_fork(&philo->fork1, philo, TAKEN);
	pthread_mutex_lock(&philo->fork2->mtx);
	update_fork(&philo->fork2, philo, TAKEN);
	
	pthread_mutex_lock(&df->mtx);
	log_event(philo, EATING);
	pthread_mutex_unlock(&df->mtx);

	pthread_mutex_lock(&philo->mtx);
	philo->lastmeal_time = get_sim_time(2);
	philo->state = EATING;
	usleep(df->tte);
	philo->meals_left--;
	philo->state = -1;
	pthread_mutex_unlock(&philo->mtx);
	
	update_fork(&philo->fork1, philo, FREE);
	pthread_mutex_unlock(&philo->fork1->mtx);
	update_fork(&philo->fork2, philo, FREE);
	pthread_mutex_unlock(&philo->fork2->mtx);
}

void	philo_pickup_forks()
{
	// Here sits the algorithm for odd and even philo fork pick-ups
	
}

void	philo_sleep(t_df *df, t_phil *philo)
{
	philo->state = SLEEPING;
	usleep(df->tts);
	philo->state = -1;
}

void	philo_think(t_df *df, t_phil *philo)
{
	
}

static void	update_fork(t_fork *fork, t_phil *philo, e_fstates state)
{
	if (state == TAKEN)
	{
		fork->state = TAKEN;
		fork->taker_id = philo->id;
	}
	else if (state == FREE)
	{
		fork->state = FREE;
		fork->taker_id = 0;
	}
}
