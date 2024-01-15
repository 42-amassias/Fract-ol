/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:03:30 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 03:49:24 by amassias         ###   ########.fr       */
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

#define ERROR__MISSING \
	"Error: Missing parameters !\n"

#define ERROR__SUB_COMMAND \
	"Unknown sub command \"%s\" for 'print'.\n"

#define ERROR__HELP \
	"Try 'help print' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__print(
		const char **tokens,
		t_cl *cl)
{
	if (tokens[0] == NULL)
	{
		ft_putstr_fd(ERROR__MISSING ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (ft_strcmp(tokens[0], "kernels") == 0 || ft_strcmp(tokens[0], "k") == 0)
		return (command__print__kernels(tokens + 1, cl));
	if (ft_strcmp(tokens[0], "current") == 0 || ft_strcmp(tokens[0], "c") == 0)
		return (command__print__current(tokens + 1, cl));
	if (ft_strcmp(tokens[0], "params") == 0 || ft_strcmp(tokens[0], "p") == 0)
		return (command__print__params(tokens + 1, cl));
	ft_fprintf(STDERR_FILENO, ERROR__SUB_COMMAND ERROR__HELP, tokens[0]);
	return (EXIT_FAILURE);
}
