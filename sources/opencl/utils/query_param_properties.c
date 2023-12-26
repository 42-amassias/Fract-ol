/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   query_param_properties.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 17:20:08 by amassias          #+#    #+#             */
/*   Updated: 2023/12/26 17:39:29 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file query_param_properties
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 * @copyright Copyright (c) 2023
 */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

void	_query(
		t_kernel_argument *arg,
		const t_kernel_argument_type_data *query
		);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	query_param_property(
		const char *type,
		t_kernel_argument *arg
		)
{
	size_t	i;

	i = 0;
	while (i < _ARG_TYPE_COUNT)
		if (ft_strcmp(g_kernel_arg_types[i].litteral_name, type) == 0)
			return (_query(arg, &g_kernel_arg_types[i]), 0);
	return (1);
}

void	_query(
		t_kernel_argument *arg,
		const t_kernel_argument_type_data *query
		)
{
	arg->size = query->size;
	arg->type_name = query->litteral_name;
	arg->type = query->type;
}
