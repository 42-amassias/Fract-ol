/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 06:08:44 by amassias          #+#    #+#             */
/*   Updated: 2023/12/25 06:40:51 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file utils.h
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @copyright Copyright (c) 2023
 */

#ifndef UTILS_H
# define UTILS_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>

/* ************************************************************************** */
/*                                                                            */
/* Header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

/**
 * The function will read the content of the file into a buffer dynamically
 * allocated with malloc. This buffer will be stored into `*buffer_ptr`.
 * You will have to free yourself the buffer.<br>
 * The number of bytes read will be sorted into `*size_ptr`.
 * @brief Reads the file located at `path`.
 * @param path The path of the file to read.
 * @param buffer_ptr A pointer to a buffer that will hold the content of the
 * file.
 * @param size_ptr A pointer to a number that will hold the size of the file.
 * @warning This function will allocate a buffer to `*buffer_ptr`, no need to
 * allocate a buffer yourself. You will lose the pointer at `*buffer_ptr`.
 * @retval 0 if no error occured.
 * @retval -1 if the file cound not be opened.
 * @retval -2 if an error occured while reading the file.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
int	read_file_in_buffer(
		const char *path,
		char **buffer_ptr,
		size_t *size_ptr
		);

#endif