/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:44:10 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 17:22:26 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file handlers.c
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
#include "mlx_wrapper.h"
#include "opencl.h"
#include "utils.h"

#include <X11/keysym.h>

/* ************************************************************************** */
/*                                                                            */
/* Header implementation                                                      */
/*                                                                            */
/* ************************************************************************** */

int	handle_keys(
		int keycode,
		t_fractol *fractol)
{
	if (keycode == XK_Escape || keycode == XK_q)
		mlx_loop_end(fractol->mlx.mlx);
	return (EXIT_SUCCESS);
}

int	handle_mouse(
		int button,
		int x,
		int y,
		t_fractol *fractol)
{
	t_cl	*cl;

	if (fractol->cl.current_kernel == NULL)
		return (EXIT_SUCCESS);
	cl = &fractol->cl;
	if (button == MOUSE_BUTTON_LEFT)
	{
		*(cl_double *)get_arg(cl, KERNEL_MANDATORY_ARG__DX)->value
			+= ((double)WIDTH / (double)HEIGHT)
			* (((double)(4. * x) / (double)(WIDTH - 1.)) - 2.)
			/ *(cl_double *)get_arg(cl, KERNEL_MANDATORY_ARG__ZOOM)->value;
		*(cl_double *)get_arg(cl, KERNEL_MANDATORY_ARG__DY)->value
			+= (((double)(4. * y) / (double)(HEIGHT - 1.)) - 2.)
			/ *(cl_double *)get_arg(cl, KERNEL_MANDATORY_ARG__ZOOM)->value;
		get_arg(cl, KERNEL_MANDATORY_ARG__DX)->need_update_on_device = true;
		get_arg(cl, KERNEL_MANDATORY_ARG__DY)->need_update_on_device = true;
	}
	else if (button == MOUSE_BUTTON_WHEEL_UP)
		*(cl_double *)get_arg(cl, KERNEL_MANDATORY_ARG__ZOOM)->value *= 1.1;
	else if (button == MOUSE_BUTTON_WHEEL_DOWN)
		*(cl_double *)get_arg(cl, KERNEL_MANDATORY_ARG__ZOOM)->value /= 1.1;
	if (button == MOUSE_BUTTON_WHEEL_UP || button == MOUSE_BUTTON_WHEEL_DOWN)
		get_arg(cl, KERNEL_MANDATORY_ARG__ZOOM)->need_update_on_device = true;
	return (EXIT_SUCCESS);
}

int	update(
		t_fractol *fractol)
{
	if (fractol->alive == false)
		return (EXIT_FAILURE);
	if (fractol->cl.current_kernel != NULL
		&& update_arguments_on_device(&fractol->cl))
	{
		fractol->need_redraw = true;
		fractol->alive = false;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	render(
		t_fractol *fractol)
{
	static const size_t	global_work_size[] = {WIDTH, HEIGHT};
	cl_int				error_code;

	if (fractol->cl.current_kernel == NULL)
		return (EXIT_SUCCESS);
	if (fractol->need_redraw)
	{
		error_code = clEnqueueNDRangeKernel(fractol->cl.command_queue,
				fractol->cl.current_kernel->kernel, 2, NULL, global_work_size,
				NULL, 0, NULL, NULL);
		if (error_code != CL_SUCCESS)
			return (ft_printf("Kernel failed (%d) :(\n", error_code),
				mlx_loop_end(fractol->mlx.mlx), EXIT_FAILURE);
		error_code = clEnqueueReadBuffer(fractol->cl.command_queue,
				fractol->cl.cl_screen, CL_TRUE, 0, WIDTH * HEIGHT * sizeof(int),
				fractol->mlx.screen, 0, NULL, NULL);
		if (error_code != CL_SUCCESS)
			return (ft_printf("Read buffer failed\n"),
				mlx_loop_end(fractol->mlx.mlx), EXIT_FAILURE);
		mlx_put_image_to_window(fractol->mlx.mlx, fractol->mlx.window,
			fractol->mlx.img, 0, 0);
		fractol->need_redraw = false;
	}
	return (0);
}
