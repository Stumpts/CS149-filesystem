#ifndef FILESYSTEM_C
#define FILESYSTEM_C

#include <stdio.h>
#include <stdlib.h>

/**
 * GUIDE: 
 * Number of sectors	100
 * Sector numbers	0–99
 * Sector size	512 bytes
 * Root directory	sector[0]
 * Directory entries per directory sector 25
 * User data per file sector	504 bytes
*/

// data blocks in system
union block {
    struct {
        int back;
        int frwd;
        int free;

        struct {
            char type;
            char name[9];
            int link;
            short size;
        } dir[25];

    } directory;

    struct {
        int backf;
        int frwdf;
        char userdata[504];
    } data;
};

//disk
union block sector[100];

//free sector (bitmap) TODO buildList(), Remove(), Insert()
struct nodeType { 
    int info; 
    struct nodeType *link; 
};

// Open-File Information
static int currDir = 0;

static struct {
	bool isOpen;
	int dirSector;
	int entry;
	char mode;
	int pos;
} openFile;

#endif
