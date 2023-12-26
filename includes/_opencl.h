/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _opencl.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 14:07:45 by amassias          #+#    #+#             */
/*   Updated: 2023/12/26 17:20:03 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file _opencl.h
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 * @copyright Copyright (c) 2023
 */

#ifndef _OPENCL_H
# define _OPENCL_H

# include "opencl.h"

/* ************************************************************************** */
/*                                                                            */
/* Header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

t_cl_code	opencl_kernels_initialize(
				t_cl *cl
				);

t_cl_code	opencl_kernels_build(
				t_cl *cl
				);

t_cl_code	opencl_kernel_build(
				t_kernel *kernel
				);

int			query_param_properties(
				const char *type,
				t_kernel_argument *arg
				);

/**
 * This functions only works for `cl_kernel_arg_info` that actually return a
 * string.
 * Allocates a string at `*string_pointer` containing the wanted information
 * with malloc.<br>
 * If an error occurs, *string_pointer will be `NULL` and a non zero value will
 * be returned.
 * @brief Gets an information in a form of a string of a parameter out of an
 * opencl kernel.
 * @param kernel The opencl kerel from which to pull an information.
 * @param index The index of the kernel parameter from which to pull an
 * information.
 * @param param_name The information to pull.
 * @param string_pointer A pointer to a string.
 * @retval 0 on success.
 * @retval "non-zero value" if an error occured.
 * @see clGetKernelArgInfo to know which info will return a string.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 */
int			get_kernel_arg_info_str(
				cl_kernel kernel,
				size_t index,
				cl_kernel_arg_info param_name,
				void **string_pointer
				);

/**
 * This functions only works for `cl_kernel_arg_info` that actually return a
 * cl_int / cl_uint.
 * @brief Gets an information in a form of an int of a parameter out of an
 * opencl kernel.
 * @param kernel The opencl kerel from which to pull an information.
 * @param index The index of the kernel parameter from which to pull an
 * information.
 * @param param_name The information to pull.
 * @param int_pointer A pointer to an int.
 * @retval 0 on success.
 * @retval "non-zero value" if an error occured.
 * @see clGetKernelArgInfo to know which info will return an int.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 */
int			get_kernel_arg_info_int(
				cl_kernel kernel,
				size_t index,
				cl_kernel_arg_info param_name,
				void *int_pointer
				);

#endif