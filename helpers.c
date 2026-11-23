#ifndef HELPERS_C
#define HELPERS_C

#include <stdio.h>
#include <string.h>
#include "FileSystem.c"

/**
 * Search a directory for an occupied entry with a given name. Returns true if found.
 * dir - first sector of the directory to search
 * name - file/dir name
 * sec - output; directory sector where entry is found
 * index - output; index of entry in that sector
 */
bool findEntry(int dir, const char *name, int *sec, int *index) {
	int s = dir;
	// for every sector in the directory
	for (int j = 0; j < 100; j++) {
		// check all 25 entries
		for (int i = 0; i < 25; i++) {
			if (sector[s].directory.dir[i].type != 'F' &&
				       	strcmp(sector[s].directory.dir[i].name, name) == 0) {
				*sec = s;
				*index = i;
				return true;
			}
		}
		// end of chain
		if (sector[s].directory.frwd <= 0) {
			return false;
		}
		s = sector[s].directory.frwd;
	}
}

/**
 * Finds a file's kth sector. Returns a sector number that may be 0 if the chain is shorter than k+1 or first is 0 (file empty).
 * first - first data sector of the file (link)
 * k - index of sector wanted (position / 504)
 */
int getSector(int first, int k) {
	int s = first;
	while (k > 0 && s > 0) {
		s = sector[s].data.frwdf; // next sector
		k--;
	}
	return s;
}

#endif
