/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_prints.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 20:23:19 by pamatya           #+#    #+#             */
/*   Updated: 2025/02/16 12:22:36 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	test_print_elements(void);

void	test_print_elements(void)
{
	t_df	*df;
	
	df = get_df();
	printf("num of philos:	%ld\n", df->num_philos);
	printf("time to die:	%ld\n", df->ttd);
	printf("time to eat:	%ld\n", df->tte);
	printf("time to sleep:	%ld\n", df->tts);	
}

void	test_print_time(void)
{
	
}
