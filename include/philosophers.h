/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:10:30 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/08 00:42:50 by pamatya          ###   ########.fr       */
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

/* ----------------------------- Error Messages ----------------------------- */

//Error messages for mac
// # define ERR_MALLOC		R"Error: "Y"malloc failed\n"RST
// # define ERR_INVALID	R"Error: "Y"invalid characters\n"RST
// # define ERR_NEGATIVE	R"Error: "Y"input is negative\n"RST
// # define ERR_TOOLONG	R"Error: "Y"input too large, limit is 2147483647\n"RST
// # define ERR_TOOSMALL	R"Error: "Y"time too small, should be >= 60\n"RST

// # define ERR_STH		R"Error: "Y"SOME ERROR THAT NEEDS ADDRESSING\n"RST


// Error messages for linux
# define ERR_MALLOC		"Error: malloc failed\n"
# define ERR_INVALID	"Error: invalid characters\n"
# define ERR_NEGATIVE	"Error: input is negative\n"
# define ERR_TOOLONG	"Error: input too large, limit is 2147483647\n"
# define ERR_TOOSMALL	"Error: time too small, should be >= 60\n"

# define ERR_STH		"Error: SOME ERROR THAT NEEDS ADDRESSING\n"

// Standard error messages for mutex errors
# define ERR_INIT_EINVAL "The value specified by attr is invalid.\n"
# define ERR_INIT_ENOMEM \
	"The process cannot allocate enough memory to create another mutex.\n"
# define ERR_GEN_EINVAL "The value specified by mutex is invalid.\n"
# define ERR_LOCK_EDEADLK \
	"A deadlock would occur if the thread blocked waiting for mutex.\n"
# define ERR_UNLOCK_EPERM "The current thread does not hold a lock on mutex.\n"
# define ERR_DEST_EBUSY "Mutex is locked.\n"

/* ----------------------------- Error Messages ----------------------------- */

# define INCREASE -10
# define DECREASE -20
# define PHILO_FULL -299
# define SIM_ENDED -399
# define SIM_COMPLETED -499

typedef pthread_mutex_t	t_mutex;

/* ------------------- enums ------------------- */
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
	EATING,
	SLEEPING,
	THINKING,
	DIED
}				e_phstates;

typedef enum	e_units
{
	SECOND,
	MILLI,
	MICRO
}				e_units;

typedef enum	e_turn
{
	EVEN_PHILOS,
	ODD_PHILOS
}				e_turn;

// Enum type for mutex operations
typedef enum	e_mtx_op
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY
}				e_mtx_op;

typedef	enum	e_check
{
	PHILO,
	SIMULATION,
	BOTH,
	ONE,
	TWO
}				e_check;

/* ==================== enums ==================== */

/* ------------------------- structs ------------------------- */
typedef struct	s_fork
{
	int		id;			// starts at 0
	t_mutex	mtx;		// pointer to the fork mutex
	bool	mtx_init;	// flag for initialization status of the mutex mtx
	int		state;		// states: TAKEN / FREE ; from e_fstates enums
	int		taker_id;	// philo_id when state is TAKEN, else flag: init to 0
}				t_fork;

typedef struct	s_phil
{
	int			id;				// philosopher id number
	pthread_t	th_id;			// thread id number
	t_mutex		mtx;			// philo mutex
	bool		mtx_init;		// flag for initialization status of the mutex mtx
	int			state;	// remove	// philo states from e_phstates enums, else flag -1
	t_fork		*fork1;			// pointer to the first fork
	t_fork		*fork2;			// pointer to the second fork
	long		meals_left;		// keep count of the number of times the philo has eaten
	long		lastmeal_time;	// time stamp when the philosopher last ate
	bool		full
	;		// 1 if it is the last phil in the round-table
	bool		dead;
}				t_phil;

typedef struct	s_df
{
	long		total_philos;	// total number of philos from user
	long		ttd;			// time to die from user, stored in microseconds
	long		tte;			// time to eat from user, stored in microseconds
	long		tts;			// time to sleep from user, stored in microseconds
	long		ttt;			// time to think, modifiable value suiting the algo
	long		max_meals;		// no. of times each philo should eat before end of simulation, optional arg provided by user
	
	t_fork		*forks;			// pointer to the array of forks
	t_phil		*philos;		// pointer to the array of philosophers
	
	t_mutex		mtx;			// dataframe mutex
	bool		mtx_init;		// flag for initialization status of the mutex mtx
	t_mutex		mtx_write;		// mutex for only writing logs
	bool		mtx_write_init;	// boolean flag to indicate mtx_write was initialized
	
	pthread_t	manager;		// manager is the supervising thread to check if simulation has ended by either completion of meals or a philo dying
	bool		all_threads_ready;
	int			threads_running_nbr;
	long		start_time;		// time of start of the simulation in microseconds
	bool		sim_finished;	// a boolean to indicate whether any criteria for ending the simulation has been met
}				t_df;

/* ================================ structs ================================ */


/* -------------------------- function prototypes -------------------------- */
/* -------------------------------- errors.c -------------------------------- */

int		arg_error(void);
void	print_errstr(char *str);
int		print_mutex_error(e_mtx_op operation, int err_code);

int	print_mutex_error_debug(t_phil *philo, t_fork *fork, e_mtx_op operation, int err_code);


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

long	get_abs_time(int mode);
long	get_sim_time(int mode);
int		ft_usleep(long start_time, long tts_usec);

int	ft_usleep2(long start_time, long tts_usec, t_phil *philo);

/* ----------------------------- simulation.c ----------------------------- */

int		start_simulation(t_df *df);

/* ----------------------------- manager.c ----------------------------- */

void	*supervise(void *arg);
bool	philo_should_exit(t_df *df, t_phil *philo, e_check check);

/* ----------------------------- events.c ----------------------------- */

int		philo_eat(t_df *df, t_phil *philo);
int		philo_sleep(t_df *df, t_phil *philo);
int		philo_think(t_df *df, t_phil *philo, bool first_think);

/* ----------------------------- forks.c ----------------------------- */

int	philo_pickup_forks(t_df *df, t_phil *philo);
int	philo_drop_forks(t_phil *philo, e_check drop);

/* ----------------------------- loggers.c ----------------------------- */

int		log_event_safe(t_phil *philo, e_phstates state);

int	log_event_unsafe(t_phil *philo, e_phstates state);	// to be removed

/* ----------------------------- getter_fns.c ----------------------------- */

// int		get_bool(t_mutex *mtx, bool *source, bool *fetch);
bool	get_bool(t_mutex *mtx, bool *source);	// destination and put for the setter function
int		get_int(t_mutex *mtx, int *source);
long	get_long(t_mutex *mtx, long *source);

/* ----------------------------- setter_fns.c ----------------------------- */

int		set_bool(t_mutex *mtx, bool *destination, bool put);
int		set_int(t_mutex *mtx, int *destination, int put);
int		set_long(t_mutex *mtx, long *destination, long put);


/* ----------------------------- synchronizers.c ----------------------------- */

bool	all_threads_running(t_mutex *mtx, int *threads, int total_philos);

/* ----------------------------- utils.c ----------------------------- */

void	clear_out(t_df *df);

/* ============================= test functions ============================= */

void	test_print_elements(void);
void	test_print_time(void);
void	test_print_fork_tags(void);
void	test_print_logs(void);
void	test_print_mutex_errors(void);
void	test_print_fork_owners();
void	test_print_philo_presence(t_phil *philo);
void	test_print_safe_mutex_destruction(t_mutex *mtx);

int		test_get_int(t_mutex *mtx, int *source, t_phil *philo);
void	keep_time(void);

#endif
