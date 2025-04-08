/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:56:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/15 01:36:05 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		set_parameters(int ac, char **av, t_df *df);
int		ft_isdigit(unsigned int c);
long	ft_atol(const char *str);

int	set_parameters(int ac, char **av, t_df *df)
{
	int	i;
	int	j;
	
	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (!(ft_isdigit(av[i][j])))
				return (printf("Invalid character in arg[%d]\n", i), 1);
		}
	}
	df->num_philos = ft_atol(av[1]);
	df->ttd = ft_atol(av[2]);
	df->tte = ft_atol(av[3]);
	df->tts = ft_atol(av[4]);
	if (ac == 6)
		df->max_eats = ft_atol(av[5]);
	return (0);
}

int	ft_isdigit(unsigned int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

// Function to convert alphanumeric to long
long	ft_atol(const char *str)
{
	long	signe;
	long	num;
	int		i;
	int		s_count;

	signe = 1;
	num = 0;
	i = 0;
	s_count = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i ++;
	while (str[i] == '+' || str[i] == '-')
	{
		s_count++;
		if (str[i] == '-')
			signe *= -1;
		i++;
	}
	if (s_count > 1)
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - '0');
	num = signe * num;
	return (num);
}
