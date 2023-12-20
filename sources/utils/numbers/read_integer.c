/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_integer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 03:13:30 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:32:27 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

cl_long	read_integer_signed(
			const char *str,
			const char **end_ptr)
{
	int		sign;
	cl_long	value;

	sign = 1;
	value = 0;
	while (*str && ft_isspace(*str))
		++str;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		++str;
	if (!ft_isdigit(*str))
	{
		*end_ptr = str;
		return (0);
	}
	while (*str && ft_isdigit(*str))
		value = 10 * value + *str++ - '0';
	value *= sign;
	while (*str && ft_isspace(*str))
		++str;
	*end_ptr = str;
	return (value);
}

cl_ulong	read_integer_unsigned(
			const char *str,
			const char **end_ptr)
{
	cl_ulong	value;

	value = 0;
	while (*str && ft_isspace(*str))
		++str;
	if (*str == '+')
		++str;
	if (!ft_isdigit(*str))
	{
		*end_ptr = str;
		return (0);
	}
	while (*str && ft_isdigit(*str))
		value = 10 * value + *str++ - '0';
	while (*str && ft_isspace(*str))
		++str;
	*end_ptr = str;
	return (value);
}
