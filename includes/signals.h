/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 09:49:51 by okoca             #+#    #+#             */
/*   Updated: 2025/02/14 02:48:45 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

void	sg_input_handler(int status);

void	sg_init_signal(void);

void	sg_heredoc_handler(int status);

void	sg_exec_handler(int status);

int		sg_readline_event(void);

#endif