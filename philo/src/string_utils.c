/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:15:45 by pamatya           #+#    #+#             */
/*   Updated: 2025/04/15 15:59:45 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

size_t	ft_strlen(const char *str);
int		ft_fprintf_str(const int fd, const char *str[]);
char	*concat_strings(const char *str[]);

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

/*
Function to malloc a total string by combining all the strings provided
  - Returns a bigger malloc'd string
  - Final string should be provided as NULL
*/
char	*concat_strings(const char *str[])
{
	size_t	indices[3];
	size_t	tlen;
	char	*concat;

	indices[0] = -1;
	tlen = 0;
	while (str[++indices[0]])
	{
		indices[1] = -1;
		while (str[indices[0]][++(indices[1])])
			tlen++;
	}
	concat = malloc((tlen + 1) * sizeof(char));
	if (!concat)
		return (NULL);
	indices[0] = -1;
	indices[2] = 0;
	while (str[++indices[0]])
	{
		indices[1] = 0;
		while (str[indices[0]][indices[1]])
			concat[(indices[2])++] = str[indices[0]][(indices[1])++];
	}
	concat[indices[2]] = '\0';
	return (concat);
}
