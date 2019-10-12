#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <switch.h>
#include "utils.h"

int  notes_active[200];
char *notes[200];
int amount = 0;
bool nonotes = false, keyboardinit = false;

void additem(const char *item, int spot){
    size_t size = strlen(item) + 1;
    notes[spot] = (char*) malloc (size);
    strcpy(notes[spot], item);
}

void loadini(){
    char c;
    int i = 0, tempnumb = 0;
    char temp[80] = "";
    
    FILE *ini = fopen("notes.ini", "r");

    while ((c = fgetc(ini)) && !feof(ini)){
        if (amount >= 200)
            break;

        if (c == '\n'){
            if (i == 0 && amount == 0){
                additem("No notes detected", 0);
                notes_active[0] = 9;
                nonotes = true;
                amount = 1;
                break;
            }

            temp[i - 1] = '\0';
            additem(temp, amount);
            i = 0, amount++;
            continue;
        }

        if (i == 0){
            tempnumb = (int)(c);
            notes_active[amount] = tempnumb - 48;
            fgetc(ini);
        }
        else {
            temp[i - 1] = c; 
        }

        i++;
    }

    fclose(ini);
}

void writeini(){
     FILE *ini = fopen("notes.ini", "w+");
     for (int i = 0; i < amount; i++)
        fprintf(ini, "%i %s\n", notes_active[i], notes[i]);
     fclose(ini);
}

void delnote(int position){
    int i;

    if (amount == 1){
        nonotes = true;
        additem("No notes detected", 0);
        notes_active[0] = 9;
        return;
    }

    for (i = (position - 1); i < amount - 1; i++){
        additem(notes[i + 1], i);
        notes_active[i] = notes_active[i + 1];
    }
    
    free(notes[i]);
    amount--;   
}

void addnote(){
    if (amount >= 200)
    return;

    char* message = NULL;
    message = (char*) malloc (79);

    if (!keyboardinit){
        keyboardinit = true;
        userAppInit();
    }

    message = keyboard("Put in a note. Max 79 characters", 79);

    if (strcmp(message, "")){
        if (nonotes){
            additem(message, 0);
            notes_active[0] = 0;
            nonotes = false;
        }
        else {
            additem(message, amount);
            notes_active[amount] = 0;
            amount++;
        }
    }
    free(message);
}

void notes_main(){
    int highlight = 1, offset = 0;
    bool update = true;

    printf(INV_WHITE BLACK "\x1b[1;1HNotes                                                                           " RESET GREEN "\x1b[42;1H(A) Cycle color of note\n" RED "(X) Delete note\n" BLUE "(Y) Add note\n" YELLOW "(+) Exit and save" RESET);

    while(1){
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

        if (!nonotes){
            if (kDown & KEY_LSTICK_DOWN || kDown & KEY_DDOWN) 
                highlight++, update = true;

            if (kDown & KEY_LSTICK_UP || kDown & KEY_DUP) 
                highlight--, update = true;

            if (kHeld & KEY_RSTICK_DOWN)
                highlight++, update = true;

            if (kHeld & KEY_RSTICK_UP)
                highlight--, update = true;

            if (highlight + offset > amount) 
                highlight = amount - offset, update = false;

            else if (highlight > MAX_LINES) 
                highlight = MAX_LINES, offset++;

            if (highlight < 1 && offset > 0) 
                highlight = 1, offset--;

            else if (highlight < 1 && offset <= 0) 
                highlight = 1, update = false;

            if (kDown & KEY_A){
                if (notes_active[highlight + offset - 1] >= 3)
                    notes_active[highlight + offset - 1] = 0;
                else
                    notes_active[highlight + offset - 1]++;
                update = true;
            }

            if (kDown & KEY_X){
                delnote(highlight + offset);
                if (highlight > amount)
                    highlight = amount;
                else if (highlight + offset > amount)
                    offset--;
                update = true;
            }

            if (update) {
                printarraynew(notes, notes_active, amount, highlight, offset, 3);
                printf(INV_WHITE BLACK "\x1b[1;60H%d / 200 notes" RESET, amount);
                update = false;
            }
        }
        else
            printf(RED "\x1b[3;1HNo notes detected! Press (Y) to add a note" RESET);

        if (kDown & KEY_Y){
            addnote();
            update = true;
        }

        if (kDown & KEY_PLUS)
            break;
        
        consoleUpdate(NULL);
    }
}

int main(int argc, char* argv[])
{
    consoleInit(NULL);

    if (access("notes.ini", F_OK) == -1) {
        additem("No notes detected", 0);
        notes_active[0] = 9;
        nonotes = true;
        amount = 1;
        writeini();
        amount = 0;
    }

    loadini();

    if (!strcmp(notes[0], "No notes detected"))
        nonotes = true;

    notes_main();

    writeini();

    for (int i = 0; i < amount; i++){
        free(notes[i]);
    }

    consoleExit(NULL);
    return 0;    
}