/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:07:53 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/25 23:17:55 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_get_next_token(t_minishell *g_minishell)
{
	g_minishell->curr_token = g_minishell->curr_token -> next;
}

bool	ft_curr_token_is_binop(t_minishell *g_minishell)
{
	t_token_type	type;

	if (!g_minishell->curr_token)
		return (false);
	type = g_minishell->curr_token->type;
	if (type == T_PIPE || type == T_AND || type == T_OR)
		return (true);
	return (false);
}

bool	ft_is_redir(t_token_type type)
{
	if (type == T_LESS || type == T_GREAT
		|| type == T_DLESS || type == T_DGREAT)
		return (true);
	return (false);
}

int	ft_prec(t_token_type type)
{
	if (type == T_OR || type == T_AND)
		return (0);
	return (1);
}

int	ft_curr_token_prec(t_minishell *g_minishell)
{
	return (ft_prec(g_minishell->curr_token->type));
}
