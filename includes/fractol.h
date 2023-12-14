/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:35:58 by amassias          #+#    #+#             */
/*   Updated: 2023/12/14 21:47:15 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "opencl.h"

# include "mlx.h"
# include "libft.h"

# include <math.h>
# include <stdio.h>

# define WIDTH 1920
# define HEIGHT 1080

typedef struct s_data
{
	struct s_error
	{
		int			code;
		const char	*message;
	}		error;
	t_cl	cl;
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