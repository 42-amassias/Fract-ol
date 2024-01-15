/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help__print__kernels.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:56:02 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 04:02:14 by amassias         ###   ########.fr       */
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

#define ERROR__UNKNOWN_PARAM \
	"Unknown sub command name \"%s\" for 'print kernels'.\n"

#define ERROR__HELP \
	"Try 'help print kernels' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__help__print__kernels(
		const char **tokens)
{
	if (*tokens != NULL)
	{
		ft_fprintf(STDERR_FILENO, ERROR__UNKNOWN_PARAM ERROR__HELP, *tokens);
		return (EXIT_FAILURE);
	}
	ft_putstr("print kernels: Prints all the available rendering kernels and ");
	ft_putstr("marks the currently used one if it is set.\n");
	ft_putstr("See 'help set kernel' to change the rendering kernel.\n");
	return (EXIT_SUCCESS);
}
