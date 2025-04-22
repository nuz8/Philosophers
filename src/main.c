/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:11 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/22 17:27:59 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	main(int ac, char **av)
{
	t_df	*df;

	df = NULL;
	if (!(ac == 5 || ac == 6))
	{
		if (arg_error() < 0)
			return (-1);
		return (1);
	}
	df = get_df();
	if (!df)
		return (print_errstr(ERR_MALLOC), -1);
	
	if (init_df(ac, av) != 0)
		return (clear_out(df), 2);
	
	// Simulation code here; TODO:
	// 1. Tag forks in each philo
	//		to indicate which forks each philo is allowed to pick up or has access to
	// 2. Initialize threads and write the simulation function to pass to the thread init fn
	//
	//
	

	
	// test_print fns
	// test_print_elements();
	// test_print_time();
	test_print_fork_tags();
	
	clear_out(df);
	return (0);
}
