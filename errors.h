/*
 * =============================================================================
 *
 *       Filename:  errors.h
 *
 *    Description:  Error constants
 *
 *        Version:  1.0
 *        Created:  23/12/2012 18:50:22
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
 * playslave is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * playslave is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with playslave; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef ERRORS_H
#define ERRORS_H

/**  DATA TYPES  **************************************************************/

/* Categories of error.
 *
 * NOTE: If you're adding new errors here, PLEASE update the arrays in errors.c
 * to add a name and blame factor to each new error.
 */
enum error {
	E_OK = 0,		/* No error */
	/* User errors */
	E_NO_FILE,		/* Tried to read nonexistent file */
	E_BAD_STATE,		/* State transition not allowed */
	E_BAD_COMMAND,		/* Command was malformed */
	/* Environment errors */
	E_BAD_FILE,		/* Tried to read corrupt file */
	E_BAD_CONFIG,		/* Program improperly configured */
	/* System errors */
	E_AUDIO_INIT_FAIL,	/* Couldn't open audio backend */
	E_INTERNAL_ERROR,	/* General system error, usually fatal */
	E_NO_MEM,		/* Allocation of memory failed */
	/* Misc */
	E_EOF,			/* Reached end of file while reading */
	E_INCOMPLETE,		/* Incomplete computation, try again */
	E_UNKNOWN,		/* Unknown error */
	/*--------------------------------------------------------------------*/
	NUM_ERRORS		/* Number of items in enum */
};

/* Categories of blame for errors. */
enum error_blame {
	EB_USER,		/* End-user is at fault */
	EB_ENVIRONMENT,		/* Environment is at fault */
	EB_PROGRAMMER,		/* Programmer is at fault */
	/*--------------------------------------------------------------------*/
	NUM_ERROR_BLAMES	/* Number of items in enum */
};

/**  FUNCTIONS  ***************************************************************/

void		dbug      (const char *format,...);
enum error	error(enum error code, const char *format,...);

#endif				/* not ERRORS_H */
