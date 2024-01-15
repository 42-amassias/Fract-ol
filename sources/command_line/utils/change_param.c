/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:00:11 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 16:16:41 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

#include "utils.h"

#include <libft.h>

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 512

#define ERROR__INTERNAL "Internal error (read)"
#define ERROR_TYPE__F "Floating point parameter malformed.\n"
#define ERROR_TYPE__I "Integer parameter malformed.\n"

int	handle_floating(
		t_cl_arg_type type,
		char *line,
		void *value_ptr
		)
{
	const char	*end;
	double		value;

	value = read_floating(line, &end);
	if (*end != '\n' && *end != '\0')
		return (ft_fprintf(STDERR_FILENO, ERROR_TYPE__F), EXIT_FAILURE);
	if ((type & CL_ARG_TYPE__TYPE_MASK)
		== (CL_ARG_TYPE__DOUBLE & CL_ARG_TYPE__TYPE_MASK))
		*(cl_double *)value_ptr = value;
	else
		*(cl_float *)value_ptr = value;
	return (EXIT_SUCCESS);
}

int	handle_integer__signed(
		t_cl_arg_type type,
		char *line,
		void *value_ptr
		)
{
	const char	*end;
	long		value;

	value = read_integer_signed(line, &end);
	if (*end != '\n' && *end != '\0')
		return (ft_fprintf(STDERR_FILENO, ERROR_TYPE__I), EXIT_FAILURE);
	if ((type & CL_ARG_TYPE__TYPE_MASK)
		== (CL_ARG_TYPE__CHAR & CL_ARG_TYPE__TYPE_MASK))
		*(cl_char *)value_ptr = value;
	else if ((type & CL_ARG_TYPE__TYPE_MASK)
		== (CL_ARG_TYPE__SHORT & CL_ARG_TYPE__TYPE_MASK))
		*(cl_short *)value_ptr = value;
	else if ((type & CL_ARG_TYPE__TYPE_MASK)
		== (CL_ARG_TYPE__INT & CL_ARG_TYPE__TYPE_MASK))
		*(cl_int *)value_ptr = value;
	else
		*(cl_long *)value_ptr = value;
	return (EXIT_SUCCESS);
}

int	handle_integer__unsigned(
		t_cl_arg_type type,
		char *line,
		void *value_ptr
		)
{
	const char	*end;
	long		value;

	value = read_integer_unsigned(line, &end);
	if (*end != '\n' && *end != '\0')
		return (ft_fprintf(STDERR_FILENO, ERROR_TYPE__I), EXIT_FAILURE);
	if ((type & CL_ARG_TYPE__TYPE_MASK)
		== (CL_ARG_TYPE__CHAR & CL_ARG_TYPE__TYPE_MASK))
		*(cl_uchar *)value_ptr = value;
	else if ((type & CL_ARG_TYPE__TYPE_MASK)
		== (CL_ARG_TYPE__SHORT & CL_ARG_TYPE__TYPE_MASK))
		*(cl_ushort *)value_ptr = value;
	else if ((type & CL_ARG_TYPE__TYPE_MASK)
		== (CL_ARG_TYPE__INT & CL_ARG_TYPE__TYPE_MASK))
		*(cl_uint *)value_ptr = value;
	else
		*(cl_ulong *)value_ptr = value;
	return (EXIT_SUCCESS);
}

int	change_param(
		t_kernel *kernel,
		cl_uint index)
{
	char			*line;
	t_kernel_arg	*arg;
	int				code;

	code = EXIT_SUCCESS;
	arg = &kernel->args[index];
	ft_printf("%-15s %-15s = ", get_cl_type_as_str(arg->type), arg->name);
	line = get_next_line(STDIN_FILENO);
	if (line == NULL)
		return (ft_fprintf(STDERR_FILENO, ERROR__INTERNAL), EXIT_FAILURE);
	if (is_type_floating(arg->type))
		code = handle_floating(arg->type, line, arg->value);
	else if (is_type_integer__signed(arg->type))
		code = handle_integer__signed(arg->type, line, arg->value);
	else if (is_type_integer__unsigned(arg->type))
		code = handle_integer__unsigned(arg->type, line, arg->value);
	else
		ft_memset(arg->value, 0, arg->size);
	free(line);
	if (code)
		return (EXIT_FAILURE);
	arg->need_update_on_device = true;
	return (EXIT_SUCCESS);
}
