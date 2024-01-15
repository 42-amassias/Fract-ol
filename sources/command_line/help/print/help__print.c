/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help__print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:10:17 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 04:10:18 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

#define ERROR__UNKNOWN_PARAM \
	"Unknown sub command name \"%s\" for 'print'.\n"

#define ERROR__HELP \
	"Try 'help print' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

int	command__help__print(
		const char **tokens)
{
	if (*tokens == NULL)
	{
		ft_putstr("print: Prints data relative to the software.\n");
		ft_putstr("It takes at least one argument:\n");
		ft_putstr("\t- kernels (k)\n\t- current (c)\n\t- params (p)\n");
		ft_putstr("Use 'help print [SUB_COMMAND]' to know more.\n");
		return (EXIT_SUCCESS);
	}
	if (ft_strcmp(*tokens, "k") == 0 || ft_strcmp(*tokens, "kernels") == 0)
		return (command__help__print__kernels(tokens + 1));
	if (ft_strcmp(*tokens, "c") == 0 || ft_strcmp(*tokens, "current") == 0)
		return (command__help__print__current(tokens + 1));
	if (ft_strcmp(*tokens, "p") == 0 || ft_strcmp(*tokens, "params") == 0)
		return (command__help__print__params(tokens + 1));
	ft_fprintf(STDERR_FILENO, ERROR__UNKNOWN_PARAM ERROR__HELP, *tokens);
	return (EXIT_FAILURE);
}
