/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 01:00:58 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: finish command line thread to get read of "_command_line_internal.h"

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
	if (fractol->cl.current_kernel == NULL)
		return (EXIT_SUCCESS);
	if (button == 1)
	{
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DX - CL_KERNEL_PRIVTAE_ARG_COUNT].value
			+= ((double)WIDTH / (double)HEIGHT)
			* (((double)(4 * x) / (double)(WIDTH - 1)) - 2.)
			/ *(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value;
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DY - CL_KERNEL_PRIVTAE_ARG_COUNT].value
			+= (((double)(4 * y) / (double)(HEIGHT - 1)) - 2.)
			/ *(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value;
		clSetKernelArg(fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DX, sizeof(cl_double), fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DX - CL_KERNEL_PRIVTAE_ARG_COUNT].value);
		clSetKernelArg(fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DY, sizeof(cl_double), fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DY - CL_KERNEL_PRIVTAE_ARG_COUNT].value);
	}
	else if (button == 4)
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value *= 1.1;
	else if (button == 5)
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value /= 1.1;
	if (button == 4 || button == 5)
		clSetKernelArg(fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__ZOOM, sizeof(cl_double), fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value);
	return (EXIT_SUCCESS);
}

int	kill_program(
			int code,
			const char *message,
			t_fractol *fractol)
{
	fractol->error.code = code;
	fractol->error.message = message;
	mlx_loop_end(fractol->mlx.mlx);
	return (code);
}

int	update_arguments_on_device(
		t_cl *cl)
{
	size_t	i;
	cl_int	error_code;
	
	i = 0;
	while (i < cl->current_kernel->arg_count)
	{
		if (cl->current_kernel->args[i].need_update_on_device)
		{
			cl->current_kernel->args[i].need_update_on_device = false;
			// pthread_mutex_lock(&fractol->threading.kernel_arg_mutex);
			error_code = clSetKernelArg(cl->current_kernel->kernel,
					i + CL_KERNEL_PRIVTAE_ARG_COUNT,
					cl->current_kernel->args[i].size,
					cl->current_kernel->args[i].value);
			// pthread_mutex_unlock(&fractol->threading.kernel_arg_mutex);
			if (error_code != CL_SUCCESS)
			{
				ft_fprintf(STDERR_FILENO, "Error: Opencl internal error (%d)\n",
					error_code);
				return (EXIT_FAILURE);
			}
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
		&& update_arguments_on_device(&fractol->cl))
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
	error_code = clEnqueueNDRangeKernel(
			fractol->cl.command_queue, fractol->cl.current_kernel->kernel, 2,
			NULL, global_work_size, NULL,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (ft_printf("Kernel failed (%d) :(\n", error_code), mlx_loop_end(fractol->mlx.mlx), EXIT_FAILURE);
	error_code = clEnqueueReadBuffer(
			fractol->cl.command_queue,
			fractol->cl.cl_screen, CL_TRUE,
			0, WIDTH * HEIGHT * sizeof(int), fractol->mlx.screen,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (ft_printf("Read buffer failed\n"), mlx_loop_end(fractol->mlx.mlx), EXIT_FAILURE);
	mlx_put_image_to_window(
		fractol->mlx.mlx, fractol->mlx.window,
		fractol->mlx.img, 0, 0);
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
	// if (init_mlx(&fractol->mlx, WIDTH, HEIGHT))
	// {
	// 	fractol->error.code = EXIT_FAILURE;
	// 	fractol->error.message = "Failed to initialize MLX";
	// 	fractol->alive = false;
	// 	return (NULL);
	// }
	// if (init_opencl(&fractol->cl, fractol->mlx.screen, WIDTH * HEIGHT))
	// {
	// 	fractol->error.code = EXIT_FAILURE;
	// 	fractol->error.message = "Failed to initialize OpenCL";
	// 	fractol->alive = false;
	// 	return (NULL);
	// }
	// if (prime_private_kernel_fields(&fractol->cl, WIDTH, HEIGHT))
	// {
	// 	fractol->error.code = EXIT_FAILURE;
	// 	fractol->error.message = "Failed to prime OpenCL kernels";
	// 	fractol->alive = false;
	// 	return (NULL);
	// }
	mlx_loop(fractol->mlx.mlx);
	fractol->alive = false;
	return (NULL);
}

void	init_handlers(
			t_fractol *fractol)
{
	ft_memset(&fractol->mlx, 0, sizeof(t_mlx));
	fractol->mlx.handlers.update = (int (*)(void *))update;
	fractol->mlx.handlers.context.update = fractol;
	fractol->mlx.handlers.render = (int (*)(void *))render;
	fractol->mlx.handlers.context.render = fractol;
	fractol->mlx.handlers.keyboard = (int (*)(int, void *))handle_keys;
	fractol->mlx.handlers.context.keyboard = fractol;
	fractol->mlx.handlers.mouse = (int (*)(int, int, int, void *))handle_mouse;
	fractol->mlx.handlers.context.mouse = fractol;
}

int	main(void)
{
	t_fractol	*fractol;
	int			error_code;

	fractol = (t_fractol *)malloc(sizeof(t_fractol));
	if (fractol == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Internal error\n");
		return (EXIT_FAILURE);
	}
	init_handlers(fractol);
	if (init_mlx(&fractol->mlx, WIDTH, HEIGHT))
	{
		ft_fprintf(STDERR_FILENO, "Failed to initialize MLX");
		return (free(fractol), EXIT_FAILURE);
	}
	if (init_opencl(&fractol->cl, fractol->mlx.screen, WIDTH * HEIGHT))
	{
		ft_fprintf(STDERR_FILENO, "Failed to initialize OpenCL");
		cleanup_mlx(&fractol->mlx);
		return (free(fractol), EXIT_FAILURE);
	}
	if (prime_private_kernel_fields(&fractol->cl, WIDTH, HEIGHT))
	{
		ft_fprintf(STDERR_FILENO, "Failed to prime OpenCL kernels");
		full_cleanup(fractol);
		return (free(fractol), EXIT_FAILURE);
	}
	fractol->alive = true;
	if (pthread_create(
		&fractol->threading.renderer, NULL,
		(void *(*)(void *))rendering_software, fractol))
	{
		perror("Renderer initialization");
		return (EXIT_FAILURE);
	}
	if (pthread_create(
			&fractol->threading.command_line, NULL,
			(void *(*)(void *))command_line_routine, fractol))
	{
		perror("Command line initialization");
		fractol->alive = false;
		pthread_join(fractol->threading.renderer, NULL);
		return (EXIT_FAILURE);
	}
	pthread_join(fractol->threading.renderer, NULL);
	pthread_cancel(fractol->threading.command_line);
	pthread_join(fractol->threading.command_line, NULL);
	full_cleanup(fractol);
	error_code = fractol->error.code;
	if (error_code != EXIT_SUCCESS && fractol->error.message != NULL)
		ft_fprintf(STDERR_FILENO, "%s\n", fractol->error.message);
	free(fractol);
	return (error_code);
}
