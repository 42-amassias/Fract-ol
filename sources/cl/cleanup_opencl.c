/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_opencl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:18:44 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 13:19:21 by amassias         ###   ########.fr       */
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

void	cleanup_opencl(
			t_cl *cl)
{
	clReleaseMemObject(cl->cl_screen);
	cleanup_kernels(cl, cl->kernel_count);
	clReleaseProgram(cl->program);
	clReleaseCommandQueue(cl->command_queue);
	clReleaseContext(cl->context);
	clReleaseDevice(cl->device);
	free(cl->platform);
}
