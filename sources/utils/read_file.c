/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:26:57 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 13:27:31 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

#include <stdio.h>

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	read_file(
		const char *file_path,
		char **file_buffer_ptr,
		size_t *file_size)
{
	FILE	*file;

	file = fopen(file_path, "rb");
	if (file == NULL)
		return (EXIT_FAILURE);
	fseek(file, 0, SEEK_END);
	*file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	*file_buffer_ptr = (char *) malloc(*file_size + 1);
	if (*file_buffer_ptr == NULL)
		return (EXIT_FAILURE);
	if (fread(*file_buffer_ptr, 1, *file_size, file) != *file_size)
		return (free(*file_buffer_ptr), EXIT_FAILURE);
	(*file_buffer_ptr)[*file_size] = '\0';
	fclose(file);
	return (EXIT_SUCCESS);
}
