/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 20:25:32 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "opencl.h"
#include "mlx_wrapper.h"

#include "_command_line_internal.h"

#include <X11/Xlib.h>

int	handle_keys(
		int keycode,
		t_fractol *fractol)
{
	static char	input_buffer[512 + 1] = {0};
	ssize_t		n;

	if (keycode == 'c')
	{
		ft_putstr("> ");
		n = read(STDIN_FILENO, input_buffer, 512);
		if (n < 0)
		{
			ft_putstr("Failed to read standard input !\n");
			return (EXIT_SUCCESS);
		}
		if (n == 0)
		{
			ft_putchar('\n');
			return (EXIT_SUCCESS);
		}
		if (input_buffer[n - 1] == '\n')
			--n;
		else
			ft_putchar('\n');
		input_buffer[n] = '\0';
		if (parse_command(input_buffer, fractol))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else if (keycode == 'q')
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
	if (button == 0)
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
	return (0);
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

int	update(
		t_fractol *fractol)
{
	(void) fractol;
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

int	main(void)
{
	if (init_mlx(&g_fractol.mlx, WIDTH, HEIGHT))
		return (EXIT_FAILURE);
	if (init_opencl(&g_fractol.cl, g_fractol.mlx.screen, WIDTH * HEIGHT))
		return (full_cleanup(&g_fractol), EXIT_FAILURE);
	if (prime_private_kernel_fields(&g_fractol.cl, WIDTH, HEIGHT))
		return (full_cleanup(&g_fractol), EXIT_FAILURE);
	mlx_loop(g_fractol.mlx.mlx);
	full_cleanup(&g_fractol);
	return (g_fractol.error.code);
}
