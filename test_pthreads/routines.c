/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:43:31 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/11 16:24:56 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/test_philo.h"

void	*routine1();
void	*routine2(void *arg);
void	*routine_wo_lock();
void	*routine_lock();
void	*routine_mutex();

int		iter = 10000000;

void	*routine1()
{
	printf("Test from threads\n");
	sleep(3);
	printf("End of thread\n");
	return (NULL);
}

void	*routine2(void *arg)
{
	char	*i;
	
	i = (char *)arg;
	printf("Test from thread-%s\n", i);
	sleep(3);
	printf("End of thread-%s\n", i);
	return (NULL);
}

void	*routine_wo_lock()
{
	int	i;
	
	i = 0;
	while (i++ < iter)
		mail++;
	return (NULL);
}

void	*routine_lock()
{
	int	i;
	
	i = 0;
	while (i++ < iter)
	{
		while (1)
		{
			if (lock == 0)
				break ;
		}
		lock = 1;
		mail++;
		lock = 0;
	}
	return (NULL);
}

void	*routine_mutex()
{
	int	i;
	
	i = 0;
	while (i++ < iter)
	{
		pthread_mutex_lock(&mutex);
		mail++;
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}
