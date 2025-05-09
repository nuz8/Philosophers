/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:15:45 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 13:24:22 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

size_t	ft_strlen(const char *str);
int		ft_fprintf_str(const int fd, const char *str[]);

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str && *str++)
		len++;
	return (len);
}

/*
Function to print multiple strings
  - Returns total number of printed characters upon success
  - Returns -1 if - fd is 0, or str is NULL, or also if write function fails
  - Final string should be provided as NULL
*/
int	ft_fprintf_str(const int fd, const char *str[])
{
	size_t	index;
	int		total;
	int		ret;

	if (!fd || !str)
		return (-1);
	if (!*str)
		return (0);
	ret = 0;
	total = 0;
	index = 0;
	while (str[index])
	{
		ret = write(fd, str[index], ft_strlen(str[index]));
		if (ret < 0)
			return (ret);
		total += ret;
		index++;
	}
	return (total);
}
