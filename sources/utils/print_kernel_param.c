/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_kernel_param.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:47:07 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:36:37 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"
#include "utils.h"

#include <libft.h>
#include <stdio.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

#define TYPE_MASK CL_ARG_TYPE__TYPE_MASK
#define CATEGORY_MASK CL_ARG_TYPE__CATEGORY_MASK
#define ATTR_MASK CL_ARG_TYPE__ATTR_MASK

#define CANNOT_PRINT "[Cannot display]"

/* ************************************************************************** */
/*                                                                            */
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

static void	_print_floating(
				t_kernel_arg *arg);

static void	_print_signed(
				t_kernel_arg *arg);

static void	_print_unsigned(
				t_kernel_arg *arg);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

void	print_kernel_param(
			t_kernel_arg *arg)
{
	if ((arg->type & CATEGORY_MASK) == (CL_ARG_TYPE_FLOATING & CATEGORY_MASK))
	{
		_print_floating(arg);
		return ;
	}
	if ((arg->type & CATEGORY_MASK) != (CL_ARG_TYPE_INTEGER & CATEGORY_MASK))
	{
		ft_putstr(CANNOT_PRINT);
		return ;
	}
	if ((arg->type & ATTR_MASK) == (CL_ARG_TYPE_ATTR_UNSIGNED & ATTR_MASK))
		_print_unsigned(arg);
	else if ((arg->type & ATTR_MASK) == (CL_ARG_TYPE_ATTR_SIGNED & ATTR_MASK))
		_print_signed(arg);
	else
		ft_putstr(CANNOT_PRINT);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static void	_print_floating(
				t_kernel_arg *arg)
{
	if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__DOUBLE & TYPE_MASK))
	{
		printf("%lf", *(cl_double *)arg->value);
		fflush(stdout);
	}
	else if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__FLOAT & TYPE_MASK))
	{
		printf("%f", *(cl_float *)arg->value);
		fflush(stdout);
	}
	else
		ft_putstr(CANNOT_PRINT);
}

static void	_print_signed(
				t_kernel_arg *arg)
{
	if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__CHAR & TYPE_MASK))
		ft_printf("%d", *(cl_char *)arg->value);
	else if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__SHORT & TYPE_MASK))
		ft_printf("%d", *(cl_short *)arg->value);
	else if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__INT & TYPE_MASK))
		ft_printf("%d", *(cl_int *)arg->value);
	else if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__LONG & TYPE_MASK))
		ft_printf("%ld", *(cl_long *)arg->value);
	else
		ft_putstr(CANNOT_PRINT);
}

static void	_print_unsigned(
				t_kernel_arg *arg)
{
	if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__UCHAR & TYPE_MASK))
		ft_printf("%u", *(cl_uchar *)arg->value);
	else if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__USHORT & TYPE_MASK))
		ft_printf("%u", *(cl_ushort *)arg->value);
	else if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__UINT & TYPE_MASK))
		ft_printf("%u", *(cl_uint *)arg->value);
	else if ((arg->type & TYPE_MASK) == (CL_ARG_TYPE__ULONG & TYPE_MASK))
		ft_printf("%lu", *(cl_ulong *)arg->value);
	else
		ft_putstr(CANNOT_PRINT);
}
