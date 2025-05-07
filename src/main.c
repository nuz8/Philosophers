/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:11 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/06 20:36:13 by pamatya          ###   ########.fr       */
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

	if (start_simulation(df) < 0)
		return (clear_out(df), 3);
	
	printf("Sim successfully completed.\n");
	
	clear_out(df);
	return (0);
}
