/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:56:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/16 16:51:29 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	parse_arguments(int ac, char **av, t_df *df);

static long	ft_atol_tailored(const char *str);
static int	ft_isdigit(unsigned int c);
static int	ft_is_wspace(unsigned int c);
// static int	check_arg_offset(const char *str);

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
	df->ttd = ft_atol_tailored(av[2]);
	df->tte = ft_atol_tailored(av[3]);
	df->tts = ft_atol_tailored(av[4]);
	if (ac == 6)
		df->max_meals = ft_atol_tailored(av[5]);	
	if (df->total_philos < 0 || df->ttd < 0 || df->tte < 0 || df->tts < 0
		|| df->max_meals < 0)
		return (-1);
	if (df->ttd < 60 || df->tte < 60 || df->tts < 60)
		return (print_errstr(ERR_TOOSMALL), -1);
	return (0);
}

/*
Function to convert alphanumeric to long while also checking if arguments are
valid.
  - Following are the valid and invalid input types.
  	- Valid:		"      +444%*  a"	✅
  	- Invalid:	"    ++--  $f%33bc&"	❌
  - Also checks for negative numbers, and numbers greater than INT_MAX
*/
static long	ft_atol_tailored(const char *str)
{
	long	num;
	int		pos;

	num = 0;
	pos = 0;
	while (ft_is_wspace(str[pos]))
		pos++;
	if (str[pos] == '+' || str[pos] == '-')
		pos++;
	if (!ft_isdigit(str[pos]))
		return (print_errstr(ERR_INVALID), -1);
	if (pos > 0 && str[pos - 1] == '-')
		return (print_errstr(ERR_NEGATIVE), -1);
	while (str[pos] && ft_isdigit(str[pos]))
		num = num * 10 + (str[pos++] - '0');
	if (num > INT_MAX)
		return (print_errstr(ERR_TOOLONG) , -1);
	return (num);
}

static int	ft_isdigit(unsigned int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	ft_is_wspace(unsigned int c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}
