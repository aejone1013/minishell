/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:10:21 by jaoh              #+#    #+#             */
/*   Updated: 2025/01/18 16:10:22 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exe_err1_open(int err_no, char *file)
{
	int		fd_tmp;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s: %s\n", P_NAME, file, strerror(err_no));
	exe_dup2_close(fd_tmp, STDOUT_FILENO);
}

void	exe_err2_pipe(int err_no)
{
	int		fd_tmp;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s\n", P_NAME, strerror(err_no));
	exe_dup2_close(fd_tmp, STDOUT_FILENO);
	exit(2);
}

void	exe_err3_fork(int err_no)
{
	int		fd_tmp;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s\n", P_NAME, strerror(err_no));
	exe_dup2_close(fd_tmp, STDOUT_FILENO);
	exit(3);
}

void	exe_err4_exec(char *path, int err_no)
{
	int				fd_tmp;
	struct stat		stats;

	fd_tmp = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	if (err_no == 13 && stat(path, &stats) != -1)
	{
		if (S_ISDIR(stats.st_mode) == 1)
			printf("%s: %s: Is a directory\n", P_NAME, path);
		else
			printf("%s: %s: %s\n", P_NAME, path, strerror(err_no));
	}
	else
		printf("%s: %s: command not found\n", P_NAME, path);
	exe_dup2_close(fd_tmp, STDOUT_FILENO);
}

void	exe_unlink_all(t_ctx *ctx)
{
	t_exec		*exec;
	t_filenames	*tmp;

	exec = ctx->exec;
	while (exec)
	{
		tmp = exec->redirs;
		while (tmp)
		{
			if (tmp->type == N_HEREDOC)
				unlink(tmp->path);
			tmp = tmp->next;
		}
		exec = exec->next;
	}
}
