/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_kernels_build.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 10:25:47 by amassias          #+#    #+#             */
/*   Updated: 2023/12/26 17:17:13 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file opencl_kernels_build.c
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
#include "_opencl.h"

/* ************************************************************************** */
/*                                                                            */
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

static char	*_get_next_kernel_name(
				char *kernel_name
				);

static void	_cleanup_kernels(
				t_cl *cl,
				size_t count
				);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

t_cl_code	opencl_kernels_build(
				t_cl *cl
				)
{
	cl_int		cl_code;
	t_cl_code	code;
	size_t		i;
	char		*kernel_name;

	i = 0;
	while (i < cl->kernel_count)
	{
		cl->kernels[i].name = kernel_name;
		kernel_name = _get_next_kernel_name(kernel_name);
		cl->kernels[i].cl = clCreateKernel(cl->program, cl->kernels[i].name,
				&cl_code);
		if (cl_code != CL_SUCCESS)
			return (_cleanup_kernels(cl, i), CL_ERR_KERNEL_CREATION_FAILURE);
		code = opencl_kernel_build(&cl->kernels[i]);
		if (code)
			return (_cleanup_kernels(cl, i), code);
		++i;
	}
	return (CL_SUCCESS);
}

static char	*_get_next_kernel_name(
				char *kernel_name
				)
{
	while (*kernel_name && *kernel_name != ';')
		++kernel_name;
	*kernel_name = '\0';
	if (kernel_name)
		return (kernel_name + 1);
	return (kernel_name);
}

static void	_cleanup_kernels(
				t_cl *cl,
				size_t count
				)
{
	while (count--)
		clReleaseKernel(cl->kernels[count].cl);
}
