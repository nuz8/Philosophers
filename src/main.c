/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:11 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/07 18:27:30 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	main(int ac, char **av)
{
	int		ret;
	t_df	*df;

	if (!(ac == 5 || ac == 6))
		return (arg_error());
	df = get_df();
	if (!df)
		return (print_errstr(ERM_MALLOC), ERC_MALLOC);
	ret = init_struct(ac, av);
	if (ret != 0)
		return (ret);

	test_print_elements();
	test_print_time();
	free(df);
	return (0);
}
