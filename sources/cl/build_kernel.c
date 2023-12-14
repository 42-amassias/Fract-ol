/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_kernel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:01:34 by amassias          #+#    #+#             */
/*   Updated: 2023/12/14 21:57:39 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

#include "libft.h"

#define MISSING_ARGUMENTS_ERROR_MESSAGE "Missing arguments. Check the README.\n"

#define ERROR__NAME "Expected argument %u to be \"%s\" but got \"%s\".\n"
#define ERROR__TYPE "Expected argument %u's type to be \"%s\" but got \"%s\".\n"
#define ERROR__QUALIFIER \
	"Expected argument %u's type qualifier to be \"%u\" but got \"%u\".\n"

static int	_get_argument_count_and_verify_mandatory_arguments(
				t_kernel *kernel);

static int	_check_special_argument(
				const t_kernel *kernel,
				const t_arg_info *arg_info);

int	build_kernel(
		t_kernel *kernel)
{
	size_t	i;
	size_t	off;
	size_t	size;
	size_t	arg_buffer_size;
	char	type[512];
	cl_int	error_code;

	if (_get_argument_count_and_verify_mandatory_arguments(kernel))
		return (EXIT_FAILURE);
	kernel->args = (t_kernel_arg *)
		malloc(kernel->arg_count * sizeof(t_kernel_arg));
	if (kernel->args == NULL)
		return (EXIT_FAILURE);
	arg_buffer_size = 0;
	i = 0;
	while (i < kernel->arg_count)
	{
		clGetKernelArgInfo(kernel->kernel, i + 3, CL_KERNEL_ARG_NAME, 0, NULL, &size);
		kernel->args[i].name = (const char *) malloc(size * sizeof(char));
		if (kernel->args[i].name == NULL)
		{
			while (i-- > 0)
				free((void *)kernel->args[i].name);
			return (free(kernel->args), EXIT_FAILURE);
		}
		clGetKernelArgInfo(kernel->kernel, i + 3, CL_KERNEL_ARG_NAME, size, (void *)kernel->args[i].name, NULL);
		clGetKernelArgInfo(kernel->kernel, i + 3, CL_KERNEL_ARG_TYPE_NAME, 512, type, NULL);
		if (get_param_size(type, &kernel->args[i].size, &kernel->args[i].type) != EXIT_SUCCESS)
		{
			ft_printf("Unsuported type: \"%s\"\n", type);
			while (i-- > 0)
				free((void *)kernel->args[i].name);
			return (free(kernel->args), EXIT_FAILURE);
		}
		arg_buffer_size += kernel->args[i].size;
		printf("\t> %-10s %-15s (type: %2u, size: %2zu)\n", type, kernel->args[i].name, kernel->args[i].type, kernel->args[i].size);
		++i;
	}
	kernel->_arg_values = (char *) ft_calloc(1, arg_buffer_size);
	if (kernel->_arg_values == NULL)
	{
		while (i-- > 0)
			free((void *)kernel->args[i].name);
		return (free(kernel->args), EXIT_FAILURE);
	}
	i = 0;
	off = 0;
	while (i < kernel->arg_count)
	{
		kernel->args[i].value = (char *)kernel->_arg_values + off;
		off += kernel->args[i++].size;
	}
	printf("\n");
	return (EXIT_SUCCESS);
}

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
		return (printf(ERROR__NAME, arg_info->index, arg_info->name, str),
			free(str), EXIT_FAILURE);
	free(str);
	if (cl_get_kernel_arg_info__str(kernel->kernel, arg_info->index,
			CL_KERNEL_ARG_TYPE_NAME, (void **)&str))
		return (EXIT_FAILURE);
	if (ft_strcmp(str, arg_info->type) != 0)
		return (printf(ERROR__TYPE, arg_info->index, arg_info->type, str),
			free(str), EXIT_FAILURE);
	free(str);
	if (cl_get_kernel_arg_info__int(kernel->kernel, arg_info->index,
			CL_KERNEL_ARG_ADDRESS_QUALIFIER, (void *)&qualifier))
		return (EXIT_FAILURE);
	if (qualifier != arg_info->qualifier)
		return (printf(ERROR__QUALIFIER,
				arg_info->index, arg_info->qualifier, qualifier), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}