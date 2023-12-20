/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:56:57 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:31:36 by amassias         ###   ########.fr       */
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

	ft_memset(mlx, 0, sizeof(t_mlx));
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
	mlx_expose_hook(mlx->window, g_handlers.render, g_handlers.context.render);
	mlx_key_hook(mlx->window, g_handlers.keyboard, g_handlers.context.keyboard);
	mlx_mouse_hook(mlx->window, g_handlers.mouse, g_handlers.context.mouse);
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
	if (g_handlers.update(g_handlers.context.update))
		return (mlx_loop_end(mlx->mlx), EXIT_FAILURE);
	if (g_handlers.render(g_handlers.context.render))
		return (mlx_loop_end(mlx->mlx), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
