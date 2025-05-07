/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:18:58 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/07 16:17:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		philo_eat(t_df *df, t_phil *philo);
int		philo_sleep(t_df *df, t_phil *philo);
int		philo_think(t_df *df, t_phil *philo, bool first_think);
bool	philo_should_exit(t_df *df, t_phil *philo, e_check check);

// static int	philo_pickup_forks(t_phil *philo);
static int	philo_pickup_forks(t_df *df, t_phil *philo);
// static int	philo_drop_forks(t_phil *philo);
static int	philo_drop_forks(t_phil *philo, e_check drop);

static int	update_fork(t_fork *fork, t_phil *philo, e_fstates state);

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
	long	start_time;

	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);
	if (philo_pickup_forks(df, philo) < 0)
		return (SIM_COMPLETED);
	
	start_time = get_sim_time(MICRO);
	if (log_event_safe(philo, EATING) != 0)
		return (-1);
	
	set_long(&philo->mtx, &philo->lastmeal_time, get_sim_time(MICRO));
	set_int(&philo->mtx, &philo->state, EATING);
	if (ft_usleep(start_time, df->tte) == SIM_COMPLETED)
		return (printf("%d quitted here\n", philo->id), philo_drop_forks(philo, BOTH), SIM_COMPLETED);

	set_int(&philo->mtx, &philo->meals_left, DECREASE);
	set_int(&philo->mtx, &philo->state, -1);

	if (df->max_meals >= 0 && get_int(&philo->mtx, &philo->meals_left) == 0)
	{
		if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) == 0)
		{
			// if (philo_should_exit(df, philo, BOTH))
			// 	return (print_mutex_error(UNLOCK,
			// 			pthread_mutex_unlock(&df->mtx_write)), SIM_COMPLETED);
			set_bool(&philo->mtx, &philo->full, true);
			printf("%ld	%d is sleeping\n", get_sim_time(MILLI), philo->id);
			print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write));
		}
	}
	philo_drop_forks(philo, BOTH);
	return (0);
}

// Function to lock fork mutexes and update fork states
static int	philo_pickup_forks(t_df *df, t_phil *philo)
{
	pthread_mutex_lock(&df->mtx_write);
	printf(M"%ld	%d is trying for 1st fork[%d]\n"RST, get_sim_time(MILLI), philo->id, philo->fork1->id);
	
	// if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork1->mtx)) != 0)
	if (print_mutex_error_debug(philo, philo->fork1, LOCK, pthread_mutex_lock(&philo->fork1->mtx)) != 0)
		return (-1);
	
	printf(C"%ld	%d got the first fork (%d) and will now go for the 2nd\n"RST, get_sim_time(MILLI), philo->id, philo->fork1->id);	
	pthread_mutex_unlock(&df->mtx_write);
	
	
	if (log_event_safe(philo, TOOK_FORK_1) != 0)
		return (-1);
	update_fork(philo->fork1, philo, TAKEN);

	if (df->total_philos > 1)
	{
		if (philo_should_exit(df, philo, BOTH))
			return (printf("%d quitted over here\n", philo->id), philo_drop_forks(philo, ONE), SIM_COMPLETED);

		pthread_mutex_lock(&df->mtx_write);
		printf(Y"%ld	%d is trying for 2nd fork[%d]\n"RST, get_sim_time(MILLI), philo->id, philo->fork2->id);
		
		// if (print_mutex_error(LOCK, pthread_mutex_lock(&philo->fork2->mtx)) != 0)
		if (print_mutex_error_debug(philo, philo->fork2, LOCK, pthread_mutex_lock(&philo->fork2->mtx)) != 0)
			return (-1);
			
		printf(C"%ld	%d got the 2nd fork (%d) and will now print that it has taken a fork...\n"RST, get_sim_time(MILLI), philo->id, philo->fork2->id);	
		pthread_mutex_unlock(&df->mtx_write);
		
		
		if (log_event_safe(philo, TOOK_FORK_2) != 0)
			return (-1);
		update_fork(philo->fork2, philo, TAKEN);
	}
	else
		return (philo_drop_forks(philo, ONE), SIM_COMPLETED);
	if (philo_should_exit(df, philo, BOTH))
		return (philo_drop_forks(philo, BOTH), SIM_COMPLETED);
	return (0);
}

// Function to unlock fork mutexes and update fork states
static int	philo_drop_forks(t_phil *philo, e_check drop)
{
	if (drop == ONE)
	{
		update_fork(philo->fork1, philo, FREE);
		if (print_mutex_error_debug(philo, philo->fork1, UNLOCK,
				pthread_mutex_unlock(&philo->fork1->mtx)) != 0)
			return (-1);
		// if (print_mutex_error(UNLOCK,
		// 		pthread_mutex_unlock(&philo->fork1->mtx)) != 0)
		// 	return (-1);
	}
	else if (drop == TWO)
	{
		update_fork(philo->fork2, philo, FREE);
		if (print_mutex_error_debug(philo, philo->fork2, UNLOCK,
				pthread_mutex_unlock(&philo->fork2->mtx)) != 0)
			return (-1);	
		// if (print_mutex_error(UNLOCK,
		// 		pthread_mutex_unlock(&philo->fork2->mtx)) != 0)
		// 	return (-1);	
	}
	else if (drop == BOTH)
	{
		update_fork(philo->fork1, philo, FREE);
		if (print_mutex_error_debug(philo, philo->fork1, UNLOCK,
				pthread_mutex_unlock(&philo->fork1->mtx)) != 0)
			return (-1);
		// if (print_mutex_error(UNLOCK,
		// 		pthread_mutex_unlock(&philo->fork1->mtx)) != 0)
		// 	return (-1);
		update_fork(philo->fork2, philo, FREE);
		if (print_mutex_error_debug(philo, philo->fork2, UNLOCK,
				pthread_mutex_unlock(&philo->fork2->mtx)) != 0)
			return (-1);
		// if (print_mutex_error(UNLOCK,
		// 		pthread_mutex_unlock(&philo->fork2->mtx)) != 0)
		// 	return (-1);
	}
	return (0);
}

// Enter Sandman function
int	philo_sleep(t_df *df, t_phil *philo)
{
	long	start_time;
	
	start_time = get_sim_time(MICRO);
	if (log_event_safe(philo, SLEEPING) < 0)
		return (-1);
	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);
	
	philo->state = SLEEPING;
	ft_usleep(start_time, df->tts);
	philo->state = -1;
	return (0);
}

int	philo_think(t_df *df, t_phil *philo, bool first_think)
{
	// long tte, tts, ttt;
	// long	start_time;

	(void)first_think;
	// start_time = get_abs_time(MICRO);
	// start_time = get_sim_time(MICRO);
	// printf("%ld	%d should start thinking now\n", start_time, philo->id);
	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);
	if (log_event_safe(philo, THINKING) < 0)
		return (-1);
	
	// if (df->total_philos % 2 == 0)
	// 	return (0);
	// tte = df->tte;
	// tts = df->tts;
	// ttt = tte * 2 - tts;
	// if (ttt < 0)
	// 	ttt = 0;
		
	// printf("%ld\t\t\t %d is going to sleep for %ld (usec) while thinking\n", get_sim_time(MILLI), philo->id, ttt);
	// ft_usleep(start_time, 0.42 * ttt);
	
	// ft_usleep(start_time, 100);
	
	// ft_usleep2(start_time, 0.42 * ttt, philo);
	// printf("%ld\t\t\t %d just woke up after sleeping %ld (usec) while thinking and now ready to compete for forks\n", get_sim_time(MILLI), philo->id, ttt);
	return (0);
}

static int	update_fork(t_fork *fork, t_phil *philo, e_fstates state)
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
	return (0);
}

bool	philo_should_exit(t_df *df, t_phil *philo, e_check check)
{
	// if (check == PHILO)
	if (philo && check == PHILO)
	{
		if (get_bool(&philo->mtx, &philo->full) == true)
			return (true);
	}
	// else if (check == SIMULATION)
	else if (df && check == SIMULATION)
	{
		if (get_bool(&df->mtx, &df->sim_finished))
			return (true);	
	}
	// else if (check == BOTH)
	else if (df && philo && check == BOTH)
	{
		if (get_bool(&df->mtx, &df->sim_finished)
				|| get_bool(&philo->mtx, &philo->full) == true)
			return (true);
	}
	return (false);
}
