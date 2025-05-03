/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter_fns.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:24:32 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/02 19:44:14 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

// int		get_bool(t_mutex *mtx, bool *source, bool *fetch);
bool	get_bool(t_mutex *mtx, bool *source);
int		get_int(t_mutex *mtx, int *source);
long	get_long(t_mutex *mtx, long *source);

int	test_get_int(t_mutex *mtx, int *source, t_phil *philo);


// int	get_bool(t_mutex *mtx, bool *source, bool *fetch)
// {
// 	int	ret_code;

// 	ret_code = 0;
// 	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
// 		ret_code = -1;
// 	*fetch = *source;
// 	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
// 		ret_code = -1;
// 	return (ret_code);
// }

bool	get_bool(t_mutex *mtx, bool *source)
{
	bool	ret;

	print_mutex_error(LOCK, pthread_mutex_lock(mtx));
	ret = *source;
	print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx));
	return (ret);
}

int	get_int(t_mutex *mtx, int *source)
{
	int	ret_value;

	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		return (-1);
	ret_value = *source;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		return (-1);
	return (ret_value);
}

long	get_long(t_mutex *mtx, long *source)
{
	long	ret_value;

	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		return (-1);
	ret_value = *source;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		return (-1);
	return (ret_value);
}

int	test_get_int(t_mutex *mtx, int *source, t_phil *philo)
{
	int	ret_value;

	// test_print_philo_presence(philo);	// TPF
	(void)philo;
	
	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		return (-1);
	ret_value = *source;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		return (-1);

	test_print_philo_presence(philo);	// TPF
	
	return (ret_value);
}
