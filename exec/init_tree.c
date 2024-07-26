/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 11:40:53 by abelayad          #+#    #+#             */
/*   Updated: 2023/06/08 13:04:45 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_sig_handler.cleanup_needed = true;
}

void	ft_heredoc(t_io_node *io, int p[2], t_minishell *g_minishell)
{
	char	*line;
	char	*quotes;

	signal(SIGINT, ft_heredoc_sigint_handler);
	quotes = io->value;
	while (*quotes && *quotes != '"' && *quotes != '\'')
		quotes++;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_is_delimiter(io->value, line))
			break ;
		if (!*quotes)
			ft_heredoc_expander(line, p[1], g_minishell);
		else
		{
			ft_putstr_fd(line, p[1]);
			ft_putstr_fd("\n", p[1]);
		}
		free(line);
	}
	if (g_sig_handler.cleanup_needed)
		ft_clean_ms(g_minishell);
	exit(0);
}

static bool	ft_leave_leaf(int p[2], int *pid)
{
	waitpid(*pid, pid, 0);
	signal(SIGQUIT, ft_sigquit_handler);
	g_sig_handler.signint_child = false;
	close(p[1]);
	if (WIFEXITED(*pid) && WEXITSTATUS(*pid) == SIGINT)
		return (true);
	return (false);
}

static void	ft_init_leaf(t_node *node, t_minishell *g_minishell)
{
	t_io_node	*io;
	int			p[2];
	int			pid;

	if (node->args)
		node->expanded_args = ft_expand(node->args, g_minishell);
	io = node->io_list;
	while (io)
	{
		if (io->type == IO_HEREDOC)
		{
			pipe(p);
			g_sig_handler.signint_child = true;
			pid = (signal(SIGQUIT, SIG_IGN), fork());
			if (!pid)
				ft_heredoc(io, p, g_minishell);
			if (ft_leave_leaf(p, &pid))
				return ;
			io->here_doc = p[0];
		}
		else
			io->expanded_value = ft_expand(io->value, g_minishell);
		io = io->next;
	}
}

void	ft_init_tree(t_node *node, t_minishell *g_minishell)
{
	if (!node)
		return ;
	if (node->type == N_PIPE
		|| node->type == N_AND
		|| node->type == N_OR)
	{
		ft_init_tree(node -> left, g_minishell);
		if (!g_sig_handler.heredoc_sgint)
			ft_init_tree(node -> right, g_minishell);
	}
	else
		ft_init_leaf(node, g_minishell);
}
