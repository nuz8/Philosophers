/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 23:42:09 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/22 15:35:06 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	keep_time();
long	get_abs_time(int mode);
long	get_sim_time(int mode);

// Test function for gettimeofday() fn
void	keep_time()
{
	struct timeval	tv;
	long			sec;
	long			msec;
	long			usec;

	gettimeofday(&tv, NULL);
	sec = tv.tv_sec;
	usec = tv.tv_usec;
	printf("sec:	%ld\n", sec);
	printf("usec:	%ld\n", usec);

	sec = get_abs_time(1);
	msec = get_abs_time(2);
	usec = get_abs_time(3);

	printf("get_abs_time sec:	%ld\n", sec);
	printf("get_abs_time msec:	%ld\n", msec);
	printf("get_abs_time usec:	%ld\n", usec);
}

/*
Function to get the time in sec, millisecond, or microsecond based on the value
of the parameter mode
	- Valid modes: 1 to 3
	- Mode 1: returns absolute time in seconds
	- Mode 2: returns absolute time in milliseconds
	- Mode 3: returns absolute time in microseconds
	
Note*: absolute time means as returned by gettimeofday() fn (starting Jan 1970)
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

	if (mode == 1)
		time = sec + usec / (1000000);
	else if (mode == 2)
		time = sec * 1000 + usec / 1000;
	else if (mode == 3)
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
	if (mode == 1)			// sec
		abs_start_time = df->start_time / 1000;
	else if (mode == 2)		// ms
		abs_start_time = df->start_time;
	else if (mode == 3)		// us
		abs_start_time = df->start_time * 1000;
	sim_time = get_abs_time(mode) - abs_start_time;
	return (sim_time);
}
