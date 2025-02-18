/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_builtin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:48:24 by jaoh              #+#    #+#             */
/*   Updated: 2025/02/18 16:19:56 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	else if (!ft_strcmp(cmd, "echo"))
		return (1);
	else if (!ft_strcmp(cmd, "cd"))
		return (1);
	else if (!ft_strcmp(cmd, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd, "export"))
		return (1);
	else if (!ft_strcmp(cmd, "unset"))
		return (1);
	else if (!ft_strcmp(cmd, "env"))
		return (1);
	else if (!ft_strcmp(cmd, "exit"))
		return (2);
	else
		return (0);
}

int	bi_do_builtin(t_data *data, char *cmd, t_args *args)
{
	if (!ft_strcmp(cmd, "echo"))
		return (bi_echo(args));
	else if (!ft_strcmp(cmd, "cd"))
		return (bi_cd(data, args));
	else if (!ft_strcmp(cmd, "pwd"))
		return (bi_pwd(args));
	else if (!ft_strcmp(cmd, "export"))
		return (bi_export(data, args));
	else if (!ft_strcmp(cmd, "unset"))
		return (bi_unset(data, args));
	else if (!ft_strcmp(cmd, "env"))
		return (bi_env(data, args));
	else if (!ft_strcmp(cmd, "exit"))
		return (bi_exit(data, args));
	else
		return (0);
}

int	bi_echo(t_args *args)
{
	int	n_flag;

	n_flag = 0;
	while (args && !bi_is_nflag(args->value))
	{
		n_flag = 1;
		args = args->next;
	}
	while (args)
	{
		printf("%s", args->value);
		if (args->next)
			printf("%s", " ");
		args = args->next;
	}
	if (!n_flag)
		printf("%s", "\n");
	return (0);
}

int	bi_cd(t_data *data, t_args *args)
{
	int		siz;
	char	*cwd;
	t_env	*home;

	siz = arg_lstsize(args);
	if (siz > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd: error retrieving current directory");
	home = ms_getenv("HOME", data->envp);
	if ((!siz || !ft_strcmp(args->value, "--")) && home && home->value)
		chdir(home->value);
	else if ((!siz || !ft_strcmp(args->value, "--")) && (!home || !home->value))
		return (bi_err_cd(errno, "HOME"), free(cwd), 1);
	else if (chdir(args->value) < 0)
	{
		bi_err_cd(errno, args->value);
		return (free(cwd), 1);
	}
	if (bi_update_pwd(data, cwd))
		return (free(cwd), 1);
	return (free(cwd), 0);
}

int	bi_pwd(t_args *args)
{
	char	*cwd;

	if (args && *(args->value) == '-' && (ft_strcmp(args->value, "-L")
			&& ft_strcmp(args->value, "-P")))
	{
		bi_err_pwd(args->value);
		return (2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd: error retrieving current directory");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	bi_export(t_data *data, t_args *args)
{
	int	exit_code;

	exit_code = 0;
	if (!args)
	{
		if (bi_print_export(data->envp))
			exit_code = 1;
	}
	else
	{
		while (args)
		{
			if (bi_add_var(args->value, &(data->envp)))
				exit_code = 1;
			args = args->next;
		}
	}
	return (exit_code);
}
