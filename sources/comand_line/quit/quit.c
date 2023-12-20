/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:12:39 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:19:28 by amassias         ###   ########.fr       */
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

// TODO: instead of `mlx_loop_end`, set a flag.

int	command__quit(
		char **tokens,
		t_fractol *fractol)
{
	if (tokens[0] != NULL)
	{
		ft_putstr_fd(ERROR_EXTRA ERROR__HELP, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putstr("Goodbye !\n");
	mlx_loop_end(fractol->mlx.mlx);
	return (EXIT_SUCCESS);
}
