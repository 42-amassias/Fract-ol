/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:02:52 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 03:55:15 by amassias         ###   ########.fr       */
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

#define H1 \
	"Welcome to Fract-ol by amassias !\n"

#define H2 \
	"First, Make sure you have an opencl program file located at kernels/\n"

#define H3 \
	"fract-ol_kernels.\nAfter that, just play with the command line to change "

#define H4 \
	"the paramters of your rendering kernels and display them !\n\nThe command "

#define H5 \
	"line has 4 commands:\n\t- print\n\t- set\n\t- quit\n\t- help\nUse the "

#define H6 \
	"command \"help\" on each commands to know what they do !\n"

#define ERROR__MISSING \
	"Error: Missing parameters !\n"

#define ERROR__SUB_COMMAND \
	"Unknown command \"%s\".\n"

#define ERROR__HELP \
	"Try 'help' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__help(
		const char	**tokens)
{
	if (tokens[0] == NULL)
	{
		ft_putstr(H1 H2 H3 H4 H5 H6);
		return (EXIT_SUCCESS);
	}
	if (ft_strcmp(tokens[0], "print") == 0 || ft_strcmp(tokens[0], "p") == 0)
		return (command__help__print(tokens + 1));
	// if (ft_strcmp(tokens[0], "set") == 0 || ft_strcmp(tokens[0], "s") == 0)
	// 	return (command__help__set(tokens + 1));
	if (ft_strcmp(tokens[0], "help") == 0 || ft_strcmp(tokens[0], "h") == 0)
		return (command__help__help(tokens + 1));
	ft_fprintf(STDERR_FILENO, ERROR__SUB_COMMAND ERROR__HELP, tokens[0]);
	return (EXIT_FAILURE);
}
