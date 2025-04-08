/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:10:30 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/08 11:34:39 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

// Fork things
# define TAKEN 1
# define FREE 0

// Philo things
# define EATING 'e'
# define SLEEPING 's'
# define THINKING 't'
# define DEAD 'd'

// Error codes
# define ERC_MALLOC -1

// Error messages
# define ERM_MALLOC "Malloc failed"

typedef struct s_fork
{
	int		status;		// taken or free
	int		isonleft;	// 1 if taken by the philo to its left, else 0
	int		isonright;	// 1 if taken by the philo to its right, else 0
	int		taker;		// philo number that picked up the fork, 0 if free
}	t_fork;

typedef struct s_phil
{
	pthread_t		*th;
	char			status;		// eating / sleeping / thinking
	t_fork			*lfork;		// has taken the left fork, else NULL
	t_fork			*rfork;		// has taken the right fork, else NULL
	int				last_phil;	// 1 if it is the last phil in the round-table
}	t_phil;

typedef struct s_df
{
	long			num_philos;
	long			ttd;
	long			tte;
	long			tts;
	long			ttt;
	long			max_eats;
	t_phil			*phil;
	pthread_mutex_t	mutex;
}	t_df;



/* -------------------------------- errors.c -------------------------------- */

int		arg_error(void);
void	print_errstr(char *str);

/* -------------------------------- init_df.c -------------------------------- */

t_df	*get_df(void);
int		init_struct(int ac, char **av);

/* ----------------------------- string_utils.c ----------------------------- */

size_t	ft_strlen(const char *str);
int		ft_fprintf_str(const int fd, const char *str[]);
char	*concat_strings(const char *str[]);


/* ----------------------------- parser.c ----------------------------- */

int		set_parameters(int ac, char **av, t_df *df);
int		ft_isdigit(unsigned int c);
long	ft_atol(const char *str);

/* ============================= test functions ============================= */

void	test_print_elements(void);

#endif

