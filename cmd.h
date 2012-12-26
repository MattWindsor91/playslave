/*
 * =============================================================================
 *
 *       Filename:  cmd.h
 *
 *    Description:  Interface to command parser
 *
 *        Version:  1.0
 *        Created:  26/12/2012 04:18:45
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Matt Windsor (CaptainHayashi), matt.windsor@ury.org.uk
 *        Company:  University Radio York Computing Team
 *
 * =============================================================================
 */
/*-
 * Copyright (C) 2012  University Radio York Computing Team
 *
 * This file is a part of playslave.
 *
 * playslave is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * playslave is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * playslave; if not, write to the Free Software Foundation, Inc., 51 Franklin
 * Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef CMD_H
#define CMD_H

/**  INCLUDES  ****************************************************************/

#include "errors.h"		/* enum error */
#include "player.h"		/* struct player */

/**  FUNCTIONS  ***************************************************************/

enum error	check_commands(struct player *pl);

#endif				/* not CMD_H */
