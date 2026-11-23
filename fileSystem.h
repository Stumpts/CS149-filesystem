#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//disk params
#define NUM_SECTORS 100
#define DIRECTORY_ENTRIES 25
#define DATA_SIZE 504

//disk block
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
        } dir[DIRECTORY_ENTRIES];

    } directory;

    struct {
        int backf;
        int frwdf;
        char userdata[DATA_SIZE];

    } data;
};

//free sector list
struct nodeType {
    int info;
    struct nodeType *link;
};

// open file information
typedef struct {
    int isOpen;
    int dirSector;
    int entry;
    char mode;
    int pos;
} OpenFile;

//globals
extern union block sector[NUM_SECTORS];
extern struct nodeType *head;
extern int currDir;
extern OpenFile openFile;

//init stuff
void initSystem(void);
void initDir(int s, int back);

//helpers
bool findEntry(int dir, const char *name, int *sec, int *index);
int getSector(int first, int k);
void release(void);



//free sector funcs
void buildList(void);
void insert(int sectorNum);
int removeSector(void);

//commands
void createCmd(void);
void deleteCmd(void);
void openCmd(char mode, const char *name);
void closeCmd(void);
void readCmd(int n);
void writeCmd(void);
void seekCmd(int p);
void dirCmd(void);
void cdCmd(void);
void rootCmd(void);
void exitCmd(void);
void runCommandLoop(void);

#endif