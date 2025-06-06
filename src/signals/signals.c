/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandemonium <pandemonium@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:23:58 by lflayeux          #+#    #+#             */
/*   Updated: 2025/06/02 10:55:10 by pandemonium      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_signal(t_signal *signals)
{
	sigemptyset(&signals->ctrl_c.sa_mask);
	signals->ctrl_c.sa_flags = 0;
	signals->ctrl_c.sa_handler = handle_ctrl_c_interactive;
	sigaction(SIGINT, &signals->ctrl_c, NULL);

	sigemptyset(&signals->ctrl_dump.sa_mask);
	signals->ctrl_dump.sa_flags = 0;
	signals->ctrl_dump.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &signals->ctrl_dump, NULL);
}


void	parent_signals(t_signal *signals)
{
	signals->ctrl_c.sa_handler = handle_ctrl_c_action;
	sigaction(SIGINT, &signals->ctrl_c, NULL);
}

void	child_signals(t_signal *signals)
{
	signals->ctrl_dump.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &signals->ctrl_dump, NULL);
}


void	reset_signals(t_signal *signals)
{
	signals->ctrl_c.sa_handler = handle_ctrl_c_interactive;
	sigaction(SIGINT, &signals->ctrl_c, NULL);
	signals->ctrl_dump.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &signals->ctrl_dump, NULL);
}
