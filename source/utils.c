#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <switch.h>
#include "utils.h"

char* keyboard(char* message, size_t size){
	SwkbdConfig	skp; 
	Result keyrc = swkbdCreate(&skp, 0);
	char* out = NULL;
	out = (char *)calloc(sizeof(char), size + 1);

	if (R_SUCCEEDED(keyrc) && out != NULL){
		swkbdConfigMakePresetDefault(&skp);
		swkbdConfigSetGuideText(&skp, message);
		keyrc = swkbdShow(&skp, out, size);
		swkbdClose(&skp);	
	}

	else {
	free(out);
	out = NULL;
	}

	return (out);
}

void userAppInit(void){
	void *addr = NULL;
	if (svcSetHeapSize(&addr, 0x4000000) == (Result)-1) fatalSimple(0);
}

void printarraynew(char *array[], int on[], int arraylength, int highlight, int offset, int starty){
	int max = arraylength - offset;

	if (max > MAX_LINES)
		max = MAX_LINES;

	printf("\x1b[%d;1H", starty);

	for (int i2 = 0; i2 < MAX_LINES * 2; i2++)
		printf("                                        ");

	printf("\x1b[%d;1H", starty);
	for (int i = 0; i < max; i++){
		switch(on[i + offset]){
			case 0:
				printf(RED);
				break;
			case 1:
				printf(GREEN);
				break;
			case 2:
				printf(CYAN);
				break;
			case 3:
				printf(MAGENTA);
				break;
		}
			
		if (i == highlight - 1)
			printf(INV_WHITE "%s", array[i + offset]);
		else
			printf("%s", array[i + offset]);

		printf("\n" RESET);
	}

}