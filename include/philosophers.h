/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:10:30 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/23 12:14:09 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>		// printf
# include <stdlib.h>	// malloc, free
# include <unistd.h>	// write, usleep
# include <pthread.h>	// threads (create, join, detach) and mutex (init, destroy, lock, unlock)
# include <sys/time.h>	// gettimeofday
# include <stdbool.h>	// boolean datatype
# include <limits.h>	// INT_MAX
# include <errno.h>		// errno

/*
ASCII escapte sequences for setting text colour (BOLD)
RST is to reset the colour after changing it...should be used everytime
*/
# define BB		"\033[1;30m"	// Bold Black
# define R		"\033[1;31m"	// Bold Red
# define G		"\033[1;32m"	// Bold Green
# define B		"\033[1;34m"	// Bold Blue
# define Y		"\033[1;33m"	// Bold Yellow
# define M		"\033[1;35m"	// Bold Magenta
# define C		"\033[1;36m"	// Bold Cyan
# define W		"\033[1;37m"	// Bold White
# define RST	"\033[0m"		// Reset to default

/*
Error messages for mac
*/
// # define ERR_MALLOC		R"Error: "Y"malloc failed\n"RST
// # define ERR_INVALID	R"Error: "Y"invalid characters\n"RST
// # define ERR_NEGATIVE	R"Error: "Y"input is negative\n"RST
// # define ERR_TOOLONG	R"Error: "Y"input too large, limit is 2147483647\n"RST
// # define ERR_TOOSMALL	R"Error: "Y"time too small, should be >= 60\n"RST

// # define ERR_STH		R"Error: "Y"SOME ERROR THAT NEEDS ADDRESSING\n"RST

/*
Error messages for linux
*/
# define ERR_MALLOC		"Error: malloc failed\n"
# define ERR_INVALID	"Error: invalid characters\n"
# define ERR_NEGATIVE	"Error: input is negative\n"
# define ERR_TOOLONG	"Error: input too large, limit is 2147483647\n"
# define ERR_TOOSMALL	"Error: time too small, should be >= 60\n"

# define ERR_STH		"Error: SOME ERROR THAT NEEDS ADDRESSING\n"


typedef pthread_mutex_t	t_mutex;

// ENUMs for fork states
typedef enum	e_fstates
{
	TAKEN,
	FREE
}				e_fstates;

// ENUMs for states of the philosophers
typedef enum	e_phstates
{
	TOOK_FORK_1,
	TOOK_FORK_2,
	READY,
	EATING,
	SLEEPING,
	THINKING,
	DIED
}				e_phstates;


typedef struct	s_fork
{
	int		id;			// starts at 0
	t_mutex	mtx;		// pointer to the fork mutex
	bool	mtx_init;	// flag for initialization status of the mutex mtx
	int		state;		// states: TAKEN / FREE ; from e_fstates enums
	bool	with_left;	// flag, init to 0, 1 when state is TAKEN by philo to its left
	bool	with_right;	// flag, init to 0, 1 when state is TAKEN by philo to its right
	int		taker_id;	// philo_id when state is TAKEN, else flag: init to 0
}				t_fork;

typedef struct	s_phil
{
	int			id;			// philosopher id number
	pthread_t	th_id;		// thread id number
	t_mutex		mtx;		// philo mutex
	bool		mtx_init;	// flag for initialization status of the mutex mtx
	int			state;		// philo states from e_phstates enums (ready to died), else flag -1
	t_fork		*fork1;		// pointer to the first fork
	t_fork		*fork2;		// pointer to the second fork
	int			eat_count;	// keep count of the number of times the philo has eaten
	bool		last_phil;	// 1 if it is the last phil in the round-table
}				t_phil;

typedef struct	s_df
{
	long	total_philos;	// total number of philos from user
	long	ttd;			// time to die from user, stored in microseconds
	long	tte;			// time to eat from user, stored in microseconds
	long	tts;			// time to sleep from user, stored in microseconds
	long	ttt;			// time to think, modifiable value suiting the algo
	long	max_meals;		// no. of times each philo should eat before end of simulation, optional arg provided by user
	t_fork	*forks;
	t_phil	*philos;		// pointer to the array of philosophers
	t_mutex	mtx;			// dataframe mutex
	bool	mtx_init;		// flag for initialization status of the mutex mtx
	long	start_time;		// time of start of the simulation
	bool	sim_finished;	// a boolean to indicate whether any criteria for ending the simulation has been met
}				t_df;


/* -------------------------------- errors.c -------------------------------- */

int		arg_error(void);
void	print_errstr(char *str);

/* ------------------------------- spawners.c ------------------------------- */

t_df	*get_df(void);
void	spawn_philo(t_phil *philo);
void	spawn_fork(t_fork *fork);

/* ------------------------------- init_df.c ------------------------------- */

int		init_df(int ac, char **av);
int		init_philos(t_df *df);
int		init_forks(t_df *df);

/* ----------------------------- string_utils.c ----------------------------- */

size_t	ft_strlen(const char *str);
int		ft_fprintf_str(const int fd, const char *str[]);
char	*concat_strings(const char *str[]);

/* ----------------------------- parser.c ----------------------------- */

int		parse_arguments(int ac, char **av, t_df *df);

/* ----------------------------- timers.c ----------------------------- */

void	keep_time();
long	get_abs_time(int mode);
long	get_sim_time(int mode);

/* ----------------------------- simulation.c ----------------------------- */

int		start_simulation(t_df *df);

/* ----------------------------- utils.c ----------------------------- */

// void	clear_out(t_df *df, int mode);
void	clear_out(t_df *df);


/* ============================= test functions ============================= */

void	test_print_elements(void);
void	test_print_time(void);
void	test_print_fork_tags();

#endif

