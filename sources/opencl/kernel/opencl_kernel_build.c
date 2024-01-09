/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_kernel_build.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 11:09:17 by amassias          #+#    #+#             */
/*   Updated: 2024/01/09 12:44:58 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file opencl_kernel_build.c
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

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

static int			_check_mandatory_arguments(
						const t_kernel *kernel,
						size_t index
						);

static t_cl_code	_count_arguments_and_verify_mandatory_arguments(
						t_kernel *kernel
						);

static int			_query_arguments(
						t_kernel *kernel);

void				_cleanup_kernel_args(
						t_kernel_argument *parameters,
						size_t parameter_count
						);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

t_cl_code	opencl_kernel_build(
				t_kernel *kernel
				)
{
	size_t		i;
	size_t		off;
	t_cl_code	code;

	code = _count_arguments_and_verify_mandatory_arguments(kernel);
	if (code)
		return (code);
	code = _query_arguments(kernel);
	if (code)
		return (code);
	i = 0;
	off = 0;
	while (i < kernel->arg_count)
		off += kernel->args[i++].size;
	kernel->__int__args_data = (char *) ft_calloc(1, off);
	if (kernel->__int__args_data == NULL)
		return (_cleanup_kernel_args(kernel->args, kernel->arg_count),
			CL_ERR_OUT_OF_MEMORY);
	while (i--)
	{
		off -= kernel->args[i].size;
		kernel->args[i].data = (char *)kernel->__int__args_data + off;
		kernel->args[i++].need_update_on_device = true;
	}
	return (CL_CODE_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static int	_check_mandatory_arguments(
				const t_kernel *kernel,
				size_t index
				)
{
	cl_kernel_arg_access_qualifier	type_qualifier;
	const t_kma_data				*mandatory_arg_template;
	char							*str;

	if (get_kernel_arg_info_str(kernel->cl, index,
			CL_KERNEL_ARG_NAME, (void **)&str))
		return (CL_ERR_CANNOT_GET_KERNEL_ARG_INFO);
	mandatory_arg_template = &g_kernel_mandatory_args[index];
	if (ft_strcmp(str, mandatory_arg_template->name) != 0)
		return (free(str), CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_NAME);
	free(str);
	if (get_kernel_arg_info_str(kernel->cl, index,
			CL_KERNEL_ARG_TYPE_NAME, (void **)&str))
		return (CL_ERR_CANNOT_GET_KERNEL_ARG_INFO);
	if (ft_strcmp(str, mandatory_arg_template->type) != 0)
		return (free(str), CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_TYPE);
	free(str);
	if (get_kernel_arg_info_int(kernel->cl, index,
			CL_KERNEL_ARG_TYPE_NAME, &type_qualifier))
		return (CL_ERR_CANNOT_GET_KERNEL_ARG_INFO);
	if (type_qualifier != mandatory_arg_template->type_qualifier)
		return (CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_TYPE_QUALIFIER);
	return (CL_CODE_SUCCESS);
}

static t_cl_code	_count_arguments_and_verify_mandatory_arguments(
						t_kernel *kernel
						)
{
	cl_int		cl_code;
	t_cl_code	code;
	size_t		i;

	cl_code = clGetKernelInfo(kernel->cl, CL_KERNEL_NUM_ARGS,
			sizeof(cl_uint), &kernel->arg_count, NULL);
	if (cl_code != CL_SUCCESS)
		return (CL_ERR_CANNOT_GET_KERNEL_INFO);
	if (kernel->arg_count < _KMA_COUNT)
		return (CL_ERR_KERNEL_MISSING_ARGUMENTS);
	kernel->arg_count -= KERNEL_HIDDEN_ARGUMENT_COUNT;
	i = 0;
	while (i < _KMA_COUNT)
	{
		code = _check_mandatory_arguments(kernel, i++);
		if (code)
			return (code);
	}
	kernel->args = (t_kernel_argument *)
		ft_calloc(kernel->arg_count, sizeof(t_kernel_argument));
	if (kernel->args == NULL)
		return (CL_ERR_OUT_OF_MEMORY);
	return (CL_CODE_SUCCESS);
}

static int	_query_arguments(
				t_kernel *kernel)
{
	size_t	i;
	char	*type;

	i = 0;
	while (i < kernel->arg_count)
	{
		if (get_kernel_arg_info_str(
				kernel->cl, i + KERNEL_HIDDEN_ARGUMENT_COUNT,
				CL_KERNEL_ARG_NAME, (void **)&kernel->args[i].name))
			return (_cleanup_kernel_args(kernel->args, i),
				CL_ERR_CANNOT_GET_KERNEL_ARG_INFO);
		if (get_kernel_arg_info_str(
				kernel->cl, i + KERNEL_HIDDEN_ARGUMENT_COUNT,
				CL_KERNEL_ARG_TYPE_NAME, (void **)&type))
			return (free((char *)kernel->name),
				_cleanup_kernel_args(kernel->args, i),
				CL_ERR_CANNOT_GET_KERNEL_ARG_INFO);
		if (query_param_properties(type, &kernel->args[i]))
			return (free((void *)kernel->args[i].name), free(type),
				_cleanup_kernel_args(kernel->args, i),
				CL_ERR_KERNEL_BUILD_ARGUMENT_INVALID_TYPE);
		free(type);
		++i;
	}
	return (CL_CODE_SUCCESS);
}

void	_cleanup_kernel_args(
			t_kernel_argument *parameters,
			size_t parameter_count
			)
{
	while (parameter_count--)
		free((void *)parameters[parameter_count].name);
	free(parameters);
}
