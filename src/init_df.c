/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_df.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 19:54:13 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/15 18:27:32 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_df	*get_df(void);
int		init_df(int ac, char **av, t_df *df);

// Function to get the pointer to the dataframe struct (t_df *)
t_df	*get_df(void)
{
	static t_df	*df = NULL;

	if (df == NULL)
	{
		df = malloc(sizeof(t_df));
		if (!df)
			return (NULL);
	}
	return (df);
}

// // Potential simplification for get_df()
// t_df	*get_df(void)
// {
// 	static t_df	*df = NULL;

// 	if (!df)
// 		df = malloc(sizeof(t_df));
// 	return (df);
// }

int	init_df(int ac, char **av, t_df *df)
{
	int		ret;

	ret = import_parameters(ac, av, df);
	if (ret != 0)
		return (ret);

	return (0);
}

// int	init_philosphers(t_df *df)
// {
// 	*df = (t_df){

// 	};
// }

/*
*shl = (t_shell){
		.ac = ac,
		.av = av,
		.stdio[0] = dup(STDIN_FILENO),
		.stdio[1] = dup(STDOUT_FILENO),
		.shlvl = 1,
		.tmp_file_fd = -1
	};
*/
