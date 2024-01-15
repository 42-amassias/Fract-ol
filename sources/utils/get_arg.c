/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:42:33 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 16:43:08 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file get_arg.c
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2024-01-15
 * @copyright Copyright (c) 2024
 */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "opencl.h"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

t_kernel_arg	*get_arg(
			t_cl *cl,
			t_kernel_mandatory_arg arg_index
			)
{
	t_kernel_arg	*args;

	args = cl->current_kernel->args;
	return (args + arg_index - KERNEL_MANDATORY_PRIVATE_ARG_COUNT);
}
