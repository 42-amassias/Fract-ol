/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:00:11 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 01:54:53 by amassias         ###   ########.fr       */
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

static int	_is_type_integer(
				t_cl_arg_type type);

static int	_is_type_floating(
				t_cl_arg_type type);

int	change_param(
		t_kernel *kernel,
		cl_uint index)
{
	char			*line;
	const char		*end;
	t_kernel_arg	*arg;
	t_type			t;

	arg = &kernel->args[index];
	ft_printf("%-15s %-15s = ", get_cl_type_as_str(arg->type), arg->name);
	line = get_next_line(STDIN_FILENO);
	if (line == NULL)
		return (ft_fprintf(STDERR_FILENO, ERROR__INTERNAL), EXIT_FAILURE);
	if (_is_type_floating(arg->type))
	{
		t.d = read_floating(line, &end);
		if (*end != '\n' && *end != '\0')
			return (free(line), ft_fprintf(STDERR_FILENO, ERROR_TYPE__F), EXIT_FAILURE);
		if ((arg->type & CL_ARG_TYPE__TYPE_MASK) == (CL_ARG_TYPE__DOUBLE & CL_ARG_TYPE__TYPE_MASK))
			*(cl_double *)arg->value = t.d;
		else
			*(cl_float *)arg->value = t.d;
	}
	else if (_is_type_integer(arg->type))
	{
		if ((arg->type & CL_ARG_TYPE__ATTR_MASK) == CL_ARG_TYPE_ATTR_SIGNED)
		{
			t.l = read_integer_signed(line, &end);
			if (*end != '\n' && *end != '\0')
				return (free(line), ft_fprintf(STDERR_FILENO, ERROR_TYPE__I), EXIT_FAILURE);
			if ((arg->type & CL_ARG_TYPE__TYPE_MASK) == (CL_ARG_TYPE__CHAR & CL_ARG_TYPE__TYPE_MASK))
				*(cl_char *)arg->value = t.l;
			else if ((arg->type & CL_ARG_TYPE__TYPE_MASK) == (CL_ARG_TYPE__SHORT & CL_ARG_TYPE__TYPE_MASK))
				*(cl_short *)arg->value = t.l;
			else if ((arg->type & CL_ARG_TYPE__TYPE_MASK) == (CL_ARG_TYPE__INT & CL_ARG_TYPE__TYPE_MASK))
				*(cl_int *)arg->value = t.l;
			else
				*(cl_long *)arg->value = t.l;
		}
		else
		{
			t.u = read_integer_unsigned(line, &end);
			if (*end != '\n' && *end != '\0')
				return (free(line), ft_fprintf(STDERR_FILENO, ERROR_TYPE__I), EXIT_FAILURE);
			if ((arg->type & CL_ARG_TYPE__TYPE_MASK) == (CL_ARG_TYPE__CHAR & CL_ARG_TYPE__TYPE_MASK))
				*(cl_uchar *)arg->value = t.u;
			else if ((arg->type & CL_ARG_TYPE__TYPE_MASK) == (CL_ARG_TYPE__SHORT & CL_ARG_TYPE__TYPE_MASK))
				*(cl_ushort *)arg->value = t.u;
			else if ((arg->type & CL_ARG_TYPE__TYPE_MASK) == (CL_ARG_TYPE__INT & CL_ARG_TYPE__TYPE_MASK))
				*(cl_uint *)arg->value = t.u;
			else
				*(cl_ulong *)arg->value = t.u;
		}
	}
	else
		ft_memset(arg->value, 0, arg->size);
	free(line);
	arg->need_update_on_device = true;
	return (EXIT_SUCCESS);
}

static int	_is_type_integer(
				t_cl_arg_type type)
{
	return ((type & CL_ARG_TYPE__CATEGORY_MASK) == CL_ARG_TYPE_INTEGER);
}

static int	_is_type_floating(
				t_cl_arg_type type)
{
	return ((type & CL_ARG_TYPE__CATEGORY_MASK) == CL_ARG_TYPE_FLOATING);
}
