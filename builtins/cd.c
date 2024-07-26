/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 12:19:12 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/25 23:28:45 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_change_pwd(t_minishell *g_minishell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	return (ft_update_envlst("PWD", cwd, false, g_minishell), 0);
}

static int	ft_cd_home(t_minishell *g_minishell)
{
	char	*home;

	ft_update_envlst("OLDPWD", ft_get_envlst_val("PWD", g_minishell), false, g_minishell);
	home = ft_get_envlst_val("HOME", g_minishell);
	if (!home)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	if (chdir(home) == ENO_SUCCESS)
		return (ft_update_envlst("PWD", home, false, g_minishell), 0);
	return (1);
}

static int	ft_cd_err_msg(char *err_msg)
{
	ft_putstr_fd("minishell: cd: `", 2);
	ft_putstr_fd(err_msg, 2);
	ft_putstr_fd("': No such file or directory\n", 2);
	return (1);
}

int	ft_cd(char *path, t_minishell *g_minishell)
{
	if (!path)
		return (ft_cd_home(g_minishell));
	if (chdir(path) != ENO_SUCCESS)
		return (ft_cd_err_msg(path));
	ft_update_envlst("OLDPWD", ft_get_envlst_val("PWD", g_minishell), false, g_minishell);
	return (ft_change_pwd(g_minishell));
}
