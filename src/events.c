/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:18:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/30 23:16:25 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		philo_eat(t_df *df, t_phil *philo);
int		philo_sleep(t_df *df, t_phil *philo);
int		philo_think(t_df *df, t_phil *philo);

static int	philo_pickup_forks(t_phil *philo);
static int	philo_drop_forks(t_phil *philo);
static void	update_fork(t_fork *fork, t_phil *philo, e_fstates state);

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
int	philo_eat(t_df *df, t_phil *philo)
{	
	// test_print_philo_presence(philo);
	philo_pickup_forks(philo);
	if (log_event(philo, EATING) != 0)
		return (-1);
	if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->mtx)) != 0)
		return (-1);
	printf("Its stuck here on philo no. %d\n", philo->id);
	philo->lastmeal_time = get_sim_time(MICRO);
	philo->state = EATING;
	usleep(df->tte);
	// philo->meals_left--;
	set_int(&philo->mtx, &philo->meals_left, DECREASE);
	philo->state = -1;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&philo->mtx)) != 0)
		return (-1);
	philo_drop_forks(philo);
	return (0);
}

// Function to lock fork mutexes and update fork states
static int	philo_pickup_forks(t_phil *philo)
{
	if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork1->mtx)) != 0)
		return (-1);
	// test_print_safe_mutex_destruction();	
	// if (log_event(philo, TOOK_FORK_1) != 0)
	// 	return (-1);
	update_fork(philo->fork1, philo, TAKEN);
	
	if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork2->mtx)) != 0)
		return (-1);
	// if (log_event(philo, TOOK_FORK_2) != 0)
	// 	return (-1);
	update_fork(philo->fork2, philo, TAKEN);

	// test_print_philo_presence(philo);	// TPF
	return (0);
}

// Function to unlock fork mutexes and update fork states
static int	philo_drop_forks(t_phil *philo)
{
	update_fork(philo->fork1, philo, FREE);
	if (print_mutex_error(UNLOCK,
			pthread_mutex_unlock(&philo->fork1->mtx)) != 0)
		return (-1);
	update_fork(philo->fork2, philo, FREE);
	if (print_mutex_error(UNLOCK,
			pthread_mutex_unlock(&philo->fork2->mtx)) != 0)
		return (-1);
	return (0);
}

// Enter Sandman function
int	philo_sleep(t_df *df, t_phil *philo)
{
	if (log_event(philo, SLEEPING) < 0)
		return (-1);
	philo->state = SLEEPING;
	usleep(df->tts);
	philo->state = -1;
	return (0);
}

int	philo_think(t_df *df, t_phil *philo)
{
	(void)df;
	if (log_event(philo, THINKING) < 0)
		return (-1);
	return (0);
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
