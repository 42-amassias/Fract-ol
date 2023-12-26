/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_get_kernel_arg_info.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 14:19:58 by amassias          #+#    #+#             */
/*   Updated: 2023/12/26 14:22:00 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

int	get_kernel_arg_info_str(
		cl_kernel kernel,
		size_t index,
		cl_kernel_arg_info param_name,
		void **string_pointer
		)
{
	size_t	size;

	if (clGetKernelArgInfo(kernel, index, param_name, 0, NULL, &size))
		return (EXIT_FAILURE);
	*string_pointer = (char *) malloc(size);
	if (*string_pointer == NULL)
		return (EXIT_FAILURE);
	if (clGetKernelArgInfo(kernel, index, param_name, size, *string_pointer,
			NULL))
	{
		free(*string_pointer);
		*string_pointer = NULL;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	get_kernel_arg_info_int(
		cl_kernel kernel,
		size_t index,
		cl_kernel_arg_info param_name,
		void *int_pointer
		)
{
	if (clGetKernelArgInfo(kernel, index, param_name,
			sizeof(cl_int), int_pointer, NULL))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
