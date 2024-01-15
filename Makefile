# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 21:43:21 by amassias          #+#    #+#              #
#    Updated: 2024/01/15 17:21:18 by amassias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	cc
NAME			:=	fractol
THIRD_PARTY_DIR	:=	third-party
SRC_DIR			:=	sources
INC_DIR			:=	includes
BIN_DIR			:=	binaries

# **************************************************************************** #
#                                                                              #
# Third party libraries                                                        #
#                                                                              #
# **************************************************************************** #

LIB_FT_PATH		:=	$(THIRD_PARTY_DIR)/libft-full
LIB_FT			:=	$(LIB_FT_PATH)/libftfull.a

LIB_MLX_PATH	:=	$(THIRD_PARTY_DIR)/minilibx-linux
LIB_MLX			:=	$(LIB_MLX_PATH)/libmlx.a

# **************************************************************************** #
#                                                                              #
# Compiler flags                                                               #
#                                                                              #
# **************************************************************************** #

CFLAGS			:=					\
	-I$(INC_DIR)					\
	-I$(LIB_FT_PATH)				\
	-I$(LIB_MLX_PATH)				\
	-DCL_TARGET_OPENCL_VERSION=210	\
	-Wall -Werror -Wextra -g
LFLAGS			:=					\
	-L$(LIB_FT_PATH)				\
	-L$(LIB_MLX_PATH)				\
	-lftfull						\
	-lmlx -lXext -lX11 -lm -lz		\
	-lpthread						\
	-lOpenCL 						\

# **************************************************************************** #
#                                                                              #
# Files                                                                        #
#                                                                              #
# **************************************************************************** #

FILES			:=															\
	cl/build_kernel															\
	cl/build_kernels														\
	cl/cleanup_kernels														\
	cl/cleanup_opencl														\
	cl/get_cl_type_as_str													\
	cl/get_param_size														\
	cl/init_kernels															\
	cl/init_opencl															\
	cl/kernel_arg_info														\
	cl/kernel_info															\
	cl/prime_private_kernel_fields											\
	cl/update_arguments_on_device											\
	command_line/help/help/help__help										\
	command_line/help/print/help__print__current							\
	command_line/help/print/help__print__kernels							\
	command_line/help/print/help__print__params								\
	command_line/help/print/help__print										\
	command_line/help/help													\
	command_line/print/print												\
	command_line/print/print__current										\
	command_line/print/print__kernels										\
	command_line/print/print__params										\
	command_line/quit/quit													\
	command_line/set/set													\
	command_line/set/set__kernel											\
	command_line/set/set__kernel__name										\
	command_line/set/set__param												\
	command_line/set/set__param__name										\
	command_line/utils/change_param											\
	command_line/utils/kernel_arg_type										\
	command_line/utils/parse_command										\
	command_line/command_line												\
	mlx/cleanup_mlx															\
	mlx/init_mlx															\
	mlx/mlx_destroy															\
	utils/numbers/read_floating												\
	utils/numbers/read_integer												\
	utils/get_arg															\
	utils/print_kernel_param												\
	utils/read_file															\
	fractol_cleanup															\
	handlers																\
	globals																	\
	main																	\

SRCS			:=	$(addprefix $(SRC_DIR)/,$(addsuffix .c,$(FILES)))

OBJS			:=	$(addprefix $(BIN_DIR)/,$(addsuffix .o,$(FILES)))

# **************************************************************************** #
#                                                                              #
# Phony rules                                                                  #
#                                                                              #
# **************************************************************************** #

.PHONY: all bonus clean fclean re cleanlibs fcleanlibs norminette debug run

all: $(NAME)

bonus: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

cleanlibs:
	$(MAKE) clean -C $(LIB_FT_PATH)
	$(MAKE) clean -C $(LIB_MLX_PATH)

fcleanlibs:
	$(MAKE) fclean -C $(LIB_FT_PATH)
	$(MAKE) clean -C $(LIB_MLX_PATH)

re:	fclean all

norminette:
	@norminette												\
		`find $(SRC_DIR) -type f -name \*.c`				\
		`find $(INC_DIR) -type f -name \*.h`				\
		| grep -Ev '^Notice: |OK\!$$'						\
		&& exit 1 || printf 'Norminette OK!\n' && exit 0

debug: $(NAME)
	@valgrind							\
	--show-leak-kinds=all				\
	--leak-check=full					\
	--suppressions=valgrind_suppress	\
	./$(NAME)

run: $(NAME)
	@./$(NAME)

# **************************************************************************** #
#                                                                              #
# Binary rules                                                                 #
#                                                                              #
# **************************************************************************** #

$(NAME): $(LIB_FT) $(LIB_MLX) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

$(LIB_FT):
	$(MAKE) -C $(LIB_FT_PATH)

$(LIB_MLX):
	$(MAKE) -C $(LIB_MLX_PATH)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)