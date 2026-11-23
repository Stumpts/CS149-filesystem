#ifndef FILEREAD_C
#define FILEREAD_C

#include <stdio.h>
#include <string.h>
#include "FileSystem.c"
#include "helpers.c"

/**
 * The READ command. Reads the currently open file.
 * n - number of bytes to read
 */
void readCmd(int n) {
	if (!openFile.isOpen) {
		printf("ERROR: No file is open.\n");
		return;
	}
	// only I or U mode allows reading
	if ((openFile.mode != 'I') && (openFile.mode != 'U')) {
		printf("ERROR: Opened file is not open for reading.\n");
		return;
	}
	// get openFile info
	int size = sector[openFile.dirSector].directory.dir[openFile.entry].size;
	int s = sector[openFile.dirSector].directory.dir[openFile.entry].link;
	// set n to only read as much as size - position
	if (n > size - openFile.pos) {
		n = size - openFile.pos;
	}

	// get the current sector
	s = getSector(s, openFile.pos / 504);
	// print n characters from the file starting from the openFile's position
	for (int i = 0; i < n; i++) {
		int p = openFile.pos + i;
		// if the position is past 504 for that sector, move to the next sector
		if (p > 0 && p % 504 == 0)
		{
			s = sector[s].data.frwdf;
		}
		printf("%c", sector[s].data.userdata[p % 504]);
	}
	printf("\n%d bytes read.\n", n);
	openFile.pos += n;
}

/**
 * SEEK command. Changes the current byte position of the open file beginning from 0.
 */
void seekCmd(int p)
{
	if (!openFile.isOpen) { 
		printf("ERROR: No file is open.");
		return;
	}
	if ((p < 0) || (p > sector[openFile.dirSector].directory.dir[openFile.entry].size)) {
		printf("ERROR: Position is out of range.");
		return;
	}
	openFile.pos = p;
}

/**
 * DIR command. Displays names and types of all occupied entries in the current directory.
 */
void dirCmd()
{
	int shown = 0; // count how many entries get printed
	int s = currDir; // sector being listed (the current directory)
	printf("%-5s  %-4s  %-8s  %-5s  %-6s\n", "Entry", "Type", "Name", "Link", "Size");
	int entryNum = 0;
	do {
		for (int i = 0; i < 25; i++) {
			char type = sector[s].directory.dir[i].type;
			const char *name = sector[s].directory.dir[i].name;
			int link = sector[s].directory.dir[i].link;
			int size = sector[s].directory.dir[i].size;

			if (type == 'F') {
				continue;
			}

			printf("%-5d  %-4c  %-8s  %-5d  %-6d\n", entryNum, type, name, link, size);
			entryNum++;
			shown++;
		}
		s = sector[s].directory.frwd; // move to next sector
	} while (s > 0);
	if (shown == 0) {
		printf("This directory is empty.");
	}
}

#endif
