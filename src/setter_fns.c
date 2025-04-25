/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter_fns.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:24:45 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/25 14:24:43 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	set_bool(t_mutex *mtx, bool *destination, bool put);
int	set_int(t_mutex *mtx, int *destination, int put);
int	set_long(t_mutex *mtx, long *destination, long put);

int	set_bool(t_mutex *mtx, bool *destination, bool put)
{
	int	ret_code;

	ret_code = 0;
	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		ret_code = -1;
	*destination = put;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		ret_code = -1;
	return (ret_code);
}

int	set_int(t_mutex *mtx, int *destination, int put)
{
	int	ret_code;

	ret_code = 0;
	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		ret_code = -1;
	*destination = put;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		ret_code = -1;
	return (ret_code);
}

int	set_long(t_mutex *mtx, long *destination, long put)
{
	int	ret_code;

	ret_code = 0;
	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		ret_code = -1;
	*destination = put;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		ret_code = -1;
	return (ret_code);
}
