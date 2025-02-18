/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_builtin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 09:09:45 by jaoh              #+#    #+#             */
/*   Updated: 2025/02/18 16:20:16 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_unset(t_data *data, t_args *args)
{
	if (!args)
		return (0);
	else
	{
		while (args)
		{
			if (bi_del_var(args->value, &(data->envp)))
				return (1);
			args = args->next;
		}
	}
	return (0);
}

int	bi_env(t_data *data, t_args *args)
{
	t_env	*tmp;

	tmp = data->envp;
	if (args)
	{
		bi_err_env(args->value);
		return (127);
	}
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->id, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

int	bi_exit(t_data *data, t_args *args)
{
	int		exit_code;

	if (args && args->next && !bi_check_exitcode(args->value))
		return (ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO), 1);
	exit_code = 0;
	if (args && !bi_check_exitcode(args->value))
		exit_code = ft_atoi(args->value);
	else if (args && bi_check_exitcode(args->value))
	{
		bi_err_exit(args->value);
		exit_code = 2;
	}
	ex_close_all_fds(data, NULL);
	ms_free_all(data);
	exit(exit_code);
}
