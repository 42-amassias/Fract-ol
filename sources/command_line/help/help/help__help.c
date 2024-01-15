/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help__help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 03:18:21 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 03:54:46 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

#define ERROR__SUB_COMMAND \
	"Unknown sub command \"%s\" for 'help'.\n"

#define ERROR__HELP \
	"Try 'help help' for more information.\n"

int	command__help__help(
		const char **tokens)
{
	if (*tokens != NULL)
	{
		ft_fprintf(STDERR_FILENO, ERROR__SUB_COMMAND ERROR__HELP, *tokens);
		return (EXIT_FAILURE);
	}
	ft_putstr("help: Prints help relative to a command with its arguments.\n");
	return (EXIT_SUCCESS);
}
