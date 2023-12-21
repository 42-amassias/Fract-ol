/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help__print__params.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 04:10:29 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 04:12:21 by amassias         ###   ########.fr       */
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
	"Unknown sub command name \"%s\" for 'print params'.\n"

#define ERROR__HELP \
	"Try 'help print params' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__help__print__params(
		const char **tokens)
{
	if (*tokens != NULL)
	{
		ft_fprintf(STDERR_FILENO, ERROR__UNKNOWN_PARAM ERROR__HELP, *tokens);
		return (EXIT_FAILURE);
	}
	ft_putstr("print params: For each parameter of a rendering kernel, will ");
	ft_putstr("print their type, name and value.\nIf no rendering kernel is");
	ft_putstr("set, will display an error accordingly.\n");
	return (EXIT_SUCCESS);
}
