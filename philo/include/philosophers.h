/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:10:30 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 23:06:38 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>		// printf
# include <stdlib.h>	// malloc, free
# include <unistd.h>	// write, usleep
# include <pthread.h>	// threads and mutex
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
# define ERR_MALLOC		"Error: malloc failed\n"
# define ERR_INVALID	"Invalid input: invalid characters\n"
# define ERR_NEGATIVE	"Invalid input: argument is negative\n"
# define ERR_TOOLONG	"Invalid input: too large, limit is 2147483647\n"
# define ERR_TOOSMALL	"Invalid input: time too small, should be >= 60\n"

// Standard error messages for mutex errors
# define ERR_INIT_EINVAL "The value specified by attr is invalid.\n"
# define ERR_INIT_ENOMEM \
	"The process cannot allocate enough memory to create another mutex.\n"
# define ERR_GEN_EINVAL "The value specified by mutex is invalid.\n"
# define ERR_LOCK_EDEADLK \
	"A deadlock would occur if the thread blocked waiting for mutex.\n"
# define ERR_UNLOCK_EPERM "The current thread does not hold a lock on mutex.\n"
# define ERR_DEST_EBUSY "Mutex is locked.\n"

// Standard error messages for pthread errors
// pthread_create errors
# define ERR_CREATE_EAGAIN "The system lacked the necessary resources to \
	create another thread, or the system-imposed limit on the total number of  \
	threads in a process [PTHREAD_THREADS_MAX] would be exceeded.\n"
# define ERR_CREATE_EPERM "The caller does not have appropriate permission to \
	set the required scheduling parameters or scheduling policy.\n"
# define ERR_CREATE_EINVAL "The value specified by attr is invalid.\n"

// pthread_join errors
# define ERR_JOIN_EINVAL "The implementation has detected that the value \
	specified by thread does not refer to a joinable thread.\n"
# define ERR_JOIN_ESRCH "No thread could be found corresponding to that \
	specified by the given thread ID, thread.\n"
# define ERR_JOIN_EDEADLK "A deadlock was detected or the value of thread \
	specifies the calling thread.\n"

/* ----------------------------- Error Messages ----------------------------- */

# define INCREASE -10
# define DECREASE -20
# define PHILO_FULL -299
# define SIM_ENDED -399
# define SIM_COMPLETED -499

typedef pthread_mutex_t	t_mutex;

/* ------------------- enums ------------------- */
// ENUMs for fork states
typedef enum e_fstates
{
	TAKEN,
	FREE
}	t_fstates;

// ENUMs for states of the philosophers
typedef enum e_phstates
{
	TOOK_FORK_1,
	TOOK_FORK_2,
	EATING,
	SLEEPING,
	THINKING,
	DIED
}	t_phstates;

typedef enum e_units
{
	SECOND,
	MILLI,
	MICRO
}	t_units;

// Enum type for mutex operations
typedef enum e_mtx_op
{
	CREATE,
	JOIN,
	INIT,
	LOCK,
	UNLOCK,
	DESTROY
}	t_mtx_op;

typedef enum e_check
{
	PHILO,
	SIMULATION,
	BOTH,
	ONE,
	TWO
}	t_check;

/* ==================== enums ==================== */

/* -------------------------------- structs --------------------------------- */
typedef struct s_fork
{
	int		id;
	t_mutex	mtx;
	bool	mtx_init;
	int		state;
	int		taker_id;
}	t_fork;

typedef struct s_phil
{
	int			id;
	pthread_t	th_id;
	t_mutex		mtx;
	bool		mtx_init;
	t_fork		*fork1;
	t_fork		*fork2;
	long		meals_left;
	long		lastmeal_time;
	bool		full;
	bool		dead;
}				t_phil;

typedef struct s_df
{
	long		total_philos;
	long		ttd;
	long		tte;
	long		tts;
	long		ttt;
	long		max_meals;

	t_fork		*forks;
	t_phil		*philos;

	t_mutex		mtx;
	bool		mtx_init;
	t_mutex		mtx_write;
	bool		mtx_write_init;

	pthread_t	manager;
	bool		all_threads_ready;
	int			threads_running_nbr;
	long		start_time;
	bool		sim_finished;
}				t_df;

/* ================================ structs ================================ */

/* ========================== function prototypes ========================== */
/* -------------------------------- errors.c -------------------------------- */

int		arg_error(void);
void	print_errstr(char *str);
int		print_mutex_error(t_mtx_op operation, int err_code);
int		print_thread_error(t_mtx_op operation, int err_code);

/* ------------------------------- spawners.c ------------------------------- */

t_df	*get_df(void);
void	spawn_philo(t_phil *philo);
void	spawn_fork(t_fork *fork);

/* ------------------------------- init_df.c ------------------------------- */

int		init_df(int ac, char **av);
int		parse_arguments(int ac, char **av, t_df *df);
int		init_philos(t_df *df);
int		init_forks(t_df *df);

/* ----------------------------- string_utils.c ----------------------------- */

size_t	ft_strlen(const char *str);
int		ft_fprintf_str(const int fd, const char *str[]);

/* ----------------------------- parser.c ----------------------------- */

long	ft_atol_tailored(const char *str);

/* ----------------------------- timers.c ----------------------------- */

long	get_abs_time(t_units mode);
long	get_sim_time(t_units mode);
int		ft_usleep(long start_time, long tts_usec);

/* ----------------------------- simulation.c ----------------------------- */

int		start_simulation(t_df *df);

/* ----------------------------- manager.c ----------------------------- */

void	*supervise(void *arg);
bool	philo_should_exit(t_df *df, t_phil *philo, t_check check);

/* ----------------------------- events.c ----------------------------- */

int		philo_eat(t_df *df, t_phil *philo);
int		philo_sleep(t_df *df, t_phil *philo);
int		philo_think(t_df *df, t_phil *philo);

/* ----------------------------- forks.c ----------------------------- */

int		philo_pickup_forks(t_df *df, t_phil *philo);
int		philo_drop_forks(t_phil *philo, t_check drop);

/* ----------------------------- loggers.c ----------------------------- */

int		log_event_safe(t_phil *philo, t_phstates state);

/* ----------------------------- getter_fns.c ----------------------------- */

bool	get_bool(t_mutex *mtx, bool *source);
int		get_int(t_mutex *mtx, int *source);
long	get_long(t_mutex *mtx, long *source);

/* ----------------------------- setter_fns.c ----------------------------- */

int		set_bool(t_mutex *mtx, bool *destination, bool put);
int		set_int(t_mutex *mtx, int *destination, int put);
int		set_long(t_mutex *mtx, long *destination, long put);

/* ---------------------------- synchronizers.c ---------------------------- */

bool	all_threads_running(t_mutex *mtx, int *threads, int total_philos);

/* ----------------------------- utils.c ----------------------------- */

void	clear_out(t_df *df);

#endif
