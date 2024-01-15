/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 16:51:59 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "opencl.h"
#include "mlx_wrapper.h"
#include "command_line.h"
#include "utils.h"

#include <X11/Xlib.h>

void	full_cleanup(
			t_fractol *fractol)
{
	cleanup_opencl(&fractol->cl);
	cleanup_mlx(&fractol->mlx);
}

void	*rendering_software(
			t_fractol *fractol)
{
	mlx_loop(fractol->mlx.mlx);
	fractol->alive = false;
	return (NULL);
}

void	init_handlers(
			t_fractol *fractol)
{
	ft_memset(&fractol->mlx, 0, sizeof(t_mlx));
	fractol->mlx.handlers.update = (t_mlx_updater)update;
	fractol->mlx.handlers.context.update = fractol;
	fractol->mlx.handlers.render = (t_mlx_renderer)render;
	fractol->mlx.handlers.context.render = fractol;
	fractol->mlx.handlers.keyboard = (t_mlx_keys_handler)handle_keys;
	fractol->mlx.handlers.context.keyboard = fractol;
	fractol->mlx.handlers.mouse = (t_mlx_mouse_handler)handle_mouse;
	fractol->mlx.handlers.context.mouse = fractol;
}

int	init_systems(
		t_fractol *fractol)
{
	ft_bzero(fractol, sizeof(t_fractol));
	init_handlers(fractol);
	if (init_mlx(&fractol->mlx, WIDTH, HEIGHT))
	{
		ft_fprintf(STDERR_FILENO, "Failed to initialize MLX");
		return (EXIT_FAILURE);
	}
	if (init_opencl(&fractol->cl, fractol->mlx.screen, WIDTH * HEIGHT))
	{
		ft_fprintf(STDERR_FILENO, "Failed to initialize OpenCL");
		cleanup_mlx(&fractol->mlx);
		return (EXIT_FAILURE);
	}
	if (prime_private_kernel_fields(&fractol->cl, WIDTH, HEIGHT))
	{
		ft_fprintf(STDERR_FILENO, "Failed to prime OpenCL kernels");
		full_cleanup(fractol);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	main(void)
{
	t_fractol	fractol;

	if (init_systems(&fractol))
		return (EXIT_FAILURE);
	fractol.alive = true;
	if (pthread_create(&fractol.threading.renderer, NULL,
			(void *(*)(void *))rendering_software, &fractol))
	{
		perror("Renderer initialization");
		return (EXIT_FAILURE);
	}
	if (pthread_create(
			&fractol.threading.command_line, NULL,
			(void *(*)(void *))command_line_routine, &fractol))
	{
		perror("Command line initialization");
		fractol.alive = false;
		pthread_join(fractol.threading.renderer, NULL);
		return (EXIT_FAILURE);
	}
	pthread_join(fractol.threading.renderer, NULL);
	pthread_cancel(fractol.threading.command_line);
	pthread_join(fractol.threading.command_line, NULL);
	full_cleanup(&fractol);
	return (EXIT_SUCCESS);
}
