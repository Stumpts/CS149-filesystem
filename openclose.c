#ifndef OPENCLOSE_C
#define OPENCLOSE_C

#include <stdio.h>
#include <string.h>
#include "FileSystem.c"
#include "helpers.c"

/**
 * The OPEN command. Opens a file.
 * mode - the mode
 * name - the name of the file
 */
void openCmd(char mode, const char *name) {
	int sec, index; // If the file is found, sec and index will be filled in with values
	if (openFile.isOpen) {
		printf("ERROR: A file is already open.\n");
		return;
	}
	if (mode != 'I' && mode != 'O' && mode != 'U') {
		printf("ERROR: Invalid mode. Must be I, O, or U\n");
		return;
	}
	if (!findEntry(currDir, name, &sec, &index)) {
		printf("ERROR: File not found.\n");
		return;
	}
	if (sector[sec].directory.dir[index].type != 'U') {
		printf("ERROR: Not a regular file.\n");
		return;
	}

	openFile.isOpen = true;
	openFile.dirSector = sec;
	openFile.entry = index;
	openFile.mode = mode;

	// set starting byte position
	if (mode == 'O') {
		// if writing, start at end of file
		openFile.pos = sector[sec].directory.dir[index].size;
	} else {
		openFile.pos = 0;
	}
	printf("File has been opened.\n");
}

/**
 * The CLOSE command. Closes the open file.
 */
void closeCmd() {
	if (!openFile.isOpen) {
		printf("ERROR: No file is open.\n");
		return;
	}
	openFile.isOpen = false;
	printf("File has been closed.\n");
}

#endif
