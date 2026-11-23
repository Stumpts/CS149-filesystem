#ifndef INIT_C
#define INIT_C

#include <stdio.h>
#include <stdlib.h>
#include "FileSystem.c"

/**
 * Initializes sector s as an empty directory sector when it is allocated.
 * s - sector number to initialize
 * back - previous sector (-1 for root)
 */
static void initDir(int s, int back) {
	// set back, frwd, and free fields
	sector[s].directory.back = back;
	sector[s].directory.frwd = -1;
	sector[s].directory.free = 25;

	// mark and initialize all 25 directory entries
	for (int i = 0; i < 25; ++i) {
		sector[s].directory.dir[i].type = 'F';
		sector[s].directory.dir[i].name[0] = '\0'; // null-terminated
		sector[s].directory.dir[i].link = 0; // empty
		sector[s].directory.dir[i].size = 0; // 0 for directory entries
	}
}

#endif
