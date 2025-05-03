/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:18:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/02 21:21:34 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		philo_eat(t_df *df, t_phil *philo);
int		philo_sleep(t_df *df, t_phil *philo);
int		philo_think(t_df *df, t_phil *philo);

// static int	philo_pickup_forks(t_phil *philo);
static int	philo_pickup_forks(t_df *df, t_phil *philo);
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
	if (get_bool(&df->mtx, &df->sim_finished))
		return (0);
	philo_pickup_forks(df, philo);
	if (log_event_safe_debug(philo, EATING) != 0)
		return (-1);

	// printf("Its stuck here on philo no. %d\n", philo->id);

	set_long(&philo->mtx, &philo->lastmeal_time, get_sim_time(MICRO));
	set_int(&philo->mtx, &philo->state, EATING);
	ft_usleep(df->tte);
	
	set_int(&philo->mtx, &philo->meals_left, DECREASE);
	set_int(&philo->mtx, &philo->state, -1);
	
	if (df->max_meals >= 0 && get_int(&philo->mtx, &philo->meals_left) == 0)
		set_bool(&philo->mtx, &philo->full, true);
	
	philo_drop_forks(philo);
	return (0);
}

// Function to lock fork mutexes and update fork states
static int	philo_pickup_forks(t_df *df, t_phil *philo)
{
	while (get_int(&df->mtx_turn, &df->turn) != philo->id % 2)
		continue ;
	if (get_bool(&df->mtx, &df->sim_finished))
		return (0);
	if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork1->mtx)) != 0)
		return (-1);
	if (log_event_safe_debug(philo, TOOK_FORK_1) != 0)
		return (-1);
	update_fork(philo->fork1, philo, TAKEN);

	if (get_bool(&df->mtx, &df->sim_finished))
		return (0);
	if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork2->mtx)) != 0)
		return (-1);
	if (log_event_safe_debug(philo, TOOK_FORK_2) != 0)
		return (-1);
	update_fork(philo->fork2, philo, TAKEN);

	return (0);
}

// Function to unlock fork mutexes and update fork states
static int	philo_drop_forks(t_phil *philo)
{
	if (get_df()->sim_finished)
		return (0);
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
	if (get_bool(&df->mtx, &df->sim_finished))
		return (0);
	if (log_event_safe_debug(philo, SLEEPING) < 0)
		return (-1);
	philo->state = SLEEPING;
	ft_usleep(df->tts);
	philo->state = -1;
	return (0);
}

int	philo_think(t_df *df, t_phil *philo)
{
	if (get_bool(&df->mtx, &df->sim_finished))
		return (0);
	if (log_event_safe_debug(philo, THINKING) < 0)
		return (-1);
	return (0);
}

static void	update_fork(t_fork *fork, t_phil *philo, e_fstates state)
{
	if (get_df()->sim_finished)
		return ;
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
