/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 04:10:59 by amassias          #+#    #+#             */
/*   Updated: 2024/01/09 13:49:22 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file main.c
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @copyright Copyright (c) 2023
 */

#include "opencl.h"

#include <libft.h>

int	main(void)
{
	t_cl		cl;
	t_cl_code	code;

	code = opencl_init(&cl);
	if (code != CL_CODE_SUCCESS)
	{
		ft_fprintf(STDERR_FILENO, "ERROR::opencl_init: %d", code);
		return (EXIT_FAILURE);
	}
	opencl_cleanup(&cl);
	return (EXIT_SUCCESS);
}
