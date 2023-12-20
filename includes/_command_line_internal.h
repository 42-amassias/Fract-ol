/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _command_line_internal.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 23:53:55 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:27:07 by amassias         ###   ########.fr       */
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
		char **tokens,
		t_fractol *fractol);

// -----
// SET
// -----

int	command__set(
		char **tokens,
		t_cl *cl);

int	command__set__param(
		char **tokens,
		t_cl *cl);

int	command__set__param__name(
		char **tokens,
		t_cl *cl);

int	command__set__kernel(
		char **tokens,
		t_cl *cl);

int	command__set__kernel__name(
		char **tokens,
		t_cl *cl);

// -----
// PRINT
// -----

int	command__print(
		char **tokens,
		t_cl *cl);

int	command__print__params(
		char **tokens,
		t_cl *cl);

int	command__print__current(
		char **tokens,
		t_cl *cl);

int	command__print__kernels(
		char **tokens,
		t_cl *cl);

#endif