/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 11:34:15 by amassias          #+#    #+#             */
/*   Updated: 2023/12/26 16:47:09 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

/* ************************************************************************** */
/*                                                                            */
/* Global variables                                                           */
/*                                                                            */
/* ************************************************************************** */

const t_kernel_argument_type_data	g_kernel_arg_types[_ARG_TYPE_COUNT] = {
[ARG_TYPE__CHAR] = {ARG_TYPE__CHAR, "char", sizeof(cl_char)},
[ARG_TYPE__SHORT] = {ARG_TYPE__SHORT, "short", sizeof(cl_short)},
[ARG_TYPE__INT] = {ARG_TYPE__INT, "int", sizeof(cl_int)},
[ARG_TYPE__LONG] = {ARG_TYPE__LONG, "long", sizeof(cl_long)},
[ARG_TYPE__UCHAR] = {ARG_TYPE__UCHAR, "uchar", sizeof(cl_uchar)},
[ARG_TYPE__USHORT] = {ARG_TYPE__USHORT, "ushort", sizeof(cl_ushort)},
[ARG_TYPE__UINT] = {ARG_TYPE__UINT, "uint", sizeof(cl_uint)},
[ARG_TYPE__ULONG] = {ARG_TYPE__ULONG, "ulong", sizeof(cl_ulong)},
[ARG_TYPE__FLOAT] = {ARG_TYPE__FLOAT, "float", sizeof(cl_float)},
[ARG_TYPE__DOUBLE] = {ARG_TYPE__DOUBLE, "double", sizeof(cl_double)},
};

const t_kma_data					g_kernel_mandatory_args[_KMA_COUNT] = {
[KMA__SCREEN] = {0, "screen", "image2d_t", CL_KERNEL_ARG_ADDRESS_GLOBAL},
[KMA__WIDTH] = {1, "width", "uint", CL_KERNEL_ARG_ADDRESS_PRIVATE},
[KMA__HEIGHT] = {2, "height", "uint", CL_KERNEL_ARG_ADDRESS_PRIVATE},
[KMA__DX] = {3, "dx", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE},
[KMA__DY] = {4, "dy", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE},
[KMA__ZOOM] = {5, "zoom", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE},
};
