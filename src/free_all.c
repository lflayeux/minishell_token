/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aherlaud <aherlaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:32:56 by aherlaud          #+#    #+#             */
/*   Updated: 2025/05/29 15:40:05 by aherlaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_lstclear_malloc(t_malloc *lst)
{
	t_malloc	*current;
	t_malloc	*tmp;

	current = lst;
	while (current)
	{
		tmp = current;
		current = current->next;
		if (tmp->tab)
			ft_free_tab((void **)tmp->tab);
		if (tmp->str)
			free(tmp->str);
		free(tmp);
	}
	lst = NULL;
}

void	ft_free_all(t_shell *shell)
{
	// if (shell->signals)
	//     free(signals);
	if (shell->tok)
		ft_lstclear_tok(shell->tok);
	if (shell->exec)
		ft_lstclear_exec(shell->exec);
	if (shell->malloc)
		ft_lstclear_malloc(shell->malloc);
	// if (shell->env)
	// 	ft_free_tab((void **)(shell->env));
	if (shell->secret)
		ft_free_tab((void **)(shell->secret));
	if (shell->child_tab)
		ft_free_tab((void **)(shell->child_tab));
}

void	add_or_free(t_shell *shell, char *str, char **tab_str, int flag)
{
	if (flag == 0)
	{
		if (!str)
		{
			ft_free_all(shell);
			exit(1);
		}
		else
			ft_lstadd_back_malloc(&(shell->malloc), ft_lstnew_malloc(NULL,
					str));
	}
	else
	{
		if (!tab_str)
		{
			ft_free_all(shell);
			exit(1);
		}
		else
			ft_lstadd_back_malloc(&(shell->malloc), ft_lstnew_malloc(tab_str,
					NULL));
	}
}
