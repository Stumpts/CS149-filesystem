#ifndef FILESYSTEM_C
#define FILESYSTEM_C

#include <stdio.h>
#include <stdlib.h>

#include "fileSystem.h"

/**
 * GUIDE: 
 * Number of sectors	100
 * Sector numbers	0–99
 * Sector size	512 bytes
 * Root directory	sector[0]
 * Directory entries per directory sector 25
 * User data per file sector	504 bytes
*/


// globals
union block sector[NUM_SECTORS];

struct nodeType *head = NULL;

int currDir = 0;

OpenFile openFile = {
    .isOpen = 0,
    .dirSector = -1,
    .entry = -1,
    .mode = '\0',
    .pos = 0
};



//init free sector bitmap
void buildList(void) {            
    head = NULL;

    for (int i = 99; i >= 1; i--) {
        struct nodeType *node = malloc(sizeof(struct nodeType));
        node ->info = i;
        node ->link = head;

        head = node;
    }
}

//find free sector and return
int removeSector(void) {
    if (head == NULL) {
        return -1;  
    }

    struct nodeType *temp = head;
    int sectorNum = temp->info;
    head = head->link;

    free(temp);

    return sectorNum;
}

//insert free sector back in
void insert(int sectorNum) {
    if (sectorNum <= 0 || sectorNum >= 100) {
        return;
    }

    struct nodeType *node = malloc(sizeof(struct nodeType));

    node->info = sectorNum;
    node->link = head;

    head = node;
}



//big init


/**
 * Initializes sector s as an empty directory sector when it is allocated.
 * s - sector number to initialize
 * back - previous sector (-1 for root)
 */

 void initDir(int s, int back) {
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


void initSystem(void) {
    initDir(0, -1);
    buildList();

    currDir = 0;

    openFile.isOpen = 0;
    openFile.dirSector = -1;
    openFile.entry = -1;
    openFile.mode = '\0';
    openFile.pos = 0;
}

#endif
