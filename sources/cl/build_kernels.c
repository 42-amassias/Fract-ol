/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_kernels.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:01:34 by amassias          #+#    #+#             */
/*   Updated: 2023/12/16 05:05:54 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Helper prototypes                                                          */
/*                                                                            */
/* ************************************************************************** */

static char	*_get_next_name(
				const char *name_list);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	build_kernels(
		t_cl *cl)
{
	size_t		i;
	cl_int		error_code;
	char		*name;
	t_kernel	*kernel;

	name = cl->_kernel_names;
	i = 0;
	while (i < cl->kernel_count)
	{
		kernel = &cl->kernels[i++];
		kernel->name = name;
		kernel->_arg_values = NULL;
		kernel->kernel = clCreateKernel(
				cl->program,
				kernel->name,
				&error_code);
		if (error_code != CL_SUCCESS || build_kernel(kernel) != EXIT_SUCCESS)
		{
			ft_printf("Failed to build kernel \"%s\"\n", kernel->name);
			return (cleanup_kernels(cl, i - 1), EXIT_FAILURE);
		}
		clSetKernelArg(kernel->kernel, 0, sizeof(cl_mem), &cl->cl_screen);
		name = _get_next_name(name);
	}
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static char	*_get_next_name(
				const char *name_list)
{
	while (*name_list)
		++name_list;
	++name_list;
	return ((char *) name_list);
}
