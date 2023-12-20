/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print__kernels.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:02:05 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:23:57 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

#define ERROR__EXTRA \
	"Error: Extra parameters for 'print kernels'.\n"

#define ERROR__HELP \
	"Try 'help print kernels' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__print__kernels(
		char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[0] != NULL)
	{
		ft_putstr_fd(ERROR__EXTRA ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putstr("Available kernels :\n");
	i = 0;
	while (i < cl->kernel_count)
		ft_printf("\t* %s\n", cl->kernels[i++].name);
	return (EXIT_SUCCESS);
}
