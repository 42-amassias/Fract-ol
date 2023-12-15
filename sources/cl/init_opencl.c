/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 20:21:31 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 17:49:30 by amassias         ###   ########.fr       */
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
/* Helper prototypes                                                          */
/*                                                                            */
/* ************************************************************************** */

static int	cl_get_platform_id(
				cl_platform_id *id);

static int	cl_get_device_id(
				cl_platform_id platform_id,
				cl_device_id *id);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

// TODO: maybe move buffer initialization elsewhere ?
int	init_opencl(
		t_cl *cl,
		char *screen_back_buffer,
		size_t pixel_count)
{
	cl_int	error_code;

	if (cl_get_platform_id(&cl->platform) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (cl_get_device_id(cl->platform, &cl->device) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	cl->context = clCreateContext(
			NULL, 1, &cl->device,
			NULL, NULL,
			&error_code);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	cl->command_queue = clCreateCommandQueue(
			cl->context, cl->device, 0,
			&error_code);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	cl->cl_screen = clCreateBuffer(
			cl->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			pixel_count * sizeof(int), screen_back_buffer,
			&error_code);
	if (error_code != CL_SUCCESS || init_opencl_kernels(cl) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static int	cl_get_platform_id(
		cl_platform_id *id)
{
	cl_uint			platform_count;

	if (clGetPlatformIDs(1, id, &platform_count) != CL_SUCCESS)
		return (EXIT_FAILURE);
	if (platform_count <= 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	cl_get_device_id(
		cl_platform_id platform_id,
		cl_device_id *id)
{
	cl_uint			device_count;

	if (clGetDeviceIDs(platform_id,
			CL_DEVICE_TYPE_ALL, 1, id, &device_count) != CL_SUCCESS)
		return (EXIT_FAILURE);
	if (device_count <= 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
