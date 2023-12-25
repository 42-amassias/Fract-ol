/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file_in_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 06:19:06 by amassias          #+#    #+#             */
/*   Updated: 2023/12/25 06:42:39 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file read_file_in_buffer.c
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @copyright Copyright (c) 2023
 */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

#include <stdio.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief The code to return if no error occured.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
#define SUCCESS 0

/**
 * @brief The code to return if `path` is an invalid path.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
#define ERROR_OPEN -1

/**
 * @brief The code to return if an internal error occurs.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
#define ERROR_INTERNAL -2

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	read_file_in_buffer(
		const char *path,
		char **buffer_ptr,
		size_t *size_ptr
		)
{
	FILE	*stream;
	long	size;

	stream = fopen(path, "r");
	if (stream == NULL)
		return (ERROR_OPEN);
	if (fseek(stream, 0, SEEK_END))
		return (fclose(stream), ERROR_INTERNAL);
	size = ftell(stream);
	if (size < 0)
		return (fclose(stream), ERROR_INTERNAL);
	if (fseek(stream, 0, SEEK_SET))
		return (fclose(stream), ERROR_INTERNAL);
	*size_ptr = (size_t) size;
	*buffer_ptr = (char *) malloc(*size_ptr);
	if (*buffer_ptr == NULL)
		return (fclose(stream), ERROR_INTERNAL);
	size = fread(*buffer_ptr, *size_ptr, 1, stream);
	if (size < 0)
		return (free(*buffer_ptr), fclose(stream), ERROR_INTERNAL);
	fclose(stream);
	return (SUCCESS);
}
