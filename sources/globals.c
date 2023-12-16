/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 20:15:54 by amassias          #+#    #+#             */
/*   Updated: 2023/12/16 04:48:34 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "opencl.h"

/* ************************************************************************** */
/*                                                                            */
/* Global variables                                                           */
/*                                                                            */
/* ************************************************************************** */

const t_cl_type		g_cl_types[] = {
{"char", CL_ARG_TYPE__CHAR, sizeof(cl_char)},
{"short", CL_ARG_TYPE__SHORT, sizeof(cl_short)},
{"int", CL_ARG_TYPE__INT, sizeof(cl_int)},
{"long", CL_ARG_TYPE__LONG, sizeof(cl_long)},
{"uchar", CL_ARG_TYPE__UCHAR, sizeof(cl_uchar)},
{"ushort", CL_ARG_TYPE__USHORT, sizeof(cl_ushort)},
{"uint", CL_ARG_TYPE__UINT, sizeof(cl_uint)},
{"ulong", CL_ARG_TYPE__ULONG, sizeof(cl_ulong)},
{"float", CL_ARG_TYPE__FLOAT, sizeof(cl_float)},
{"double", CL_ARG_TYPE__DOUBLE, sizeof(cl_double)},
};

const t_arg_info	g_needed_kernel_args[] = {
{0, "screen", "int*", CL_KERNEL_ARG_ADDRESS_GLOBAL},
{1, "width", "uint", CL_KERNEL_ARG_ADDRESS_PRIVATE},
{2, "height", "uint", CL_KERNEL_ARG_ADDRESS_PRIVATE},
{3, "dx", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE},
{4, "dy", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE},
{5, "zoom", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE},
};
