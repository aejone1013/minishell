/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:40:18 by jaoh              #+#    #+#             */
/*   Updated: 2025/01/22 16:49:24 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

typedef struct s_ctx	t_ctx;
typedef struct s_env	t_env;

typedef struct s_args
{
	char			*value;
	struct s_args	*next;
}	t_args;

typedef struct s_filenames
{
	char				*path;
	t_token_type		type;
	struct s_filenames	*next;
}	t_filenames;

typedef struct s_exec
{
	char			*cmd;
	t_args			*args;
	t_filenames		*redirs;
	struct s_exec	*next;
	int				fd_in;
	int				fd_out;
}	t_exec;

/* exec main */
int		exec(t_ctx *ctx);
int		exec_2(t_ctx *ctx);
void	ex_set_stdfds(t_ctx *ctx, int mode);
void	ex_close_all(t_ctx *ctx, int pipe[]);
void	ex_close(int *fd);
void	ex_wait_all(t_ctx *ctx);

/* fdio utils*/
int		ex_init_fdio(t_exec *exec);
int		ex_handle_files(t_exec *exec);
void	ex_redir_files(t_exec *exec, t_filenames *file);

/* pipe utils */
void	ex_create_pipe(int fd_pipe[2]);
void	ex_do_child(t_ctx *ctx, t_exec *exec);
void	ex_do_child2(t_ctx *ctx, t_exec *exec, int fd_pipe[]);
void	ex_dup2_close(int fd1, int fd2);
int		ex_is_abs_path(char *file);

/* child_utils */
int		ex_do_exec(t_ctx *ctx, char *cmd, t_args *args);
char	*ex_get_path(char *file, t_env *env);
char	*ex_get_exec(char **paths, char *file);
char	**ex_get_cmds(char *cmd, t_args *args);
char	**ex_get_envs(t_env *env);

/* err_utils */
void	ex_err1_open(int err_no, char *file);
void	ex_err2_pipe(int err_no);
void	ex_err3_fork(int err_no);
void	ex_err4_exec(char *path, int err_no);
void	ex_err_coredump(int pid);
void	ex_unlink_all(t_ctx *ctx);

#endif