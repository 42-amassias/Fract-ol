/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print__params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:03:05 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 03:49:50 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

#include "utils.h"

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

#define ERROR__EXTRA_PARAM \
	"Error: Extra parameters for 'print params'.\n"

#define ERROR__NO_KERNEL \
	"No kernel is currently selected. Use \"help set kernel\" for more infos.\n"

#define ERROR__HELP \
	"Try 'help print params' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__print__params(
		const char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[0] != NULL)
	{
		ft_putstr_fd(ERROR__EXTRA_PARAM ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (cl->current_kernel == NULL)
	{
		ft_putstr(ERROR__NO_KERNEL);
		return (EXIT_FAILURE);
	}
	ft_putstr("Current kernel's arguments :\n");
	i = 0;
	while (i < cl->current_kernel->arg_count)
	{
		ft_printf("\t* %-15s %-15s = ",
			get_cl_type_as_str(cl->current_kernel->args[i].type),
			cl->current_kernel->args[i].name);
		print_kernel_param(&cl->current_kernel->args[i++]);
		ft_putchar('\n');
	}
	return (EXIT_SUCCESS);
}
