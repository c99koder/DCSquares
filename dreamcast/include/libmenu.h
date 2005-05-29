/*      
   GOAT Games Menu System
        
   libmenu.h

   Copyright (C)2005 Cryptic Allusion, LLC

   License: KOS/BSD

*/

#ifndef __LIBMENU_H
#define __LIBMENU_H

#include <arch/types.h>
#include <errno.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#define PACKED __attribute__((packed))

// You'll have one of these per entry in the score table.
typedef struct score_entry {
	char	name[8]		PACKED;		// NUL-terminated!
	uint32	score		PACKED;
	uint32	difficulty	PACKED;
	uint32	level_reached	PACKED;
	uint32	time_lasted	PACKED;
} score_entry_t;

// Your game will have one score table object.
// Note that play_cnt should generally be preserved!
typedef struct score_table {
	uint32	game_id		PACKED;	// Game ID for this score table
	uint32	settings[2]	PACKED;	// Random game settings, if applicable
	uint16	play_cnt	PACKED;	// Number of times the game's been played
	uint8	bitfield	PACKED;	// Which fields are actually used
	uint8	score_cnt	PACKED;	// Number of entries
	score_entry_t	entries[0 /*score_cnt*/] PACKED;
} score_table_t;
#undef PACKED

#define SCORE_TABLE_SIZE_CNT(cnt) (sizeof(score_table_t) + sizeof(score_entry_t) * (cnt))
#define SCORE_TABLE_SIZE(t) SCORE_TABLE_SIZE_CNT(t->score_cnt)

// Bitfields for 'bitfield' in the score_table_t.
#define SCORE_NAME	(1<<0)
#define SCORE_SCORE	(1<<1)
#define SCORE_DIFF	(1<<2)
#define SCORE_LEVEL	(1<<3)
#define SCORE_TIME	(1<<4)

// If any of the following functions return an error code, they will set
// errno to something appropriate.


/* Call this before using any of the GOAT Menu functions. Returns >= 0 if
   the GOAT Menu is actually running (useful for debugging). Note that
   most of these functions will keep working regardless. */
int goat_init();

// Returns zero if the menu is not running, or non-zero if it is.
int goat_menu_present();

/* Returns the score table for the current game, or NULL if there is none
   yet. The memory is malloc'd and the client program is responsible for
   freeing it. This could return NULL on an error (can't load table, out
   of memory, etc). If you want to query the scores for some other game,
   pass a non-zero value for game_id.

   Possible error codes:
     - ENOMEM: malloc() returned NULL
     - ENOENT: there was no save file (feel free to start one)
     - ENOMSG: there was a save file, but no score table for game_id
     - EIO:    error loading from VMU (corrupted file, etc)
   
*/
score_table_t * goat_load_score_table(uint32 game_id);

/* Saves the score table for the current game. If 'table' is NULL, any
   existing score table will be erased; otherwise its contents will be
   replaced with theone in 'table'. You can change and reuse a table
   returned by goat_load_score_table by using realloc(). Returns <0 if
   there's some sort of problem. Note also that table->game_id will be
   overwritten by the game_id parameter (or the current game's game_id
   if the parameter is zero), so there's no need to fill it first.

   Possible error codes:
     - ENOMEM: malloc() returned NULL
     - ENOENT: there was no save file (feel free to start one)
     - EIO:    error loading from VMU
     - ENODEV: no memory card with enough space available
*/
int goat_save_score_table(uint32 game_id, score_table_t * table);

/* Deletes the existing GOAT Games save. You should pretty much only do
   this if the save is corrupted when you try to save over it, and the user
   wants you to do it to save new scores. */
void goat_save_erase();

/* Returns a malloc'd buffer containing a null-terminated string with the
   passcode to be entered on the GOAT Games web site, to represent this
   score. You can use this to display a code for users to enter in. This
   could return NULL on an out of memory failure.

   Possible error codes:
     - ENOMEM: malloc() returned NULL
*/
char * goat_make_code(score_entry_t * score);

/* Returns to the main menu. You must do this if you want the menu to reload
   at all. This will simply call arch_exit() if the menu isn't running. */
void goat_exit();

/* Returns 0 if you should stay in 60hz, or >0 if you should switch to 50hz.
   This will always return 0 if the menu isn't running. */
int goat_50hz();

__END_DECLS

#endif	// __LIBMENU_H
