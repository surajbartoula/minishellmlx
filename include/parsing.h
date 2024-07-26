#ifndef PARSING_H
#define PARSING_H

#include "minishell.h"
#include "tokenizing.h"

typedef enum e_node_type
{
    N_PIPE,
    N_AND,
    N_OR,
    N_CMD
}   t_node_type;

typedef enum e_io_type
{
    IO_IN,
    IO_OUT,
    IO_HEREDOC,
    IO_APPEND
}   t_io_type;

typedef enum e_parse_err_type
{
    E_MEM = 1,
    E_SYNTAX
}   t_parse_err_type;

typedef struct s_io_node
{
    t_io_type        type;
    char            *value;
    char            **expanded_value;
    int             here_doc;
    struct s_io_node *prev;
    struct s_io_node *next;
}   t_io_node;

typedef struct s_node
{
    t_node_type      type;
    t_io_node        *io_list;
    char            *args;
    char            **expanded_args;
    struct s_node   *left;
    struct s_node   *right;
}   t_node;

typedef struct s_parse_err
{
    t_parse_err_type type;
    char            *str;
}   t_parse_err;

// parser_clear.c
void        ft_clear_io_list(t_io_node **lst);
void        ft_clear_cmd_node(t_node *node);
void        ft_recursive_clear_ast(t_node *node);

// parser_nodes.c
t_node_type ft_get_node_type(t_token_type type);
t_io_type   ft_get_io_type(t_token_type type);
t_node      *ft_new_node(t_node_type type);
t_io_node   *ft_new_io_node(t_token_type type, char *value);
void        ft_append_io_node(t_io_node **lst, t_io_node *new);

// parser_utils.c
bool        ft_is_redir(t_token_type type);
int         ft_prec(t_token_type type);

#endif