#ifndef COMMANDS_C
#define COMMANDS_C

#include <stdio.h>
#include <string.h>
#include "fileSystem.h"



/*
 * TODO: CREATE, DELETE, WRITE, CD, EXIT, COMMAND_LOOP
 */



void createCmd(void) {
/*
 * TODO: CREATE U creates an empty regular file and adds its entry to the current directory. 
 * Initially the new file has size 0 and link 0. Its first data sector is allocated when data is written. 
 * CREATE D allocates and initializes a directory sector and adds an entry of type D to the current directory. 
 * Duplicate names in the same directory must be rejected. If the current directory has no free entries, extend it with another directory sector when space is available.
 */
}

void deleteCmd(void) {
/*
 * TODO: DELETE removes the specified regular file or empty subdirectory. 
 * For a regular file, follow the complete chain of data sectors and return every allocated sector to the free list. 
 * Mark its directory entry F and update the directory's free count. 
 * A directory may be deleted only if it contains no files or subdirectories. 
 * The root directory may not be deleted. 
 * Do not delete the currently open file.
 */
}

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
void closeCmd(void) {
	if (!openFile.isOpen) {
		printf("ERROR: No file is open.\n");
		return;
	}
	openFile.isOpen = false;
	printf("File has been closed.\n");
}



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

void writeCmd(void) {
	/*
	 * TODO: WRITE places the specified number of bytes at the current position of the open file. 
	 * In the second example, the six bytes consist of a leading space followed by WORLD. 
	 * The program must preserve spaces in the data, allocate additional sectors when necessary, connect the data-sector links, update the file position, and increase the file size when a write extends beyond the previous end of the file. 
	 * Only files opened in O or U mode may be written.
	 * Use a consistent command parser that distinguishes the byte count from the data. 
	 * For this project, WRITE input may be limited to printable characters entered on one line. Reject a command if fewer data bytes are provided than requested.
	*/
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
void dirCmd(void)
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


void cdCmd(void) {
	/*
	 * TODO: CD changes the current directory to the named subdirectory. Verify that the entry exists and has type D. Your program must allow navigation into nested subdirectories by issuing successive CD commands.
	 */
}

// ROOT changes the current directory to sector 0.
void rootCmd(void) {
	currDir = 0;
}

//EXIT terminates the program after releasing dynamically allocated management nodes.
void exitCmd(void) {
	release();
	//break command loop after
}


//for taking commands in terminal
void runCommandLoop(void) {
	while (1) {
		break;
	}
}





#endif
