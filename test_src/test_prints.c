/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_prints.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 20:23:19 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/25 12:58:46 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	test_print_elements(void);
void	test_print_time(void);
void	test_print_fork_tags();
void	test_print_logs();
void	test_print_mutex_errors();


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

void	test_print_time(void)
{
	t_df	*df;

	// keep_time();
	df = get_df();

	printf("Abs df-> start time in milliseconds:	%ld\n", df->start_time);
	printf("Sim start time in seconds:		%ld\n", get_sim_time(1));
	printf("Sim start time in milliseconds:	%ld\n", get_sim_time(2));
	printf("Sim start time in microseconds:	%ld\n", get_sim_time(3));
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
		log_event(philos + (i % df->total_philos), i % 6);
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