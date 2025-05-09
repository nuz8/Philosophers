/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:01:45 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 18:09:41 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int		arg_error(void);
void	print_errstr(char *str);
int		print_mutex_error(t_mtx_op operation, int err_code);
int		print_thread_error(t_mtx_op operation, int err_code);

// Function to print argument error message when no. of args is not valid
int	arg_error(void)
{
	int	ret;

	ret = ft_fprintf_str(STDERR_FILENO, (const char *[]){"Invalid arguments! ",
			"Usage options:\n", "\t1. ./philo (no. of philos) (time to die) ",
			"(time to eat) (time to sleep)\n",
			"\t2. ./philo (no. of philos) (time to die) ",
			"(time to eat) (time to sleep) ",
			"[(optional) number of times each philosopher must eat]\n", NULL});
	if (ret < 0)
		return (ret);
	return (0);
}

// Function to print an error string to the standard error output
void	print_errstr(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}

/*
Function to print mutex error strings based on error codes
	- Prints nothing if there is no error and return 0
	- Prints mutex errors if an error is detected and returns the error code
	  associated to the mutex error
*/
int	print_mutex_error(t_mtx_op operation, int err_code)
{
	if (err_code == 0)
		return (0);
	else
	{
		if (operation == INIT && err_code == EINVAL)
			print_errstr(ERR_INIT_EINVAL);
		else if (operation == INIT && err_code == ENOMEM)
			print_errstr(ERR_INIT_ENOMEM);
		else if (operation != INIT && err_code == EINVAL)
			print_errstr(ERR_GEN_EINVAL);
		else if (operation == LOCK && err_code == EDEADLK)
			print_errstr(ERR_LOCK_EDEADLK);
		else if (operation == UNLOCK && err_code == EPERM)
			print_errstr(ERR_UNLOCK_EPERM);
		else if (operation == DESTROY && err_code == EBUSY)
			print_errstr(ERR_DEST_EBUSY);
		return (err_code);
	}
}

/*
Function to print mutex error strings based on error codes
	- Prints nothing if there is no error and return 0
	- Prints mutex errors if an error is detected and returns the error code
	  associated to the mutex error
*/
int	print_thread_error(t_mtx_op operation, int err_code)
{
	if (err_code == 0)
		return (0);
	else
	{
		if (operation == CREATE && err_code == EAGAIN)
			print_errstr(ERR_CREATE_EAGAIN);
		else if (operation == CREATE && err_code == EPERM)
			print_errstr(ERR_CREATE_EPERM);
		else if (operation != CREATE && err_code == EINVAL)
			print_errstr(ERR_CREATE_EINVAL);
		else if (operation == JOIN && err_code == EINVAL)
			print_errstr(ERR_JOIN_EINVAL);
		else if (operation == JOIN && err_code == ESRCH)
			print_errstr(ERR_JOIN_ESRCH);
		else if (operation == JOIN && err_code == EDEADLK)
			print_errstr(ERR_JOIN_EDEADLK);
		return (err_code);
	}
}
