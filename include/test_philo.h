/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_philo.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:49:20 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/11 13:20:16 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_PHILO_H
# define TEST_PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

# include "../lib/includes/libft.h"
# include <string.h>
# include <time.h>

extern int				mail;
extern int				lock;
extern pthread_mutex_t	mutex;
extern int				primes[10];

// Routine functions
void	*routine1();
void	*routine2(void *arg);
void	*routine_wo_lock();
void	*routine_lock();
void	*routine_mutex();

// Test functions
int		test_first_threads(int ac, char **av);
int		test_mutex(int ac, char **av);
int		test_mutex_loop(int ac, char **av);

int		test_return_rolled_dice(int ac, char **av);
int		test_print_primes(int ac, char **av);
int		test_sum_primes(int ac, char **av);
int		test_sum_primes(int ac, char **av);
int		test_trylock(int ac, char **av);

#endif
