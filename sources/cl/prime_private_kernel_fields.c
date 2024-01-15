/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prime_private_kernel_fields.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 05:06:36 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 15:27:13 by amassias         ###   ########.fr       */
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
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	prime_private_kernel_fields(
		t_cl *cl,
		cl_uint width,
		cl_uint height)
{
	size_t		i;
	cl_int		error_code;
	t_kernel	*kernel;

	kernel = cl->kernels;
	i = 0;
	while (i++ < cl->kernel_count)
	{
		error_code = clSetKernelArg(kernel->kernel,
				KERNEL_MANDATORY_ARG__BUFFER, sizeof(cl_mem), &cl->cl_screen);
		if (error_code != CL_SUCCESS)
			return (EXIT_FAILURE);
		error_code = clSetKernelArg(kernel->kernel, KERNEL_MANDATORY_ARG__WIDTH,
				sizeof(cl_uint), &width);
		if (error_code != CL_SUCCESS)
			return (EXIT_FAILURE);
		error_code = clSetKernelArg(kernel->kernel,
				KERNEL_MANDATORY_ARG__HEIGHT, sizeof(cl_uint), &height);
		if (error_code != CL_SUCCESS)
			return (EXIT_FAILURE);
		++kernel;
	}
	return (EXIT_SUCCESS);
}
