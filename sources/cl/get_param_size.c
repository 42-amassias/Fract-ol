/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_param_size.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 12:59:58 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 13:16:33 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

#include "libft.h"

/* ************************************************************************** */
/*                                                                            */
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

static int	_is_type_ptr(
				const char *type);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	get_param_size(
		const char *type,
		size_t *size,
		t_cl_arg_type *internal_type)
{
	size_t	i;

	if (_is_type_ptr(type))
		type = "pointer";
	i = 0;
	while (i < CL_ARG_TYPE_COUNT)
	{
		if (ft_strcmp(type, g_cl_types[i].str_type) == 0)
			break ;
		++i;
	}
	if (i == CL_ARG_TYPE_COUNT)
		return (EXIT_FAILURE);
	*size = g_cl_types[i].size;
	*internal_type = g_cl_types[i].internal_type;
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

// `type` cannot be an empty string, so taking `type[len(type) - 2]` is ok !
static int	_is_type_ptr(
				const char *type)
{
	while (*type)
		++type;
	return (type[-1] == '*');
}
