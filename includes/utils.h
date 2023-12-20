/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:25:53 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:29:19 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# include "opencl.h"

# include <stdlib.h>

/* ************************************************************************** */
/*                                                                            */
/* Header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

int			read_file(
				const char *file_path,
				char **file_buffer_ptr,
				size_t *file_size);

void		print_kernel_param(
				t_kernel_arg *arg);

cl_long		read_integer_signed(
				const char *str,
				const char **end_ptr);

cl_ulong	read_integer_unsigned(
				const char *str,
				const char **end_ptr);

cl_double	read_floating(
				const char *str,
				const char **end_ptr);

#endif