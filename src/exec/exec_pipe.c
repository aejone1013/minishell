/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:10:05 by jaoh              #+#    #+#             */
/*   Updated: 2025/02/09 16:27:38 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ex_create_pipe(int fd_pipe[2])
{
	if (pipe(fd_pipe) == -1)
		ex_err_pipe(errno);
}

void	ex_setup_child(t_data *data, t_exec *exec)
{
	int		fd_pipe[2];

	fd_pipe[0] = -1;
	fd_pipe[1] = -1;
	ex_create_pipe(fd_pipe);
	signal(SIGINT, sig_exec);
	data->pids[data->pid_count] = fork();
	if (data->pids[data->pid_count] == -1)
		ex_err_fork(errno);
	else if (!data->pids[data->pid_count])
		ex_execute_child(data, exec, fd_pipe);
	else
	{
		if (fd_pipe[0] != -1)
			dup2(fd_pipe[0], STDIN_FILENO);
	}
	ex_close_all_fds(NULL, fd_pipe);
}

void	ex_execute_child(t_data *data, t_exec *exec, int fd_pipe[])
{
	int	exit_code;

	exit_code = 0;
	signal(SIGQUIT, SIG_DFL);
	if (exec->next && exec->fd_out == STDOUT_FILENO)
	{
		if (fd_pipe[1] != -1)
			dup2(fd_pipe[1], STDOUT_FILENO);
	}
	if (ex_init_redir(exec))
	{
		ex_close_all_fds(data, fd_pipe);
		ms_free_all(data);
		exit(EXIT_FAILURE);
	}
	ex_close_all_fds(data, fd_pipe);
	if (bi_is_builtin(exec->cmd))
	{
		exit_code = bi_do_builtin(data, exec->cmd, exec->args);
		ms_free_all(data);
		exit(exit_code);
	}
	exit_code = ex_do_exec(data, exec->cmd, exec->args);
	ms_free_all(data);
	if (exit_code == -2)
		exit(IS_A_DIRECTORY);
	exit(COMMAND_NOT_FOUND);
}

void	ex_dup2_close(int fd1, int fd2)
{
	dup2(fd1, fd2);
	close(fd1);
}

int	ex_is_abs_path(char *file)
{
	while (*file)
	{
		if (*file++ == '/')
			return (1);
	}
	return (0);
}
