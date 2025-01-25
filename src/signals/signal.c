/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:11:30 by jaoh              #+#    #+#             */
/*   Updated: 2025/01/25 15:53:37 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sig_event(void)
{
	return (EXIT_SUCCESS);
}

void	sig_init_signals(void)
{
	rl_event_hook = sig_event;
	signal(SIGINT, sig_int_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	sig_int_handler(int status)
{
	(void)status;
	if (g_signals.signal_code != 1)
	{
		g_signals.signal_code = SIGNAL_OFFSET + SIGINT;
		write(STDIN_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_exec(int status)
{
	(void)status;
	g_signals.signal_code = SIGNAL_OFFSET + SIGINT;
	write(STDERR_FILENO, "\n", 1);
}

void	sig_heredoc(int status)
{
	(void)status;
	rl_replace_line("", 0);
	rl_redisplay();
	rl_done = 1;
	g_signals.end_heredoc = 1;
	g_signals.signal_code = SIGNAL_OFFSET + SIGINT;
}
