/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 05:14:29 by amassias          #+#    #+#             */
/*   Updated: 2024/01/09 13:48:58 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file opencl_init.c
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @copyright Copyright (c) 2023
 */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"
#include "_opencl.h"

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

t_cl_code	opencl_init(
				t_cl *cl
				)
{
	cl_uint	c;
	cl_int	code;

	ft_bzero(cl, sizeof(*cl));
	code = clGetPlatformIDs(1, &cl->platform, &c);
	if (c == 0 || code != CL_SUCCESS)
		return (CL_ERR_NO_PLATFORM);
	code = clGetDeviceIDs(cl->platform, CL_DEVICE_TYPE_ALL, 1, &cl->device, &c);
	if (c == 0 || code != CL_SUCCESS)
		return (opencl_cleanup(cl), CL_ERR_NO_DEVICE);
	cl->context = clCreateContext(NULL, 1, &cl->device, NULL, NULL, &code);
	if (code != CL_SUCCESS)
		return (opencl_cleanup(cl), CL_ERR_CONTEXT_INITIALIZATION_FAILURE);
	cl->command_queue = clCreateCommandQueueWithProperties(
			cl->context, cl->device, NULL, &code);
	if (code != CL_SUCCESS)
		return (opencl_cleanup(cl),
			CL_ERR_COMMAND_QUEUE_INITIALIZATION_FAILURE);
	return (CL_CODE_SUCCESS);
}
