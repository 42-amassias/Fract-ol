/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_arguments_on_device.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:52:10 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 17:49:07 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	update_arguments_on_device(
		t_cl *cl,
		bool *has_updated
		)
{
	size_t	i;
	cl_int	error_code;

	i = 0;
	while (i < cl->current_kernel->arg_count)
	{
		if (!cl->current_kernel->args[i].need_update_on_device)
		{
			++i;
			continue ;
		}
		cl->current_kernel->args[i].need_update_on_device = false;
		error_code = clSetKernelArg(cl->current_kernel->kernel,
				i + KERNEL_MANDATORY_PRIVATE_ARG_COUNT,
				cl->current_kernel->args[i].size,
				cl->current_kernel->args[i].value);
		if (error_code != CL_SUCCESS)
		{
			ft_fprintf(STDERR_FILENO, "Error: Opencl internal error (%d)\n",
				error_code);
			return (EXIT_FAILURE);
		}
		++i;
		*has_updated = true;
	}
	return (EXIT_SUCCESS);
}
