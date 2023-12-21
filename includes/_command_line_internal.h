/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _command_line_internal.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 23:53:55 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 04:14:57 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COMMAND_LINE_INTERNAL_H
# define _COMMAND_LINE_INTERNAL_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# include "fractol.h"

/* ************************************************************************** */
/*                                                                            */
/* Unions                                                                     */
/*                                                                            */
/* ************************************************************************** */

union u_type
{
	cl_ulong	u;
	cl_long		l;
	cl_double	d;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

typedef union u_type	t_type;

/* ************************************************************************** */
/*                                                                            */
/* Header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

// Utils

int	parse_command(
		const char *input,
		t_fractol *fractol);

int	chose_new_kernel(
		t_cl *cl);

int	change_param(
		t_kernel *kernel,
		cl_uint index);

// -----
// QUIT
// -----

int	command__quit(
		const char **tokens,
		t_fractol *fractol);

// -----
// SET
// -----

int	command__set(
		const char **tokens,
		t_cl *cl);

int	command__set__param(
		const char **tokens,
		t_cl *cl);

int	command__set__param__name(
		const char **tokens,
		t_cl *cl);

int	command__set__kernel(
		const char **tokens,
		t_cl *cl);

int	command__set__kernel__name(
		const char **tokens,
		t_cl *cl);

// -----
// PRINT
// -----

int	command__print(
		const char **tokens,
		t_cl *cl);

int	command__print__params(
		const char **tokens,
		t_cl *cl);

int	command__print__current(
		const char **tokens,
		t_cl *cl);

int	command__print__kernels(
		const char **tokens,
		t_cl *cl);

// -----
// HELP
// -----

int	command__help(
		const char **tokens);

int	command__help__help(
		const char **tokens);

int	command__help__print(
		const char **tokens);

int	command__help__print__kernels(
		const char **tokens);

int	command__help__print__current(
		const char **tokens);

int	command__help__print__params(
		const char **tokens);

#endif