/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set__param__name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:04:44 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:17:22 by amassias         ###   ########.fr       */
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
	"Error: Extra parameters for 'set param [PARAMETER_NAME]'.\n"

#define ERROR__UNKNOWN_PARAM \
	"Unknown parameter name \"%s\".\n"

#define ERROR__HELP \
	"Try 'help set param' for more information.\n"

#define WARNING__NO_KERNEL \
	"No kernel is currently selected. Use \"help set kernel\" for more infos.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__set__param__name(
		char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[1] != NULL)
	{
		ft_putstr_fd(ERROR__EXTRA ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (cl->current_kernel == NULL)
	{
		ft_putstr(WARNING__NO_KERNEL);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < cl->current_kernel->arg_count)
	{
		if (ft_strcmp(tokens[0], cl->current_kernel->args[i].name) == 0)
			break ;
		++i;
	}
	if (i == cl->current_kernel->arg_count)
	{
		ft_printf(ERROR__UNKNOWN_PARAM ERROR__HELP, tokens[0]);
		return (EXIT_FAILURE);
	}
	return (change_param(cl->current_kernel, i));
}
