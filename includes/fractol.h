/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:35:58 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 15:55:33 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# include "mlx_wrapper.h"
# include "opencl.h"

# include <mlx.h>
# include <libft.h>

# include <math.h>
# include <stdio.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

# define WIDTH 1920
# define HEIGHT 1080

/* ************************************************************************** */
/*                                                                            */
/* Structures                                                                 */
/*                                                                            */
/* ************************************************************************** */

struct s_params
{
	int		max_itr;
	double	k;
	double	dx;
	double	dy;
	double	zoom;
};

struct s_error
{
	int			code;
	const char	*message;
};

struct s_fractol
{
	t_mlx			mlx;
	t_cl			cl;
	struct s_params	params;
	struct s_error	error;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_params		t_params;

typedef struct s_error		t_error;

typedef struct s_fractol	t_fractol;

#endif