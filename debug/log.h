/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:28:11 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/05 11:22:26 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
#define LOG_H

#include <ft_printf.h>

#ifndef DEBUG_FD
#define DEBUG_FD 2
#endif

# ifndef LOG_DEBUG
#  define LOG_DEBUG	log_msg
# endif
# ifndef LOG_INFO
#  define LOG_INFO	log_msg
# endif
# ifndef LOG_WARN
#  define LOG_WARN	log_msg
# endif
# ifndef LOG_ERROR
#  define LOG_ERROR	log_msg
# endif
# ifndef LOG_FATAL
#  define LOG_FATAL	log_msg
# endif

void	log_msg(const char *format, ...);

#endif // LOG_H
