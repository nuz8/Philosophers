/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:42:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 15:37:43 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long	get_abs_time(t_units mode);
long	get_sim_time(t_units mode);
int		ft_usleep(long start_time, long tts_usec);

/*
Function to get the time in sec, millisecond, or microsecond based on the value
of the parameter mode
	- Valid modes: 1 to 3
	- Mode 1: returns absolute time in seconds
	- Mode 2: returns absolute time in milliseconds
	- Mode 3: returns absolute time in microseconds
	
Note*: absolute time means as returned by gettimeofday() fn (Jan 1970 onwards)
*/
long	get_abs_time(t_units mode)
{
	struct timeval	tv;
	long			time;
	long			sec;
	long			usec;

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
long	get_sim_time(t_units mode)
{
	t_df	*df;
	long	sim_time;
	long	abs_start_time;

	df = get_df();
	sim_time = 0;
	abs_start_time = 0;
	if (mode == SECOND)
		abs_start_time = df->start_time / 1000000;
	else if (mode == MILLI)
		abs_start_time = df->start_time / 1000;
	else if (mode == MICRO)
		abs_start_time = df->start_time;
	sim_time = get_abs_time(mode) - abs_start_time;
	return (sim_time);
}

/*
ft_usleep fn above but with get_sim_time instead of get_abs_time
	- 
*/
int	ft_usleep(long start_time, long tts_usec)
{
	long	rem_tts;
	t_df	*df;

	df = get_df();
	while (get_sim_time(MICRO) < start_time + tts_usec)
	{
		if (philo_should_exit(df, NULL, SIMULATION))
			return (SIM_COMPLETED);
		rem_tts = tts_usec + start_time - get_sim_time(MICRO);
		if (rem_tts > 100)
			usleep(100);
		else
		{
			while (get_sim_time(MICRO) < start_time + tts_usec)
				continue ;
		}
	}
	return (0);
}
