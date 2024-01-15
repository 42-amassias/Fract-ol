/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print__current.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:02:45 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 03:49:39 by amassias         ###   ########.fr       */
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
	"Error: Extra parameters for 'print current'.\n"

#define ERROR__HELP \
	"Try 'help print current' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__print__current(
		const char **tokens,
		t_cl *cl)
{
	const char	*kernel_name;

	if (tokens[0] != NULL)
	{
		ft_putstr_fd(ERROR__EXTRA ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	kernel_name = "None";
	if (cl->current_kernel != NULL)
		kernel_name = cl->current_kernel->name;
	ft_printf("Current kernel : \"%s\".\n", kernel_name);
	return (EXIT_SUCCESS);
}
