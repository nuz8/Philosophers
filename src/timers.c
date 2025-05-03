/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:42:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/02 20:49:48 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long	get_abs_time(int mode);
long	get_sim_time(int mode);
int		ft_usleep(long	tts_usec);


/*
Function to get the time in sec, millisecond, or microsecond based on the value
of the parameter mode
	- Valid modes: 1 to 3
	- Mode 1: returns absolute time in seconds
	- Mode 2: returns absolute time in milliseconds
	- Mode 3: returns absolute time in microseconds
	
Note*: absolute time means as returned by gettimeofday() fn (Jan 1970 onwards)
*/
long	get_abs_time(int mode)
{
	struct timeval	tv;
	long	time;
	long	sec;
	long	usec;

	time = 0;
	if (gettimeofday(&tv, NULL) < 0)
		return (-1);
	sec = tv.tv_sec;
	usec = tv.tv_usec;

	if (mode == SECOND)
		time = sec + usec / (1000000);
	else if (mode == MILLI)
		time = sec * 1000 + usec / 1000;
	else if (mode == MICRO)
		time = sec * 1000000 + usec;
	return (time);
}

/*
Function to return the sim time relative to the start time of the simulation
	- Valid modes: 1 to 3
	- Mode 1: returns sim time in seconds
	- Mode 2: returns sim time in milliseconds
	- Mode 3: returns sim time in microseconds
*/
long	get_sim_time(int mode)
{
	t_df	*df;
	long	sim_time;
	long	abs_start_time;

	df = get_df();
	sim_time = 0;
	abs_start_time = 0;
	if (mode == SECOND)			// sec
		abs_start_time = df->start_time / 1000000;
	else if (mode == MILLI)		// ms
		abs_start_time = df->start_time / 1000;
	else if (mode == MICRO)		// us
		abs_start_time = df->start_time;
	sim_time = get_abs_time(mode) - abs_start_time;
	return (sim_time);
}

// int	ft_usleep(long	tts_usec)
// {
// 	long	cur_time;
// 	long	rem_tts_usec;

// 	cur_time = get_sim_time(MICRO);
// 	if (usleep(0.5 * tts_usec) < 0)
// 		return (print_errstr("usleep failed\n"), -1);
// 	rem_tts_usec = 0.5 * tts_usec;
// 	while (rem_tts_usec > 1)
// 	{
// 		if (usleep(rem_tts_usec / 2) < 0)
// 			return (print_errstr("usleep failed\n"), -1);
// 		rem_tts_usec = rem_tts_usec / 2;
// 	}
// 	return (0);
// }

/*
Custom usleep fn			// Need to remove the type variable later
	- if type = 0, then uses built-in usleep
	- if type = 1, then uses custom usleep
*/
int	ft_usleep(long	tts_usec)
{
	int	type;
	type = 1;
	
	if (type == 0)
		usleep(tts_usec);
	else
	{
		t_df	*df;
		long	start_time;
	
		df = get_df();
		start_time = get_abs_time(MICRO);
		usleep(0.5 * tts_usec);
		while (get_abs_time(MICRO) < start_time + tts_usec)
		{
			if (get_bool(&df->mtx, &df->sim_finished))
				break ;
			usleep (40);	
		}	
	}
	return (0);
}

// int	ft_usleep(long tts_usec)
// {
// 	long	start_time;
// 	long	time_passed;
// 	long	rem_tts;

// 	start_time = get_abs_time(MICRO);
// 	while (get_abs_time(MICRO) < start_time + tts_usec)
// 	{
// 		time_passed = get_abs_time(MICRO) - start_time;
// 		rem_tts = tts_usec - time_passed;
		
// 		if (rem_tts > 100)
// 			usleep(rem_tts);
// 		else
// 		{
// 			while (start_time + tts_usec > get_abs_time(MICRO))
// 				continue ;
// 		}
// 	}
// 	return (0);
// }
