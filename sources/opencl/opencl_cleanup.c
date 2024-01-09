/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 18:53:47 by amassias          #+#    #+#             */
/*   Updated: 2024/01/03 19:10:48 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file opencl_cleanup.c
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2024-01-03
 * @copyright Copyright (c) 2024
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

void	_cleanup_kernels(
			t_cl *cl
			);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

void	opencl_cleanup(
			t_cl *cl
			)
{
	_cleanup_kernels(cl);
	clReleaseMemObject(cl->screen.device_buffer);
	clReleaseProgram(cl->program);
	clReleaseCommandQueue(cl->command_queue);
	clReleaseContext(cl->context);
	clReleaseDevice(cl->device);
	free(cl->_kernel_names);
	ft_bzero(cl, sizeof(t_cl));
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

void	_cleanup_args(
			t_kernel *kernel
			)
{
	size_t	i;

	i = 0;
	while (i < kernel->arg_count)
	{
		free((void *)kernel->args[i].name);
		free((void *)kernel->args[i].type_name);
		++i;
	}
	free(kernel->args);
}

void	_cleanup_kernels(
			t_cl *cl
			)
{
	size_t	i;

	i = 0;
	while (i < cl->kernel_count)
	{
		clReleaseKernel(cl->kernels[i].cl);
		_cleanup_args(&cl->kernels[i]);
		free((void *)cl->kernels[i].name);
		free(cl->kernels[i].__int__args_data);
		++i;
	}
	free(cl->kernels);
}
