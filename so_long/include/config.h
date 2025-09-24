/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 21:11:43 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 18:11:14 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# define WIN_WIDTH  1280
# define WIN_HEIGHT 768

# ifndef TILEDS_PATH
#  define TILES_PATH "textures/tile.xpm"
# endif

# ifndef PLAYER_PATH
#  define PLAYER_PATH "textures/lumberjack.xpm"
# endif
# ifndef PLAYER_SPEED
#  define PLAYER_SPEED 6 
# endif

# ifndef FIRE_PATH
#  define FIRE_PATH "textures/bonfire.xpm"
# endif

# ifndef BOAT_PATH
#  define BOAT_PATH "textures/boat_64x64.xpm"
# endif

# ifndef TREE_PATH
#  define TREE_PATH "textures/trees_56x94.xpm"
# endif

# ifndef BANNER_PATH
#  define BANNER_PATH "textures/banner.xpm"
# endif

#endif
