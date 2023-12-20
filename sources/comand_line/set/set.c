/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:12:14 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:16:52 by amassias         ###   ########.fr       */
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

#define ERROR__HELP \
	"Try 'help set' for more information.\n"

#define ERROR__SUB_COMMAND \
	"Unknown sub command \"%s\" for 'set'.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__set(
		char **tokens,
		t_cl *cl)
{
	if (*tokens == NULL)
	{
		ft_putstr_fd(ERROR__MISSING ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (ft_strcmp(*tokens, "kernel") == 0)
		return (command__set__kernel(tokens + 1, cl));
	if (ft_strcmp(*tokens, "param") == 0)
		return (command__set__param(tokens + 1, cl));
	ft_fprintf(STDERR_FILENO, ERROR__SUB_COMMAND ERROR__HELP, *tokens);
	return (EXIT_FAILURE);
}
