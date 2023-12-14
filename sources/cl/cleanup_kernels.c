/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_kernels.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 21:13:09 by amassias          #+#    #+#             */
/*   Updated: 2023/12/14 21:41:43 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

void	cleanup_kernels(
			t_cl *cl,
			size_t count)
{
	t_kernel	*kernel;

	while (count-- > 0)
	{
		kernel = &cl->kernels[count];
		while (kernel->arg_count-- > 0)
			free((void *)kernel->args[kernel->arg_count].name);
		free((void *)kernel->args);
		free((void *)kernel->name);
		free((void *)kernel->_arg_values);
		clReleaseKernel(kernel->kernel);
	}
	free((void *)cl->kernels);
}
