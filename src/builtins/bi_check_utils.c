/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_check_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:31:23 by jaoh              #+#    #+#             */
/*   Updated: 2025/02/09 16:27:38 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	bi_check_id(char *id)
{
	int	i;

	i = 0;
	if (!*id)
		return (0);
	while (id[i])
	{
		if (i == 0 && ft_isalpha(id[i]) == 0 && id[i] != '_')
			return (0);
		if (ft_isalnum(id[i]) == 0 && id[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	bi_check_exitcode(char *value)
{
	char	*tmp;
	long	num;

	tmp = value;
	if ((*tmp == '+' || *tmp == '-') && *(tmp + 1))
		tmp++;
	while (*tmp)
	{
		if (!ft_isdigit(*tmp++))
			return (1);
	}
	num = ft_atol(value);
	if ((num > 0 && (LONG_MAX / num < 1))
		|| (num < 0 && (LONG_MIN / ft_atol(value) < 1)))
		return (1);
	return (0);
}

int	bi_update_pwd(t_data *data, char *value)
{
	t_env	*old_pwd;
	t_env	*pwd;
	char	*cwd_new;
	char	*raw;
	char	*raw2;

	cwd_new = getcwd(NULL, 0);
	old_pwd = ms_getenv("OLDPWD", data->envp);
	pwd = ms_getenv("PWD", data->envp);
	if (pwd && cwd_new)
	{
		raw2 = ft_strjoin("PWD=", cwd_new);
		if (!raw2 || bi_add_var(raw2, &data->envp))
			return (free(cwd_new), 1);
		free(raw2);
	}
	if (old_pwd && value)
	{
		raw = ft_strjoin("OLDPWD=", value);
		if (!raw || bi_add_var(raw, &data->envp))
			return (free(cwd_new), 1);
		free(raw);
	}
	free(cwd_new);
	return (0);
}

int	bi_is_nflag(char *flag)
{
	if (!ft_strncmp(flag, "-n", 2))
		flag += 2;
	else
		return (1);
	while (*flag)
	{
		if (*flag++ != 'n')
			return (1);
	}
	return (0);
}
