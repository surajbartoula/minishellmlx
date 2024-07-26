/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 22:55:17 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 01:13:55 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*ft_term(t_minishell *g_minishell)
{
	t_node	*node;

	if (g_minishell->parse_err.type)
		return (NULL);
	if (ft_curr_token_is_binop(g_minishell) || g_minishell->curr_token->type == T_C_PARENT)
		return (ft_set_parse_err(E_SYNTAX, g_minishell), NULL);
	else if (g_minishell->curr_token->type == T_O_PARENT)
	{
		ft_get_next_token(g_minishell);
		node = ft_expression(0, g_minishell);
		if (!node)
			return (ft_set_parse_err(E_MEM, g_minishell), NULL);
		if (!g_minishell->curr_token
			|| g_minishell->curr_token->type != T_C_PARENT)
			return (ft_set_parse_err(E_SYNTAX, g_minishell), node);
		ft_get_next_token(g_minishell);
		return (node);
	}
	else
		return (ft_get_simple_cmd(g_minishell));
}

t_node	*ft_expression(int min_prec, t_minishell *g_minishell)
{
	t_node			*left;
	t_node			*right;
	int				n_prec;
	t_token_type	op;

	if (g_minishell->parse_err.type || !g_minishell->curr_token)
		return (NULL);
	left = ft_term(g_minishell);
	if (!left)
		return (NULL);
	while (ft_curr_token_is_binop(g_minishell) && ft_curr_token_prec(g_minishell) >= min_prec)
	{
		op = g_minishell->curr_token->type;
		ft_get_next_token(g_minishell);
		if (!g_minishell->curr_token)
			return (ft_set_parse_err(E_SYNTAX, g_minishell), left);
		n_prec = ft_prec(op) + 1;
		right = ft_expression(n_prec, g_minishell);
		if (!right)
			return (left);
		left = ft_combine(op, left, right, g_minishell);
		if (!left)
			return (ft_clear_ast(&left, g_minishell), ft_clear_ast(&right, g_minishell), NULL);
	}
	return (left);
}

t_node	*ft_combine(t_token_type op, t_node *left, t_node *right, t_minishell *g_minishell)
{
	t_node	*node;

	if (g_minishell->parse_err.type)
		return (NULL);
	node = ft_new_node(ft_get_node_type(op));
	if (!node)
		return (ft_set_parse_err(E_MEM, g_minishell), NULL);
	node -> left = left;
	node -> right = right;
	return (node);
}

t_node	*ft_parse(t_minishell *g_minishell)
{
	t_node	*ast;

	g_minishell->curr_token = g_minishell->tokens;
	ast = ft_expression(0, g_minishell);
	if (g_minishell->curr_token)
		return (ft_set_parse_err(E_SYNTAX, g_minishell), ast);
	return (ast);
}
