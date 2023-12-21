/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:56:57 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 00:53:17 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_wrapper.h"
#include "fractol.h"

#include <libft.h>

/* ************************************************************************** */
/*                                                                            */
/* Helper protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

static int	_loop(
				t_mlx *mlx);

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	init_mlx(
		t_mlx *mlx,
		unsigned int width,
		unsigned int height)
{
	int	_;

	mlx->mlx = mlx_init();
	if (mlx->mlx == NULL)
		return (cleanup_mlx(mlx), EXIT_FAILURE);
	mlx->window = mlx_new_window(mlx->mlx, width, height, WINDOW_TITLE);
	if (mlx->window == NULL)
		return (cleanup_mlx(mlx), EXIT_FAILURE);
	mlx->img = mlx_new_image(mlx->mlx, width, height);
	if (mlx->img == NULL)
		return (cleanup_mlx(mlx), EXIT_FAILURE);
	mlx->screen = mlx_get_data_addr(mlx->img, &_, &_, &_);
	mlx_loop_hook(mlx->mlx, _loop, mlx);
	mlx_expose_hook(mlx->window, mlx->handlers.render, mlx->handlers.context.render);
	mlx_key_hook(mlx->window, mlx->handlers.keyboard, mlx->handlers.context.keyboard);
	mlx_mouse_hook(mlx->window, mlx->handlers.mouse, mlx->handlers.context.mouse);
	mlx_set_font(mlx->mlx, mlx->window, WINDOW_X_FONT);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/* Helper implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

static int	_loop(
				t_mlx *mlx)
{
	if (mlx->handlers.update(mlx->handlers.context.update))
		return (mlx_loop_end(mlx->mlx), EXIT_FAILURE);
	if (mlx->handlers.render(mlx->handlers.context.render))
		return (mlx_loop_end(mlx->mlx), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
