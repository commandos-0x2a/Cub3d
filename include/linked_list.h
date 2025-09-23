/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:50:58 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/19 15:52:55 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LIST_H
# define LIST_H

typedef struct s_list
{
	void			*ctx;
	struct s_list	*next;
}	t_list;

#endif
