/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help__print__current.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:56:02 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 04:08:40 by amassias         ###   ########.fr       */
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
	"Unknown sub command name \"%s\" for 'print current'.\n"

#define ERROR__HELP \
	"Try 'help print current' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__help__print__current(
		const char **tokens)
{
	if (*tokens != NULL)
	{
		ft_fprintf(STDERR_FILENO, ERROR__UNKNOWN_PARAM ERROR__HELP, *tokens);
		return (EXIT_FAILURE);
	}
	ft_putstr("print current: Prints the currently use kernel or 'None' if ");
	ft_putstr("none is in use.\n");
	return (EXIT_SUCCESS);
}
