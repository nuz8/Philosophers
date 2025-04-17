/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:01:45 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/16 16:55:16 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		arg_error(void);
void	print_errstr(char *str);

int	arg_error(void)
{
	int	ret;

	ret = ft_fprintf_str(STDERR_FILENO, (const char *[]){R"Syntax error! ",
		G"Usage options:\n", "\t1. ./philo (no. of philos) (time to die) ",
		"(time to eat) (time to sleep)\n",
		"\t2. ./philo (no. of philos) (time to die) ",
		"(time to eat) (time to sleep) ",
		"[(optional) number of times each philosopher must eat]\n"RST, NULL});
	if (ret < 0)
		return (ret);
	return (0);
}

void	print_errstr(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}
