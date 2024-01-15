/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 17:20:00 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 17:22:00 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file fractol_cleanup.c
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2024-01-15
 * @copyright Copyright (c) 2024
 */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "opencl.h"
#include "mlx_wrapper.h"

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

void	fractol_cleanup(
			t_fractol *fractol
			)
{
	cleanup_opencl(&fractol->cl);
	cleanup_mlx(&fractol->mlx);
}
