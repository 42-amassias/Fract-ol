/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_floating.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 03:21:37 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 01:54:07 by amassias         ###   ########.fr       */
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
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

static const char	*_skip_white_spaces(
						const char *str);

static int			_read_sign(
						const char **str_ptr);

static const char	*_read_integer(
						cl_double *value,
						const char *str);

static const char	*_read_fract(
						cl_double *value,
						const char *str);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

cl_double	read_floating(
				const char *str,
				const char **end_ptr)
{
	int			sign;
	cl_double	value;

	value = 0;
	str = _skip_white_spaces(str);
	if (*str == '\0')
	{
		*end_ptr = str;
		return (0.);
	}
	sign = _read_sign(&str);
	if (*str == '\0')
	{
		*end_ptr = str;
		return (sign * 0.);
	}
	str = _read_integer(&value, str);
	str = _read_fract(&value, str);
	*end_ptr = str;
	return (sign * value);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static const char	*_skip_white_spaces(
						const char *str)
{
	while (ft_isspace(*str))
		++str;
	return (str);
}

static int	_read_sign(
				const char **str_ptr)
{
	if (**str_ptr != '+' && **str_ptr != '-')
		return (1);
	*str_ptr = *str_ptr + 1;
	if ((*str_ptr)[-1] == '-')
		return (-1);
	return (1);
}

static const char	*_read_integer(
						cl_double *value,
						const char *str)
{
	*value = 0.;
	while (ft_isdigit(*str))
		*value = 10. * *value + *str++ - '0';
	return (str);
}

static const char	*_read_fract(
						cl_double *value,
						const char *str)
{
	cl_double	fract;
	size_t		n;

	n = 0;
	fract = 0;
	if (*str != '.')
		return (_skip_white_spaces(str));
	++str;
	while (ft_isdigit(*str))
	{
		fract = 10. * fract + *str++ - '0';
		++n;
	}
	while (n--)
		fract /= 10.;
	*value += fract;
	return (_skip_white_spaces(str));
}
