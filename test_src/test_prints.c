/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_prints.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 20:23:19 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/08 01:55:56 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	test_print_elements(void);
void	keep_time(void);
void	test_print_time(void);
void	test_print_fork_tags();
void	test_print_logs();
void	test_print_mutex_errors();
void	test_print_fork_owners();
void	test_print_philo_presence(t_phil *philo);


void	test_print_elements(void)
{
	t_df	*df;
	
	df = get_df();
	printf("num of philos:	%ld\n", df->total_philos);
	printf("time to die:	%ld\n", df->ttd);
	printf("time to eat:	%ld\n", df->tte);
	printf("time to sleep:	%ld\n", df->tts);
	printf("maximum meals:	%ld\n", df->max_meals);

}

// Test function for gettimeofday() fn
void	keep_time(void)
{
	struct timeval	tv;
	long			sec;
	long			msec;
	long			usec;

	gettimeofday(&tv, NULL);
	sec = tv.tv_sec;
	usec = tv.tv_usec;
	printf("sec:	%ld\n", sec);
	printf("usec:	%ld\n", usec);

	sec = get_abs_time(SECOND);
	msec = get_abs_time(MILLI);
	usec = get_abs_time(MICRO);

	printf("get_abs_time sec:	%ld\n", sec);
	printf("get_abs_time msec:	%ld\n", msec);
	printf("get_abs_time usec:	%ld\n", usec);
}

void	test_print_time(void)
{
	t_df	*df;

	// keep_time();
	df = get_df();

	printf("Abs df-> start time in milliseconds:	%ld\n", df->start_time);
	printf("Sim start time in seconds:		%ld\n", get_sim_time(SECOND));
	printf("Sim start time in milliseconds:	%ld\n", get_sim_time(MILLI));
	printf("Sim start time in microseconds:	%ld\n", get_sim_time(MICRO));
}

void	test_print_fork_tags()
{
	t_df	*df;
	int		i;
	t_phil	*philos;
	
	df = get_df();
	i = -1;
	philos = df->philos;
	while (++i < df->total_philos)
	{
		printf("Philosopher no. "Y"%d"RST" has the following forks:\n", (philos + i)->id);
		printf("Fork-1: fork_id = "G"%d"RST"\n", (philos + i)->fork1->id);
		printf("Fork-2: fork_id = "G"%d"RST"\n", (philos + i)->fork2->id);
		printf("\n");
	}
}

void	test_print_logs()
{
	t_df	*df;
	t_phil	*philos;
	int		i;

	df = get_df();
	philos = df->philos;
	
	i = -1;
	while (++i < 500000)
	{
		// printf("i = %d:\t", i);
		log_event_safe(philos + (i % df->total_philos), i % 6);
	}
}

void	test_print_mutex_errors()
{
	print_mutex_error(INIT, EINVAL);
	print_mutex_error(INIT, ENOMEM);
	print_mutex_error(LOCK, EINVAL);
	print_mutex_error(LOCK, EDEADLK);
	print_mutex_error(UNLOCK, EPERM);
	print_mutex_error(DESTROY, EBUSY);
}

// Test fn to print the fork owners in the order that they were acquired
void	test_print_fork_owners()
{
	t_df	*df;
	int		i;
	int		printed;
	int		*indicator;

	df = get_df();
	
	indicator = malloc(df->total_philos * sizeof(int));
	if (!indicator)
		return ;

	printed = 0;
	i = -1;
	while (++i < df->total_philos)
		indicator[i] = 0;

	i = -1;
	while (++i < df->total_philos && printed != df->total_philos)
	{
		// if (indicator[i] == 0 && (df->forks + i)->taker_id > 0)
		if (indicator[i] == 0 && get_int(&(df->forks + i)->mtx, &(df->forks + i)->taker_id) != 0)
		{
			printf("%ld\tFork "G"%d"RST" is with Philo "Y"%d\n"RST, get_sim_time(MILLI), (df->forks + i)->id,
				(df->forks + i)->taker_id);
			// printf("%ld\tFork %d is with Philo %d\n", get_sim_time(MILLI), (df->forks + i)->id,
			// 	(df->forks + i)->taker_id);
			printed++;
			indicator[i] = 1;
		}
		// if (indicator[i] == 0 && (df->forks + i)->taker_id == 0)
		if (indicator[i] == 0 && get_int(&(df->forks + i)->mtx, &(df->forks + i)->taker_id) == 0)

		{
			printf("%ld\tFork "G"%d"RST" is "Y"free\n"RST, get_sim_time(MILLI), (df->forks + i)->id);
			printed++;
			indicator[i] = 1;
		}
		if (i >= (df->total_philos - 1) && printed != df->total_philos)
			i = -1;
	}
	free(indicator);
}

void	test_print_philo_presence(t_phil *philo)
{
	// t_df	*df;
	// int		i;

	// df = get_df();
	// i = -1;	
	// if (!philo)
	// {
	// 	while (++i < df->total_philos)
	// 	{
	// 		if ((df->philos + i)->id == (i + 1))
	// 			printf("Philo "Y"%d"RST" is here.\n", (df->philos + i)->id);
	// 	}
	// }
	// else
	printf("\t\t\tPhilo "Y"%d"RST" is here.\n", philo->id);
}

