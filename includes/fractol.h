/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:35:58 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 00:23:31 by amassias         ###   ########.fr       */
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

# include <libft.h>
# include <math.h>
# include <mlx.h>
# include <pthread.h>
# include <stdbool.h>
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

struct s_error
{
	int			code;
	const char	*message;
};

struct s_threading
{
	struct s_is_alive
	{
		bool	renderer;
		bool	command_line;
	}	is_alive;
	pthread_t		renderer;
	pthread_t		command_line;
	pthread_mutex_t	kernel_mutex;
};

struct s_fractol
{
	t_mlx				mlx;
	t_cl				cl;
	bool				alive;
	struct s_threading	threading;
	struct s_error		error;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_error		t_error;

typedef struct s_threading	t_threading;

typedef struct s_fractol	t_fractol;

/* ************************************************************************** */
/*                                                                            */
/* Global variables                                                           */
/*                                                                            */
/* ************************************************************************** */

// extern const t_handlers		g_handlers;
// extern t_fractol			g_fractol;

/* ************************************************************************** */
/*                                                                            */
/* Header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

int	update(
		t_fractol *fractol);

int	render(
		t_fractol *fractol);

int	handle_keys(
		int keycode,
		t_fractol *fractol);

int	handle_mouse(
		int button,
		int x,
		int y,
		t_fractol *fractol);

#endif