/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_wrapper.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:00:38 by amassias          #+#    #+#             */
/*   Updated: 2024/01/15 15:48:08 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_WRAPPER_H
# define MLX_WRAPPER_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# include <mlx.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

# define WINDOW_TITLE "fract-ol"

# define WINDOW_X_FONT "-*-*-r-normal-*-12-120-*-*-*-*-*-*"

/* ************************************************************************** */
/*                                                                            */
/* Enums                                                                      */
/*                                                                            */
/* ************************************************************************** */

enum e_mouse_button
{
	MOUSE_BUTTON_LEFT = 1,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_WHEEL_UP = 4,
	MOUSE_BUTTON_WHEEL_DOWN = 5,
};

/* ************************************************************************** */
/*                                                                            */
/* Structures                                                                 */
/*                                                                            */
/* ************************************************************************** */

struct	s_handlers
{
	int		(*update)(void *);
	int		(*render)(void *);
	int		(*mouse)(int, int, int, void *);
	int		(*keyboard)(int, void *);
	struct s_context
	{
		void	*update;
		void	*render;
		void	*mouse;
		void	*keyboard;
	}	context;
};

struct s_mlx
{
	void				*mlx;
	void				*window;
	void				*img;
	char				*screen;
	struct s_handlers	handlers;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_mlx		t_mlx;

typedef struct s_handlers	t_handlers;

typedef int					(*t_mlx_updater)(void *);

typedef int					(*t_mlx_renderer)(void *);

typedef int					(*t_mlx_keys_handler)(int, void *);

typedef int					(*t_mlx_mouse_handler)(int, int, int, void *);

/* ************************************************************************** */
/*                                                                            */
/* header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

int		init_mlx(
			t_mlx *mlx,
			unsigned int width,
			unsigned int height);

int		mlx_destroy(
			t_mlx *mlx);

void	cleanup_mlx(
			t_mlx *mlx);

#endif