/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_kernel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:01:34 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 13:31:29 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

#include "libft.h"

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

#define MISSING_ARGUMENTS_ERROR_MESSAGE "Missing arguments. Check the README.\n"

#define ERROR__NAME "Expected argument %u to be \"%s\" but got \"%s\".\n"
#define ERROR__TYPE "Expected argument %u's type to be \"%s\" but got \"%s\".\n"
#define ERROR__QUALIFIER \
	"Expected argument %u's type qualifier to be \"%u\" but got \"%u\".\n"

#define ERROR_SUPPORT_TYPE "Unsuported type: \"%s\"\n"

/* ************************************************************************** */
/*                                                                            */
/* Helper prototypes                                                          */
/*                                                                            */
/* ************************************************************************** */

static int	_get_argument_count_and_verify_mandatory_arguments(
				t_kernel *kernel);

static int	_check_special_argument(
				const t_kernel *kernel,
				const t_arg_info *arg_info);

static int	_query_arguments(
				t_kernel *kernel);

static void	_cleanup_kernel_args(
				t_kernel_arg *arg,
				size_t count);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	build_kernel(
		t_kernel *kernel)
{
	size_t	i;
	size_t	off;
	size_t	arg_buffer_size;

	if (_get_argument_count_and_verify_mandatory_arguments(kernel))
		return (EXIT_FAILURE);
	if (_query_arguments(kernel))
		return (free(kernel->args), EXIT_FAILURE);
	i = 0;
	arg_buffer_size = 0;
	while (i < kernel->arg_count)
		arg_buffer_size = kernel->args[i++].size;
	kernel->_arg_values = (char *) ft_calloc(1, arg_buffer_size);
	if (kernel->_arg_values == NULL)
		return (_cleanup_kernel_args(kernel->args, kernel->arg_count),
			free(kernel->args), EXIT_FAILURE);
	i = 0;
	off = 0;
	while (i < kernel->arg_count)
	{
		kernel->args[i].value = (char *)kernel->_arg_values + off;
		off += kernel->args[i++].size;
	}
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static int	_get_argument_count_and_verify_mandatory_arguments(
				t_kernel *kernel)
{
	if (cl_get_kernel_info__int(
			kernel->kernel, CL_KERNEL_NUM_ARGS, &kernel->arg_count))
		return (EXIT_FAILURE);
	if (kernel->arg_count < 3)
		return (ft_printf(MISSING_ARGUMENTS_ERROR_MESSAGE), EXIT_FAILURE);
	kernel->arg_count -= 3;
	if (_check_special_argument(kernel, &g_needed_kernel_args[0])
		|| _check_special_argument(kernel, &g_needed_kernel_args[1])
		|| _check_special_argument(kernel, &g_needed_kernel_args[2]))
		return (EXIT_FAILURE);
	kernel->args = (t_kernel_arg *)
		malloc(kernel->arg_count * sizeof(t_kernel_arg));
	if (kernel->args == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	_check_special_argument(
				const t_kernel *kernel,
				const t_arg_info *arg_info)
{
	cl_kernel_arg_access_qualifier	qualifier;
	char							*str;

	if (cl_get_kernel_arg_info__str(kernel->kernel, arg_info->index,
			CL_KERNEL_ARG_NAME, (void **)&str))
		return (EXIT_FAILURE);
	if (ft_strcmp(str, arg_info->name) != 0)
		return (ft_printf(ERROR__NAME, arg_info->index, arg_info->name, str),
			free(str), EXIT_FAILURE);
	free(str);
	if (cl_get_kernel_arg_info__str(kernel->kernel, arg_info->index,
			CL_KERNEL_ARG_TYPE_NAME, (void **)&str))
		return (EXIT_FAILURE);
	if (ft_strcmp(str, arg_info->type) != 0)
		return (ft_printf(ERROR__TYPE, arg_info->index, arg_info->type, str),
			free(str), EXIT_FAILURE);
	free(str);
	if (cl_get_kernel_arg_info__int(kernel->kernel, arg_info->index,
			CL_KERNEL_ARG_ADDRESS_QUALIFIER, (void *)&qualifier))
		return (EXIT_FAILURE);
	if (qualifier != arg_info->qualifier)
		return (ft_printf(ERROR__QUALIFIER,
				arg_info->index, arg_info->qualifier, qualifier), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	_query_arguments(
				t_kernel *kernel)
{
	size_t	i;
	char	*type;

	i = 0;
	while (i < kernel->arg_count)
	{
		if (cl_get_kernel_arg_info__str(kernel->kernel, i + 3,
				CL_KERNEL_ARG_NAME, (void **)&kernel->args[i].name))
			return (_cleanup_kernel_args(kernel->args, i), EXIT_FAILURE);
		if (cl_get_kernel_arg_info__str(kernel->kernel, i + 3,
				CL_KERNEL_ARG_TYPE_NAME, (void **)&type))
			return (free((char *)kernel->name),
				_cleanup_kernel_args(kernel->args, i), EXIT_FAILURE);
		if (get_param_size(type, &kernel->args[i].size, &kernel->args[i].type))
			return (ft_printf(ERROR_SUPPORT_TYPE, type),
				free((char *)kernel->name), free(type),
				_cleanup_kernel_args(kernel->args, i), EXIT_FAILURE);
		++i;
	}
	return (EXIT_SUCCESS);
}

static void	_cleanup_kernel_args(
				t_kernel_arg *args,
				size_t count)
{
	while (count-- > 0)
		free((void *)args[count].name);
}
