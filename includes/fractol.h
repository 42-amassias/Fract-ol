/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:35:58 by amassias          #+#    #+#             */
/*   Updated: 2023/12/13 20:39:16 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "mlx.h"
# include "libft.h"

# include <CL/cl.h>
# include <math.h>
# include <stdio.h>
# include <sys/mman.h>

# define WIDTH 1920
# define HEIGHT 1080

# define LOG_SIZE 4096

# define ERROR__KERNEL_ARG "Failed to "
# define ERROR__KERNEL_START ""
# define ERROR__KERNEL_BUFFER_READ ""

# define KERNEL_ARG_INDEX__BUFFER 0
# define KERNEL_ARG_INDEX__DX 1
# define KERNEL_ARG_INDEX__DY 2
# define KERNEL_ARG_INDEX__MAX_ITR 3
# define KERNEL_ARG_INDEX__K 4
# define KERNEL_ARG_INDEX__ZOOM 5

typedef struct s_kernel_arg
{
	const char		*name;
	size_t			size;
	t_cl_arg_type	type;
}	t_kernel_arg;

typedef struct s_kernel
{
	const char		*name;
	cl_kernel		kernel;
	cl_uint			arg_count;
	t_kernel_arg	*args;
	char			*_arg_values;
}	t_kernel;

typedef struct s_data
{
	struct s_error
	{
		int			code;
		const char	*message;
	}		error;
	struct s_cl
	{
		cl_platform_id		platform;
		cl_device_id		device;
		cl_context			context;
		cl_command_queue	command_queue;
		cl_program			program;
		cl_mem				cl_screen;
		t_kernel			*kernel;
		size_t				kernel_count;
		t_kernel			*kernels;
		size_t				_kernel_names_size;
		char				*_kernel_names;
	}		cl;
	struct s_mlx
	{
		char	*mlx_screen;
		void	*mlx;
		void	*window;
		void	*img;
	}		mlx;
	struct s_params
	{
		int		max_itr;
		double	k;
		double	dx;
		double	dy;
		double	zoom;
	} params;
}	t_data;

#endif