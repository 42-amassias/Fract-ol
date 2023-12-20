/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set__kernel__name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:03:49 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:17:48 by amassias         ###   ########.fr       */
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

#define ERROR__EXTRA_PARAM \
	"Error: Extra parameters for 'set kernel [KERNEL_NAME]'.\n"

#define ERROR__UNKNOWN_KERNEL \
	"Unknown kernel \"%s\".\n"

#define ERROR__HELP \
	"Try 'help set kernel' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__set__kernel__name(
		char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[1] != NULL)
	{
		ft_putstr_fd(ERROR__EXTRA_PARAM ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < cl->kernel_count)
	{
		if (ft_strcmp(tokens[0], cl->kernels[i].name) == 0)
			break ;
		++i;
	}
	if (i == cl->kernel_count)
	{
		ft_printf(ERROR__UNKNOWN_KERNEL ERROR__HELP, tokens[0]);
		return (EXIT_FAILURE);
	}
	cl->current_kernel = &cl->kernels[i];
	return (EXIT_SUCCESS);
}
