/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 17:43:55 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 00:48:56 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_handle_dollar(char *str, size_t *i, t_minishell *g_minishell)
{
	size_t	start;
	char	*var;
	char	*env_val;

	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(g_minishell->exit_s));
	}
	else if (!ft_is_valid_var_char(str[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	env_val = ft_get_envlst_val(var, g_minishell);
	if (!env_val)
		return (free(var), ft_strdup(""));
	return (free(var), ft_strdup(env_val));
}

static char	*ft_cmd_pre_expander(char *str, t_minishell *g_minishell)
{
	char	*ret;
	size_t	i;

	ret = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			ret = ft_strjoin_f(ret, ft_handle_squotes(str, &i));
		else if (str[i] == '"')
			ret = ft_strjoin_f(ret, ft_handle_dquotes(str, &i, g_minishell));
		else if (str[i] == '$')
			ret = ft_strjoin_f(ret, ft_handle_dollar(str, &i, g_minishell));
		else
			ret = ft_strjoin_f(ret, ft_handle_normal_str(str, &i));
	}
	return (ret);
}

char	**ft_expand(char *str, t_minishell *g_minishell)
{
	char	**expanded;
	char	**globbed;
	size_t	i;

	str = ft_cmd_pre_expander(str, g_minishell);
	if (!str)
		return (NULL);
	str = ft_clean_empty_strs(str);
	if (!str)
		return (NULL);
	expanded = ft_expander_split(str);
	free(str);
	if (!expanded)
		return (NULL);
	globbed = ft_globber(expanded);
	if (!globbed)
		return (NULL);
	i = 0;
	while (globbed[i])
	{
		globbed[i] = ft_strip_quotes(globbed[i]);
		i++;
	}
	return (globbed);
}
