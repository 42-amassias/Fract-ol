/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel_arg_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 17:41:08 by amassias          #+#    #+#             */
/*   Updated: 2024/01/12 17:45:23 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file kernel_arg_type.c
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2024-01-12
 * @copyright Copyright (c) 2024
 */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	is_type_integer(
		t_cl_arg_type type
		)
{
	return ((type & CL_ARG_TYPE__CATEGORY_MASK) == CL_ARG_TYPE_INTEGER);
}

int	is_type_floating(
		t_cl_arg_type type
		)
{
	return ((type & CL_ARG_TYPE__CATEGORY_MASK) == CL_ARG_TYPE_FLOATING);
}

int	is_type_integer__signed(
		t_cl_arg_type type
		)
{
	return (
		is_type_integer(type)
		&& (type & CL_ARG_TYPE__ATTR_MASK) == CL_ARG_TYPE_ATTR_SIGNED);
}

int	is_type_integer__unsigned(
		t_cl_arg_type type
		)
{
	return (
		is_type_integer(type)
		&& (type & CL_ARG_TYPE__ATTR_MASK) == CL_ARG_TYPE_ATTR_UNSIGNED);
}
