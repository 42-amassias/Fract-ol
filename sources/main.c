/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 16:30:21 by amassias         ###   ########.fr       */
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

#include <X11/Xlib.h>

static t_kernel_arg	*get_arg(
			t_cl *cl,
			t_kernel_mandatory_arg arg_index
			)
{
	t_kernel_arg	*args;

	args = cl->current_kernel->args;
	return (args + arg_index - KERNEL_MANDATORY_PRIVATE_ARG_COUNT);
}

int	handle_keys(
		int keycode,
		t_fractol *fractol)
{
	if (keycode == 'q')
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

int	kill_program(
			t_fractol *fractol)
{
	mlx_loop_end(fractol->mlx.mlx);
	return (EXIT_FAILURE);
}

int	update_arguments_on_device(
		t_fractol *fractol)
{
	size_t	i;
	cl_int	error_code;

	i = 0;
	while (i < fractol->cl.current_kernel->arg_count)
	{
		if (fractol->cl.current_kernel->args[i].need_update_on_device)
		{
			fractol->cl.current_kernel->args[i].need_update_on_device = false;
			error_code = clSetKernelArg(fractol->cl.current_kernel->kernel,
					i + KERNEL_MANDATORY_PRIVATE_ARG_COUNT,
					fractol->cl.current_kernel->args[i].size,
					fractol->cl.current_kernel->args[i].value);
			if (error_code != CL_SUCCESS)
			{
				ft_fprintf(STDERR_FILENO, "Error: Opencl internal error (%d)\n",
					error_code);
				return (EXIT_FAILURE);
			}
			fractol->need_redraw = true;
		}
		++i;
	}
	return (EXIT_SUCCESS);
}

int	update(
		t_fractol *fractol)
{
	if (fractol->alive == false)
		return (EXIT_FAILURE);
	if (fractol->cl.current_kernel != NULL
		&& update_arguments_on_device(fractol))
	{
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
