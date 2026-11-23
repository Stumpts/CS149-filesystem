#include <stdio.h>
#include <string.h>
#include "FileSystem.c"
#include "helpers.c"
#include "init.c"
#include "openclose.c"
#include "fileRead.c"

/**
 * Add a test file with 2 sectors s1 and s2 since we don't have CREATE command yet
 */
void addFile(int i, const char *name, int len, int s1, int s2)
{
	sector[0].directory.dir[i].type = 'U';
	strcpy(sector[0].directory.dir[i].name, name);
	sector[0].directory.dir[i].link = s1;
	sector[0].directory.dir[i].size = len;
	sector[0].directory.free--;

	sector[s1].data.frwdf = s2;
	sector[s2].data.backf = s1;

	const char *pattern = "HelloWorld!ThisIsMyPattern!abcdefghijklmnopqrstuvwxyz1234567890qwertyuiopasdfghjklzxcvbnmGoodbyeWorld!";
	for (int b = 0; b < len; b++) {
		int s = (b < 504) ? s1 : s2; // which sector
		sector[s].data.userdata[b % 504] = pattern[b % strlen(pattern)];
	}
}

int main(void)
{
	initDir(0, -1);
	addFile(0, "A", 600, 5, 6); // file with two sectors
	addFile(1, "B", 30, 9, 0); // file with one sector

	char cmd[16];
	char name[16];
	char mode;
	int n;

	printf("=========== SIMPLIFIED FILE SYSTEM =============\n");
		printf("Currently supports: \n");
		printf("- OPEN [I/O/U] [filename]\n");
		printf("- CLOSE\n");
		printf("- READ [number]\n");
		printf("- SEEK [number]\n");
		printf("- DIR\n");
		printf("- EXIT\n");

	do {
		printf("Enter a command > ");
		// prevent infinite loop if scanf fails
		if (scanf("%15s", cmd) != 1) {
            strcpy(cmd, "EXIT");
        } else if (strcmp(cmd, "DIR") == 0) {
			dirCmd();
		} else if (strcmp(cmd, "CLOSE") == 0) {
			closeCmd();
		} else if (strcmp(cmd, "OPEN") == 0) {
			scanf(" %c %15s", &mode, name);
			openCmd(mode, name);
		} else if (strcmp(cmd, "READ") == 0) {
			scanf("%d", &n);
			readCmd(n);
		} else if (strcmp(cmd, "SEEK") == 0) {
			scanf("%d", &n);
			seekCmd(n);
		} else if (strcmp(cmd, "EXIT") != 0) {
			printf("ERROR: Please enter a valid command.\n");
		}
		printf("\n");
	} while (strcmp(cmd, "EXIT") != 0);
	return 0;
}
