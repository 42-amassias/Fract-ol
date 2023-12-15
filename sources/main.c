/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 18:27:16 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "opencl.h"
#include "mlx_wrapper.h"

#include <X11/Xlib.h>

cl_int	set_kernel_arg__int(
		cl_kernel kernel,
		cl_uint index,
		cl_int value)
{
	return (clSetKernelArg(kernel, index, sizeof(cl_int), &value));
}

cl_int	set_kernel_arg__double(
		cl_kernel kernel,
		cl_uint index,
		cl_double value)
{
	return (clSetKernelArg(kernel, index, sizeof(cl_double), &value));
}

void	change_parameter__int(
			int *value_ptr,
			const char *param_name,
			cl_kernel kernel,
			size_t kernel_index)
{
	char	*line;
	int		v;

	printf("New value for %s (current %d): ", param_name, *value_ptr);
	fflush(stdout);
	line = get_next_line(STDIN_FILENO);
	if (line == NULL)
		return ;
	v = ft_atoi(line);
	if (v > 0)
	{
		*value_ptr = v;
		set_kernel_arg__int(kernel, kernel_index, v);
	}
	free(line);
}

void	change_parameter__double(
			double *value_ptr,
			const char *param_name,
			cl_kernel kernel,
			size_t kernel_index)
{
	char	*line;
	double	v;

	printf("New value for %s (current %lf): ", param_name, *value_ptr);
	fflush(stdout);
	line = get_next_line(STDIN_FILENO);
	if (line == NULL)
		return ;
	v = ft_atoi(line);
	if (v > 0)
	{
		*value_ptr = v;
		set_kernel_arg__double(kernel, kernel_index, v);
	}
	free(line);
}

static char	*param_str_buffer = NULL;

char	*get_arg_as_string(
			const t_kernel_arg *arg)
{
	char	*v;

	v = NULL;
	if (arg->type == CL_ARG_TYPE__CHAR)
		v = ft_itoa(*(cl_char *)arg->value);
	else if (arg->type == CL_ARG_TYPE__SHORT)
		v = ft_itoa(*(cl_short *)arg->value);
	else if (arg->type == CL_ARG_TYPE__INT)
		v = ft_itoa(*(cl_int *)arg->value);
	else if (arg->type == CL_ARG_TYPE__LONG)
		v = ft_itoa(*(cl_long *)arg->value);
	else if (arg->type == CL_ARG_TYPE__UCHAR)
		v = ft_itoa(*(cl_uchar *)arg->value);
	else if (arg->type == CL_ARG_TYPE__USHORT)
		v = ft_itoa(*(cl_ushort *)arg->value);
	else if (arg->type == CL_ARG_TYPE__UINT)
		v = ft_itoa(*(cl_uint *)arg->value);
	else if (arg->type == CL_ARG_TYPE__ULONG)
		v = ft_itoa(*(cl_ulong *)arg->value);
	// else if (arg->type == CL_ARG_TYPE__FLOAT)
	// 	v = ft_ftoa(*(cl_float *)arg->value);
	// else if (arg->type == CL_ARG_TYPE__DOUBLE)
	// 	v = ft_ftoa(*(cl_double *)arg->value);
	return (v);
}

int	handle_keys(
		int keycode,
		t_fractol *fractol)
{
	if (keycode == 'p')
	{
		size_t			i;
		char			*tmp;
		static size_t	size;
		char			*padd;

		if (param_str_buffer != NULL)
			free(param_str_buffer);
		i = 0;
		size = 0;
		param_str_buffer = malloc(1);
		param_str_buffer[0] = '\0';
		while (i < fractol->cl.current_kernel->arg_count)
		{
			padd = ft_strdup("               ");
			ft_memcpy(padd, fractol->cl.current_kernel->args[i].name, ft_strlen(fractol->cl.current_kernel->args[i].name));
			tmp = ft_strcat(param_str_buffer, padd);
			free(padd);
			free(param_str_buffer);
			param_str_buffer = tmp;
			tmp = ft_strcat(param_str_buffer, " : ");
			free(param_str_buffer);
			param_str_buffer = tmp;
			padd = malloc(128);
			if (fractol->cl.current_kernel->args[i].type == CL_ARG_TYPE__DOUBLE)
				sprintf(padd, "%lf\n", *(cl_double *)fractol->cl.current_kernel->args[i].value);
			else if (fractol->cl.current_kernel->args[i].type == CL_ARG_TYPE__INT)
				sprintf(padd, "%d\n", *(cl_int *)fractol->cl.current_kernel->args[i].value);
			else
				sprintf(padd, "x\n");
			tmp = ft_strcat(param_str_buffer, padd);
			free(padd);
			free(param_str_buffer);
			param_str_buffer = tmp;
			++i;
		}
	}
	else if (keycode == 'q')
		mlx_loop_end(fractol->mlx.mlx);
	else if (keycode == 'i')
		change_parameter__int(
			&fractol->params.max_itr, "MAX_ITR",
			fractol->cl.current_kernel->kernel, 4);
	else if (keycode == 'k')
		change_parameter__double(
			&fractol->params.k, "K",
			fractol->cl.current_kernel->kernel, 5);
	else if (keycode == 'x')
		change_parameter__double(
			&fractol->params.dx, "x",
			fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DX);
	else if (keycode == 'y')
		change_parameter__double(
			&fractol->params.dy, "y",
			fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DY);
	return (0);
}

int	handle_mouse(
		int button,
		int x,
		int y,
		t_fractol *fractol)
{
	if (button == 1)
	{
		fractol->params.dx += ((double)WIDTH / (double)HEIGHT)
			* (((double)(4 * x) / (double)(WIDTH - 1)) - 2.)
			/ fractol->params.zoom;
		fractol->params.dy += (((double)(4 * y) / (double)(HEIGHT - 1)) - 2.)
			/ fractol->params.zoom;
		set_kernel_arg__double(
			fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DX, fractol->params.dx);
		set_kernel_arg__double(
			fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DY, fractol->params.dy);
	}
	if (button == 4)
		fractol->params.zoom *= 1.1;
	if (button == 5)
		fractol->params.zoom /= 1.1;
	if (button == 4 || button == 5)
		set_kernel_arg__double(fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__ZOOM, fractol->params.zoom);
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

int update(
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

	error_code = clEnqueueNDRangeKernel(
			fractol->cl.command_queue, fractol->cl.current_kernel->kernel, 2,
			NULL, global_work_size, NULL,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (mlx_loop_end(fractol->mlx.mlx));
	error_code = clEnqueueReadBuffer(
			fractol->cl.command_queue,
			fractol->cl.cl_screen, CL_TRUE,
			0, WIDTH * HEIGHT * sizeof(int), fractol->mlx.screen,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (mlx_loop_end(fractol->mlx.mlx));
	mlx_put_image_to_window(
		fractol->mlx.mlx, fractol->mlx.window,
		fractol->mlx.img, 0, 0);
	if (param_str_buffer != NULL)
		mlx_string_put(fractol->mlx.mlx, fractol->mlx.window, 100, 100, 0xFFFFFF, param_str_buffer);
	return (0);
}

void	full_cleanup(
			t_fractol *fractol)
{
	cleanup_opencl(&fractol->cl);
	cleanup_mlx(&fractol->mlx);
}

int	prime_kernel_args(
		t_cl *cl,
		double *dx,
		double *dy,
		double *itr)
{
	size_t			i;
	t_kernel		*kernel;
	t_kernel_arg	*arg;

	kernel = cl->current_kernel;
	if (clSetKernelArg(kernel->kernel, 0, sizeof(cl_mem), &cl->cl_screen))
		return (EXIT_FAILURE);
	if (clSetKernelArg(kernel->kernel, 1, sizeof(cl_double), dx))
		return (EXIT_FAILURE);
	if (clSetKernelArg(kernel->kernel, 2, sizeof(cl_double), dy))
		return (EXIT_FAILURE);
	if (clSetKernelArg(kernel->kernel, 3, sizeof(cl_double), itr))
		return (EXIT_FAILURE);
	i = 0;
	arg = kernel->args;
	while (i < kernel->arg_count)
	{
		if (clSetKernelArg(kernel->kernel, i + CL_KERNEL_NEEDED_ARG_COUNT, arg->size, arg->value))
			return (EXIT_FAILURE);
		++i;
		++arg;
	}
	return (EXIT_SUCCESS);
}

void	create_handlers(
			t_handlers *handlers,
			t_fractol *fractol)
{
	handlers->update = (int (*)(void *))update;
	handlers->context.update = fractol;
	handlers->render = (int (*)(void *))render;
	handlers->context.render = fractol;
	handlers->keyboard = (int (*)(int, void *))handle_keys;
	handlers->context.keyboard = fractol;
	handlers->mouse = (int (*)(int, int, int, void *))handle_mouse;
	handlers->context.mouse = fractol;
}

int	main(void)
{
	static t_fractol	fractol = {0};
	static t_handlers	handlers = {0};

	fractol.params.dx = -0.744567;
	fractol.params.dy = 0.121201;
	fractol.params.zoom = 1.;
	fractol.params.max_itr = 1024;
	fractol.params.k = log(2);
	create_handlers(&handlers, &fractol);
	if (init_mlx(&fractol.mlx, WIDTH, HEIGHT, &handlers))
		return (EXIT_FAILURE);
	if (init_opencl(&fractol.cl, fractol.mlx.screen, WIDTH * HEIGHT))
		return (cleanup_opencl(&fractol.cl), EXIT_FAILURE);
	*(cl_int *)fractol.cl.current_kernel->args[0].value = 1024;
	*(cl_double *)fractol.cl.current_kernel->args[1].value = log(2);
	if (prime_kernel_args(&fractol.cl, &fractol.params.dx, &fractol.params.dy, &fractol.params.zoom) != EXIT_SUCCESS)
		return (cleanup_opencl(&fractol.cl), EXIT_FAILURE);
	mlx_loop(fractol.mlx.mlx);
	full_cleanup(&fractol);
	return (fractol.error.code);
}
