/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fdio_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:10:16 by jaoh              #+#    #+#             */
/*   Updated: 2025/01/22 16:48:53 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ex_init_fdio(t_exec *exec)
{
	if (ex_handle_files(exec))
		return (1);
	return (0);
}

int	ex_handle_files(t_exec *exec)
{
	t_filenames	*tmp;

	tmp = exec->redirs;
	while (tmp)
	{
		ex_redir_files(exec, tmp);
		if (exec->fd_in == -1 || exec-> fd_out == -1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	ex_redir_files(t_exec *exec, t_filenames *file)
{
	if (file->type == INFILE || file->type == N_HEREDOC)
	{
		if (exec->fd_in != STDIN_FILENO)
			close (exec->fd_in);
		exec->fd_in = open(file->path, O_RDONLY);
		if (exec->fd_in == -1)
			ex_err1_open(errno, file->path);
		ex_dup2_close(exec->fd_in, STDIN_FILENO);
	}
	else
	{
		if (exec->fd_out != STDOUT_FILENO)
			close(exec->fd_out);
		if (file->type == OUTFILE)
			exec->fd_out = open(file->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (file->type == APPEND)
			exec->fd_out
				= open(file->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->fd_out == -1)
			ex_err1_open(errno, file->path);
		ex_dup2_close(exec->fd_out, STDOUT_FILENO);
	}
}
