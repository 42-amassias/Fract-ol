/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_kernels_initialize.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 09:40:20 by amassias          #+#    #+#             */
/*   Updated: 2023/12/26 17:42:59 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file opencl_kernels_initialize.c
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
#include "utils.h"

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief The path of the OpenCL program to load.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
#define PROGRAM_PATH "kernels/fract-ol_kernels.cl"

/**
 * @brief Build options for the OpenCL program.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
#define PROGRAM_BUILD_OPTIONS \
"-cl-kernel-arg-info -cl-fast-relaxed-math -cl-mad-enable"

/* ************************************************************************** */
/*                                                                            */
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

/**
 * If a build error occurs, a message and a log will be printed on standard
 * error.
 * @brief Build an OpenCL program.
 * @param cl A pointer to a valid OpenCL context.
 * @retval CL_CODE_SUCCESS if no error occured
 * @retval CL_ERR_PROGRAM_BUILD_FAILURE if an error occured.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
static t_cl_code	_build_program(
						t_cl *cl
						);

/**
 * @brief Initializes the program.
 * @param cl A pointer to a valid OpenCL context.
 * @retval CL_CODE_SUCCESS if no error occured
 * @retval CL_ERR_PROGRAM_INITIALIZATION_FAILURE if any error occured.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
static t_cl_code	_initialize_program(
						t_cl *cl
						);

/**
 * @brief 
 * @param cl 
 * @return t_cl_code 
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 */
static t_cl_code	_query_infos(
						t_cl *cl
						);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

t_cl_code	opencl_kernels_initialize(
				t_cl *cl
				)
{
	cl_int	code;

	code = _initialize_program(cl);
	if (code != CL_CODE_SUCCESS)
		return (code);
	code = _build_program(cl);
	if (code != CL_CODE_SUCCESS)
		return (code);
	code = _query_infos(cl);
	if (code != CL_CODE_SUCCESS)
		return (code);
	cl->kernels = (t_kernel *) ft_calloc(cl->kernel_count, sizeof(t_kernel));
	if (cl->kernels == NULL)
		return (CL_ERR_OUT_OF_MEMORY);
	return (CL_CODE_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

// TODO: cleanup in case of error for opencl_kernels_build.
static t_cl_code	_initialize_program(
						t_cl *cl
						)
{
	cl_int	code;
	char	*source;
	size_t	size;

	if (read_file_in_buffer(PROGRAM_PATH, &source, &size))
		return (CL_ERR_PROGRAM_INITIALIZATION_FAILURE);
	cl->program = clCreateProgramWithSource(
			cl->context, 1, (const char **)&source, &size, &code);
	free(source);
	if (code)
		return (CL_ERR_PROGRAM_INITIALIZATION_FAILURE);
	code = opencl_kernels_build(cl);
	if (code == CL_CODE_SUCCESS)
		return (CL_CODE_SUCCESS);
	clReleaseProgram(cl->program);
	return (code);
}

static t_cl_code	_build_program(
						t_cl *cl
						)
{
	cl_int	code;
	char	*log;
	size_t	log_size;

	code = clBuildProgram(
			cl->program, 1, &cl->device, PROGRAM_BUILD_OPTIONS, NULL, NULL);
	if (code == CL_SUCCESS)
		return (CL_CODE_SUCCESS);
	ft_fprintf(STDERR_FILENO, "Build error (%s):\n", code);
	code = clGetProgramBuildInfo(
			cl->program, cl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	if (code != CL_SUCCESS)
		return (CL_ERR_PROGRAM_BUILD_FAILURE);
	log = (char *) malloc(log_size);
	if (log == NULL)
		return (CL_ERR_OUT_OF_MEMORY);
	code = clGetProgramBuildInfo(
			cl->program, cl->device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
	if (code == CL_SUCCESS)
		ft_printf("%s\n", log);
	free(log);
	return (CL_ERR_PROGRAM_BUILD_FAILURE);
}

static t_cl_code	_query_infos(
						t_cl *cl
						)
{
	cl_int	code;
	size_t	name_set_length;

	code = clGetProgramInfo(cl->program, CL_PROGRAM_NUM_KERNELS,
			sizeof(size_t), &cl->kernel_count, NULL);
	if (code != CL_SUCCESS)
		return (CL_ERR_INFO_QUERY_FAILURE);
	code = clGetProgramInfo(cl->program, CL_PROGRAM_KERNEL_NAMES,
			0, NULL, &name_set_length);
	if (code != CL_SUCCESS)
		return (CL_ERR_INFO_QUERY_FAILURE);
	cl->_kernel_names = (char *) malloc(name_set_length);
	if (cl->_kernel_names == NULL)
		return (CL_ERR_OUT_OF_MEMORY);
	code = clGetProgramInfo(cl->program, CL_PROGRAM_KERNEL_NAMES,
			name_set_length, cl->_kernel_names, NULL);
	if (code != CL_SUCCESS)
		return (free(cl->_kernel_names), CL_ERR_INFO_QUERY_FAILURE);
	return (CL_CODE_SUCCESS);
}
