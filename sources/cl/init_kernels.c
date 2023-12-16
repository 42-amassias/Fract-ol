/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_kernels.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 20:32:13 by amassias          #+#    #+#             */
/*   Updated: 2023/12/16 04:55:42 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

#include <libft.h>

#include "utils.h"

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

#define PROGRAM_PATH "kernels/fract-ol_kernels.cl"

#define PROGRAM_BUILD_OPTIONS "-cl-kernel-arg-info"

#define PROGRAM_BUILD_ERROR_LOG \
			"Error %d: Failed to build program executable!\nLog:\n%s\n"

/* ************************************************************************** */
/*                                                                            */
/* Helper prototypes                                                          */
/*                                                                            */
/* ************************************************************************** */

static int	_create_program(
				t_cl *cl);

static int	_build_program(
				t_cl *cl);

static int	_query_infos(
				t_cl *cl);

static void	parse_kernel_names(
				char *name_list);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	init_opencl_kernels(
		t_cl *cl)
{
	if (_create_program(cl) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (_build_program(cl) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (_query_infos(cl) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	parse_kernel_names(cl->_kernel_names);
	cl->kernels = (t_kernel *)
		ft_calloc(cl->kernel_count, sizeof(t_kernel));
	if (cl->kernels == NULL)
		return (EXIT_FAILURE);
	if (build_kernels(cl) != EXIT_SUCCESS)
	{
		free(cl->kernels);
		cl->kernels = NULL;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static int	_create_program(
				t_cl *cl)
{
	cl_int	error_code;
	char	*source;
	size_t	source_length;

	error_code = read_file(PROGRAM_PATH, &source, &source_length);
	if (error_code != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	cl->program = clCreateProgramWithSource(
			cl->context,
			1, (const char **)&source, &source_length,
			&error_code);
	free(source);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	_build_program(
				t_cl *cl)
{
	cl_int	error_code;
	char	_log[LOG_SIZE + 1];

	error_code = clBuildProgram(
			cl->program,
			1, &cl->device,
			PROGRAM_BUILD_OPTIONS,
			NULL, NULL);
	if (error_code == CL_SUCCESS)
		return (EXIT_SUCCESS);
	clGetProgramBuildInfo(
		cl->program, cl->device,
		CL_PROGRAM_BUILD_LOG, LOG_SIZE, _log,
		NULL);
	ft_printf(PROGRAM_BUILD_ERROR_LOG, error_code, _log);
	return (EXIT_FAILURE);
}

static int	_query_infos(
				t_cl *cl)
{
	cl_int	error_code;

	error_code = clGetProgramInfo(
			cl->program, CL_PROGRAM_NUM_KERNELS,
			sizeof(size_t), &cl->kernel_count,
			NULL);
	if (error_code != CL_SUCCESS || cl->kernel_count == 0)
		return (ft_printf("No kernels.\n"), EXIT_FAILURE);
	error_code = clGetProgramInfo(
			cl->program, CL_PROGRAM_KERNEL_NAMES,
			0, NULL,
			&cl->_kernel_names_size);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	cl->_kernel_names = (char *)malloc(cl->_kernel_names_size);
	if (cl->_kernel_names == NULL)
		return (EXIT_FAILURE);
	error_code = clGetProgramInfo(
			cl->program, CL_PROGRAM_KERNEL_NAMES,
			cl->_kernel_names_size, cl->_kernel_names,
			NULL);
	if (error_code != CL_SUCCESS)
		return (free(cl->_kernel_names), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	parse_kernel_names(
				char *name_list)
{
	while (*name_list)
	{
		if (*name_list == ';')
			*name_list = '\0';
		++name_list;
	}
}
