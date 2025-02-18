/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_builtin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:48:24 by jaoh              #+#    #+#             */
/*   Updated: 2025/02/18 17:13:39 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (2);
	return (0);
}


int	bi_do_builtin(t_data *data, char *cmd, t_args *args)
{
	if (!ft_strcmp(cmd, "echo"))
		return (bi_echo(args));
	if (!ft_strcmp(cmd, "cd"))
		return (bi_cd(data, args));
	if (!ft_strcmp(cmd, "pwd"))
		return (bi_pwd(args));
	if (!ft_strcmp(cmd, "export"))
		return (bi_export(data, args));
	if (!ft_strcmp(cmd, "unset"))
		return (bi_unset(data, args));
	if (!ft_strcmp(cmd, "env"))
		return (bi_env(data, args));
	if (!ft_strcmp(cmd, "exit"))
		return (bi_exit(data, args));
	return (0);
}

int	bi_echo(t_args *args)
{
	while (args)
	{
		printf("%s", args->value);
		if (args->next)
			printf("%s", " ");
		args = args->next;
	}
	printf("%s", "\n");
	return (0);
}

/*
인자가 없거나 "--"이면 HOME으로 이동
그렇지 않으면 입력된 경로로 이동
이동 후 PWD 업데이트
*/	
int	bi_cd(t_data *data, t_args *args)
{
	int		size;
	char	*cwd;
	t_env	*home;

	size = arg_lstsize(args);
	if (size > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd: error retrieving current directory");
	home = ms_getenv("HOME", data->envp);
	if ((!size || !ft_strcmp(args->value, "--")) && home && home->value)
		chdir(home->value);
	else if ((!size || !ft_strcmp(args->value, "--")) && (!home || !home->value))
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

// 현재 작업 디렉토리를 출력
// 옵션이 잘못되면 오류 처리
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
