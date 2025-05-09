/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_df.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:13 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 23:05:34 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int			init_df(int ac, char **av);
int			parse_arguments(int ac, char **av, t_df *df);
int			init_philos(t_df *df);
int			init_forks(t_df *df);

static void	tag_forks(t_phil *philo);

// int	init_df(int ac, char **av, t_df *df)
int	init_df(int ac, char **av)
{
	t_df	*df;

	df = get_df();
	if (parse_arguments(ac, av, df) < 0)
		return (-1);
	if (init_forks(df) < 0)
		return (-1);
	if (init_philos(df) < 0)
		return (-1);
	if (print_mutex_error(INIT, pthread_mutex_init(&df->mtx, NULL)))
		return (-1);
	df->mtx_init = true;
	if (print_mutex_error(INIT, pthread_mutex_init(&df->mtx_write, NULL)))
		return (-1);
	df->mtx_write_init = true;
	return (0);
}

/*
Function to import the arguments input by the user, parse them and assign them
to the appropriate elements in the dataframe
  - Input format:	./philo   5    800   200   200   [10]
	 				  av[0] av[1] av[2] av[3] av[4] (av[5])
  - av[0] = "./philo"
  - av[1] = total number of philosophers for the simulation
  - av[2] = time to die
  - av[3] = time to eat
  - av[4] = time to sleep
  - av[5] = no. of times each philosopher must eat
  - ac = 6 (including the optional parameter)
*/
int	parse_arguments(int ac, char **av, t_df *df)
{
	df->total_philos = ft_atol_tailored(av[1]);
	df->ttd = ft_atol_tailored(av[2]) * 1000;
	df->tte = ft_atol_tailored(av[3]) * 1000;
	df->tts = ft_atol_tailored(av[4]) * 1000;
	if (df->total_philos < 0 || df->ttd < 0 || df->tte < 0 || df->tts < 0)
		return (-1);
	if (ac == 6)
		df->max_meals = ft_atol_tailored(av[5]);
	if (ac == 6 && df->max_meals < 0)
		return (-1);
	return (0);
}

/*
Function to init all forks in the forks array
	- fork_id starts at 0 and onwards...
*/
int	init_forks(t_df *df)
{
	int		i;

	i = -1;
	df->forks = malloc(sizeof(t_fork) * df->total_philos);
	if (!df->forks)
		return (-1);
	while (++i < df->total_philos)
	{
		spawn_fork(df->forks + i);
		(df->forks + i)->id = i;
		if (pthread_mutex_init(&(df->forks + i)->mtx, NULL) < 0)
			return (-1);
		(df->forks + i)->mtx_init = true;
	}
	return (0);
}

/*
Function to init all the philos in the philos array
	- philo_id starts at 1 and onwards...
*/
int	init_philos(t_df *df)
{
	int		i;

	df->philos = malloc(sizeof(t_phil) * df->total_philos);
	if (!df->philos)
		return (-1);
	i = -1;
	while (++i < df->total_philos)
	{
		spawn_philo(df->philos + i);
		(df->philos + i)->id = i + 1;
		if (pthread_mutex_init(&(df->philos + i)->mtx, NULL) < 0)
			return (-1);
		(df->philos + i)->mtx_init = true;
		tag_forks(df->philos + i);
		(df->philos + i)->meals_left = df->max_meals;
	}
	return (0);
}

/*
Function to tag the forks for the provided philo that it should be able to lock
when it is time to pick up the forks
*/
static void	tag_forks(t_phil *philo)
{
	t_df	*df;
	t_fork	*forks;

	df = get_df();
	forks = df->forks;
	if (philo->id % 2)
	{
		philo->fork1 = forks + ((philo->id - 1) % df->total_philos);
		philo->fork2 = forks + (philo->id % df->total_philos);
	}
	else
	{
		philo->fork1 = forks + (philo->id % df->total_philos);
		philo->fork2 = forks + ((philo->id - 1) % df->total_philos);
	}
}

// static void	tag_forks(t_phil *philo)
// {
// 	t_df	*df;
// 	t_fork	*forks;

// 	df = get_df();
// 	forks = df->forks;
// 	philo->fork1 = forks + (philo->id % df->total_philos);
// 	philo->fork2 = forks + ((philo->id - 1) % df->total_philos);
// }
