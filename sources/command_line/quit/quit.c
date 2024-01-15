/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:12:39 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 16:22:31 by amassias         ###   ########.fr       */
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

#define ERROR_EXTRA \
	"Error: Extra parameters for 'quit'.\n"

#define ERROR__HELP \
	"Try 'help quit' for more information.\n"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	command__quit(
		const char **tokens,
		t_fractol *fractol)
{
	if (tokens[0] != NULL)
	{
		ft_putstr_fd(ERROR_EXTRA ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putstr("Goodbye !\n");
	fractol->alive = false;
	return (EXIT_SUCCESS);
}
