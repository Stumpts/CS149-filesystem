#include <stdio.h>
#include <stdlib.h>

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
