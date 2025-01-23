/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:10:26 by jaoh              #+#    #+#             */
/*   Updated: 2025/01/22 16:49:26 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ex_do_exec(t_ctx *ctx, char *cmd, t_args *args)
{
	char	*path;
	char	**cmds;
	char	**envs;

	if (!cmd)
		return (0);
	path = ex_get_path(cmd, ctx->envp);
	if (!path)
		return (ex_err4_exec(cmd, errno), -1);
	envs = ex_get_envs(ctx->envp);
	if (!envs)
		return (free(path), -1);
	cmds = ex_get_cmds(cmd, args);
	if (!cmds)
		return (free(path), free(envs), -1);
	if (execve(path, cmds, envs) == -1)
	{
		ex_err4_exec(path, errno);
		ft_free_all(cmds);
		return (free(path), free(envs), -2);
	}
	ft_free_all(cmds);
	return (free(path), free(envs), 0);
}

char	*ex_get_path(char *file, t_env *env)
{
	char	**paths;
	char	*exec;
	t_env	*path_env;

	if (file && file[0] == '\0')
		return (NULL);
	if (!env || ex_is_abs_path(file) == 1)
		return (ft_strdup(file));
	path_env = ms_getenv("PATH", env);
	if (!path_env)
		return (ft_strdup(file));
	paths = ft_split(path_env->value, ':');
	if (!paths)
		return (ft_strdup(file));
	exec = ex_get_exec(paths, file);
	ft_free_all(paths);
	return (exec);
}

char	*ex_get_exec(char **paths, char *file)
{
	char	*exec;
	char	*path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/");
		if (!path)
			return (NULL);
		exec = ft_strjoin(path, file);
		free(path);
		if (!exec)
			return (NULL);
		if (!access(exec, X_OK | F_OK))
			return (exec);
		free(exec);
	}
	return (NULL);
}

char	**ex_get_cmds(char *cmd, t_args *args)
{
	char	**cmds;
	int		arg_size;
	int		i;

	i = -1;
	arg_size = arg_lstsize(args);
	cmds = (char **)malloc((arg_size + 2) * sizeof(char *));
	if (!cmds)
		return (NULL);
	while (++i < arg_size + 1)
	{
		if (!i)
			cmds[i] = ft_strdup(cmd);
		else
		{
			cmds[i] = ft_strdup(args->value);
			args = args->next;
		}
		if (!cmds[i])
			return (ft_free_all(cmds), NULL);
	}
	cmds[i] = NULL;
	return (cmds);
}

char	**ex_get_envs(t_env *env)
{
	char	**envs;
	int		env_size;
	int		i;

	i = -1;
	env_size = env_lstsize(env);
	envs = (char **)malloc((env_size + 1) * sizeof(char *));
	if (!envs)
		return (NULL);
	while (++i < env_size)
	{
		envs[i] = env->raw;
		env = env->next;
	}
	envs[i] = NULL;
	return (envs);
}
