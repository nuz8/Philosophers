/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_df.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:13 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/17 01:27:43 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_df	*get_df(void);
// int		init_df(int ac, char **av, t_df *df);
int		init_df(int ac, char **av);

// Function to get the pointer to the dataframe struct (t_df *)
t_df	*get_df(void)
{
	static t_df	*df = NULL;

	if (!df)
	{
		df = malloc(sizeof(t_df));
		if (df)
		{
			df->philos = NULL;
			df->forks = NULL;	
			df->mtx_initialized = false;	
		}
	}
	return (df);
}

// int	init_df(int ac, char **av, t_df *df)
int	init_df(int ac, char **av)
{
	t_df	*df;

	df = get_df();
	if (parse_arguments(ac, av, df) < 0)
		return (-1);
	
	df->philos = malloc(sizeof(t_phil) * df->total_philos);
	if (!df->philos)
		return (-1);
	init_philos(df->philos);									// TODO

	df->forks = malloc(sizeof(t_fork) * df->total_philos);
	if (!df->forks)
		return (clear_out(df, 0), -1);
	init_forks(df->forks);										// TODO
	
	if (pthread_mutex_init(df->mtx, NULL) < 0)					// To be checked: error codes
		return (clear_out(df, 0), -1);

	return (0);
}

// int	init_philosphers(t_df *df)
// {
// 	*df = (t_df){

// 	};
// }

/*
*shl = (t_shell){
		.ac = ac,
		.av = av,
		.stdio[0] = dup(STDIN_FILENO),
		.stdio[1] = dup(STDOUT_FILENO),
		.shlvl = 1,
		.tmp_file_fd = -1
	};
*/
