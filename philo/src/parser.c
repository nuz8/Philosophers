/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 00:56:59 by pamatya           #+#    #+#             */
/*   Updated: 2025/05/09 23:07:00 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long		ft_atol_tailored(const char *str);

static int	ft_isdigit(unsigned int c);
static int	ft_is_wspace(unsigned int c);
static int	check_invalid_beyond_digits(const char *str, int pos);
static int	check_invalid_digit_length(const char *str, int pos);

/*
Function to convert alphanumeric to long while also checking if arguments are
valid.
  - Following are the valid and invalid input types.
  	- Valid:		"      +444"
  	- Valid:		"      +444abc"
  	- Invalid:	"    ++--  $f%33bc&"
  - Also checks for negative numbers, and numbers greater than INT_MAX
*/
long	ft_atol_tailored(const char *str)
{
	long	num;
	int		pos;

	num = 0;
	pos = 0;
	while (ft_is_wspace(str[pos]))
		pos++;
	if (str[pos] == '+' || str[pos] == '-')
		pos++;
	if (!ft_isdigit(str[pos]))
		return (print_errstr(ERR_INVALID), -1);
	if (pos > 0 && str[pos - 1] == '-')
		return (print_errstr(ERR_NEGATIVE), -1);
	if (check_invalid_beyond_digits(str, pos))
		return (print_errstr(ERR_INVALID), -1);
	if (check_invalid_digit_length(str, pos))
		return (print_errstr(ERR_TOOLONG), -1);
	while (str[pos] && ft_isdigit(str[pos]))
		num = num * 10 + (str[pos++] - '0');
	if (num > INT_MAX)
		return (print_errstr(ERR_TOOLONG), -1);
	return (num);
}

// Function that returns 1 if character is a digit, else returns 0
static int	ft_isdigit(unsigned int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

// Function to return 1 if character is a whitespace, otherwise returns 0
static int	ft_is_wspace(unsigned int c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

/*
Function to check if all characters from the position 'pos' in the string 'str'
are digits
	- Returns 1 if an invalid character (i.e. non-digit) is found
	- Returns 0 if the check is over and no invalid characters were found
*/
static int	check_invalid_beyond_digits(const char *str, int pos)
{
	int	i;

	i = pos;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	check_invalid_digit_length(const char *str, int pos)
{
	int	digit_len;

	digit_len = ft_strlen(str + pos);
	if (digit_len > 10)
		return (1);
	return (0);
}
