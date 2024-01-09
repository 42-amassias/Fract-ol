# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 21:43:21 by amassias          #+#    #+#              #
#    Updated: 2024/01/09 13:21:24 by amassias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

VALGRIND			:=	valgrind
CC					:=	cc
NAME				:=	fractol
THIRD_PARTY_DIR		:=	third-party
SRC_DIR				:=	sources
INC_DIR				:=	includes
BIN_DIR				:=	binaries

VALGRIND_SUPP_FILE	:=	valgrind_suppress
VALGRIND_LOG_FILE	:=	valgrind_log

# **************************************************************************** #
#                                                                              #
# Third party libraries                                                        #
#                                                                              #
# **************************************************************************** #

LIB_FT_PATH			:=	$(THIRD_PARTY_DIR)/libft-full
LIB_FT				:=	$(LIB_FT_PATH)/libftfull.a

LIB_MLX_PATH		:=	$(THIRD_PARTY_DIR)/minilibx-linux
LIB_MLX				:=	$(LIB_MLX_PATH)/libmlx.a

# **************************************************************************** #
#                                                                              #
# Compiler flags                                                               #
#                                                                              #
# **************************************************************************** #

CFLAGS				:=				\
	-I$(INC_DIR)					\
	-I$(LIB_FT_PATH)				\
	-I$(LIB_MLX_PATH)				\
	-DCL_TARGET_OPENCL_VERSION=210	\
	-Wall -Werror -Wextra -g -O2
LFLAGS				:=				\
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

FILES				:=														\
	opencl/kernel/opencl_kernel_build										\
	opencl/kernel/opencl_kernels_build										\
	opencl/kernel/opencl_kernels_initialize									\
	opencl/utils/opencl_get_kernel_arg_info									\
	opencl/utils/query_param_properties										\
	opencl/opencl_cleanup													\
	opencl/opencl_init														\
	utils/read_file_in_buffer												\
	globals																	\
	main																	\

SRCS				:=	$(addprefix $(SRC_DIR)/,$(addsuffix .c,$(FILES)))

OBJS				:=	$(addprefix $(BIN_DIR)/,$(addsuffix .o,$(FILES)))

# **************************************************************************** #
#                                                                              #
# Phony rules                                                                  #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re cleanlibs fcleanlibs run debug norminette 

all: $(NAME)

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

run: $(NAME)
	@./$(NAME)

debug: $(NAME)
	@$(VALGRIND)						\
	--leak-check=full					\
	--show-leak-kinds=all				\
	--suppressions=valgrind_suppress	\
	--log-fd=3							\
	./$(NAME)							\
	3> $(VALGRIND_LOG_FILE)

norminette:
	@norminette												\
		`find $(SRC_DIR) -type f -name \*.c`				\
		`find $(INC_DIR) -type f -name \*.h`				\
		| grep -Ev '^Notice: |OK\!$$'						\
		&& exit 1 || printf 'Norminette OK!\n' && exit 0

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
