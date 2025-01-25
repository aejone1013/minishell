/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:09:53 by jaoh              #+#    #+#             */
/*   Updated: 2025/01/25 16:37:49 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ex_run_exec(t_ctx *ctx)
{
	if (ctx->exec_count == 0)
		return (0);
	ex_backup_restore_fds(ctx, 0);
	ex_run_pipeline(ctx);
	ex_backup_restore_fds(ctx, 1);
	return (0);
}

int	ex_run_pipeline(t_ctx *ctx)
{
	t_exec	*tmp;

	tmp = ctx->exec;
	if (tmp->next == NULL && bi_is_builtin(tmp->cmd))
	{
		if (ex_init_redir(tmp))
		{
			ctx->exit_code = 1;
			return (1);
		}
		if (bi_is_builtin(tmp->cmd) == 2)
			ft_putstr_fd("exit\n", STDERR_FILENO);
		ex_unlink_heredoc(ctx);
		ctx->exit_code = bi_do_builtin(ctx, tmp->cmd, tmp->args);
		return (0);
	}
	while (tmp)
	{
		ex_setup_child(ctx, tmp);
		ctx->pid_count++;
		tmp = tmp->next;
	}
	ex_wait_child(ctx);
	return (0);
}

void	ex_backup_restore_fds(t_ctx *ctx, int mode)
{
	if (!mode)
	{
		ctx->def_in = dup(STDIN_FILENO);
		ctx->def_out = dup(STDOUT_FILENO);
	}
	else
	{
		ex_dup2_close(ctx->def_in, STDIN_FILENO);
		ex_dup2_close(ctx->def_out, STDOUT_FILENO);
	}
}

void	ex_close_all_fds(t_ctx *ctx, int pipe[])
{
	if (ctx)
	{
		ex_close(&(ctx->def_in));
		ex_close(&(ctx->def_out));
	}
	if (pipe)
	{
		ex_close(&(pipe[0]));
		ex_close(&(pipe[1]));
	}
}

void	ex_wait_child(t_ctx *ctx)
{
	int		status;
	int		i;

	i = 0;
	while (i < ctx->pid_count)
	{
		if (waitpid(ctx->pids[i], &(status), 0))
		{
			if (WIFEXITED(status))
			{
				g_signals.signal_code = 0;
				ctx->exit_code = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					ex_err_coredump(ctx->pids[i]);
				g_signals.signal_code = SIGNAL_OFFSET + WTERMSIG(status);
			}
		}
		i++;
	}
	ex_unlink_heredoc(ctx);
}
