/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loggers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:58:17 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/08 01:19:54 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	log_event_safe(t_phil *philo, e_phstates state);

int	log_event_safe_debug(t_phil *philo, e_phstates state);
int	log_event_unsafe(t_phil *philo, e_phstates state);

/*
Function to print logs for each event, indicating the current state of the
philosopher printing the log
	- Prints logs by locking/unlocking df->mtx_write
	- If successful, returns 0
	- If mutex errors are detected, then the corresponding error message is
	  printed by print_mutex_error() fn call and -1 is returned
*/
// int	log_event_safe(t_phil *philo, e_phstates state)
// {
// 	t_df	*df;
	
// 	df = get_df();
// 	if (philo_should_exit(df, philo, BOTH))
// 		return (SIM_COMPLETED);
// 	if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) != 0)
// 		return (-1);
// 	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
// 		printf("%ld	"Y"%d "C"has taken a fork"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == EATING)
// 		printf("%ld	"Y"%d "G"is eating"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == SLEEPING)
// 		printf("%ld	"Y"%d "B"is sleeping"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == THINKING)
// 		printf("%ld	"Y"%d "W"is thinking"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == DIED)
// 		printf("%ld	"Y"%d "R"died"RST"\n", get_sim_time(MILLI), philo->id);
// 	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write)) != 0)
// 		return (-1);
// 	return (0);
// }

// int	log_event_safe_debug(t_phil *philo, e_phstates state)
// {
// 	t_df	*df;
	
// 	df = get_df();
// 	if (philo_should_exit(df, philo, BOTH))
// 		return (SIM_COMPLETED);
// 	if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) != 0)
// 		return (-1);
// 	if (philo_should_exit(df, philo, BOTH))
// 	{
// 		print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write));
// 		return (SIM_COMPLETED);
// 	}
// 	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
// 		printf("%ld	"Y"%d "C"has taken a fork"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == EATING)
// 		printf("%ld	"Y"%d "G"is eating\t\t"M"Meals left for %d = %d\n"RST, get_sim_time(MILLI), philo->id, philo->id, get_int(&philo->mtx, &philo->meals_left) - 1);
// 	else if (state == SLEEPING)
// 		printf("%ld	"Y"%d "B"is sleeping"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == THINKING)
// 		printf("%ld	"Y"%d "W"is thinking"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == DIED)
// 		printf("%ld	"Y"%d "R"died"RST"\n", get_sim_time(MILLI), philo->id);
// 	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write)) != 0)
// 		return (-1);
// 	return (0);
// }

// // Same event logging fn as log_event_safe() fn but without mutex locks
// int	log_event_unsafe(t_phil *philo, e_phstates state)
// {
// 	t_df	*df;
	
// 	df = get_df();
// 	if (philo_should_exit(df, philo, BOTH))
// 		return (SIM_COMPLETED);
// 	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
// 		printf("%ld	%d "C"has taken a fork"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == EATING)
// 		printf("%ld	%d "G"is eating"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == SLEEPING)
// 		printf("%ld	%d "B"is sleeping"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == THINKING)
// 		printf("%ld	%d "W"is thinking"RST"\n", get_sim_time(MILLI), philo->id);
// 	else if (state == DIED)
// 		printf("%ld	%d "R"died"RST"\n", get_sim_time(MILLI), philo->id);
// 	return (0);
// }


/* ------------------------------- For linux ------------------------------- */

// log_event_safe() fn for linux
int	log_event_safe(t_phil *philo, e_phstates state)
{
	long	start_time;
	t_df	*df;
	
	start_time = get_sim_time(MILLI);
	df = get_df();
	if (philo_should_exit(df, philo, BOTH))
		return (SIM_COMPLETED);
	if (print_mutex_error(LOCK, pthread_mutex_lock(&df->mtx_write)) != 0)
		return (-1);
	if (philo_should_exit(df, philo, BOTH))
		return (print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write)),
				SIM_COMPLETED);
	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
		printf("%ld	%d has taken a fork\n", start_time, philo->id);
	else if (state == EATING)
		printf("%ld	%d is eating\n", start_time, philo->id);
	else if (state == SLEEPING)
		printf("%ld	%d is sleeping\n", start_time, philo->id);
	else if (state == THINKING)
		printf("%ld	%d is thinking\n", start_time, philo->id);
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(&df->mtx_write)) != 0)
		return (-1);
	return (0);
}


// // log_event_unsafe() fn for linux
// int	log_event_unsafe(t_phil *philo, e_phstates state)
// {
// 	t_df	*df;
	
// 	df = get_df();
// 	if (philo_should_exit(df, philo, BOTH))
// 		return (SIM_COMPLETED);
// 	if (state == TOOK_FORK_1 || state == TOOK_FORK_2)
// 		printf("%ld	%d has taken a fork\n", get_sim_time(MILLI), philo->id);
// 	else if (state == EATING)
// 		printf("%ld	%d is eating\n", get_sim_time(MILLI), philo->id);
// 	else if (state == SLEEPING)
// 		printf("%ld	%d is sleeping\n", get_sim_time(MILLI), philo->id);
// 	else if (state == THINKING)
// 		printf("%ld	%d is thinking\n", get_sim_time(MILLI), philo->id);
// 	else if (state == DIED)
// 		printf("%ld	%d died\n", get_sim_time(MILLI), philo->id);
// 	return (0);
// }
