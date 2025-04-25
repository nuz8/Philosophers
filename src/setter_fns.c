/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter_fns.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:24:45 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/25 17:33:56 by pamatya          ###   ########.fr       */
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

/*
Function to update the value of an int, as well as increase or decrease it
	- if put is INCREASE (-10) then the value at destination is increased by 1
	- if put is DECREASE (-20) then the value at destination is decreased by 1
	- for any other value of put (which can only be positive in this context),
	  the value at destination will be modified to the value of put
	- all the operations are carried out by locking/unlocking the mutex 'mtx' in
	  a thread-safe way
	- returns 0 for success and -1 for any failures from mutex operations
*/
int	set_int(t_mutex *mtx, int *destination, int put)
{
	int	ret_code;

	ret_code = 0;
	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		ret_code = -1;
	if (put == INCREASE)
		(*destination)++;
	else if (put == DECREASE)
		(*destination)--;
	else
		*destination = put;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		ret_code = -1;
	return (ret_code);
}

/*
Function to update the value of a long, as well as increase or decrease it
	- if put is INCREASE (-10) then the value at destination is increased by 1
	- if put is DECREASE (-20) then the value at destination is decreased by 1
	- for any other value of put (which can only be positive in this context),
	  the value at destination will be modified to the value of put
	- all the operations are carried out by locking/unlocking the mutex 'mtx' in
	  a thread-safe way
	- returns 0 for success and -1 for any failures from mutex operations
*/
int	set_long(t_mutex *mtx, long *destination, long put)
{
	int	ret_code;

	ret_code = 0;
	if (print_mutex_error(LOCK, pthread_mutex_lock(mtx)) != 0)
		ret_code = -1;
	if (put == INCREASE)
		(*destination)++;
	else if (put == DECREASE)
		(*destination)--;
	else
		*destination = put;
	if (print_mutex_error(UNLOCK, pthread_mutex_unlock(mtx)) != 0)
		ret_code = -1;
	return (ret_code);
}
