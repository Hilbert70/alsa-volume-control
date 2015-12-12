
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readIni.h"
#include "trim.h"

/*
 * ini file
 */
iniItem items[] = {
    {"item"      , NULL},
    {"event"     , NULL},
    {"user"      , NULL},
    {"mute"      , NULL},
    {"volumeup"  , NULL},
    {"volumedown", NULL},
};
    
iniItem default_items[] = {
    {"item"      , "Master"},
    {"event"     , "/dev/input/by-id/usb-Adafruit_Trinket_HID_Combo-event-mouse"},
    {"user"      , "mpd"},
    {"mute"      , "mute"},
    {"volumeup"  , "volume up"},
    {"volumedown", "volume down"},
};
    

void readIni(char * filename,iniItem **iniItems)
{
    FILE * fh;
    char line[32768];	// tricky but I guess the lines are not longer
    char lval[32768];
    char rval[32768];
    
    char *pos;
    char * item;
    char * olditem;	/* item is not item in the config, just a place holder */
    int userlen,i;

    for (i=0;i<CFG_MAX;i++)
    {
	items[i].value = malloc(strlen(default_items[i].value)+1);
	strcpy(items[i].value , default_items[i].value );	
	fprintf(stderr,"-lval = \"%s\"\t\"%s\"\n",items[i].name, items[i].value);
    }    
 
    if (fh = fopen(filename, "r")) {
	while (fgets(line, sizeof(line), fh)) {
	    if ( (pos = strstr(line, "=")) != NULL )
	    {
		/*
		  found the =
		  lval should be the key
		  rval should be the value
		*/
		strncpy(lval, &line[0], pos-&line[0]);
		strncpy(rval, pos+1, strlen(pos));
		trim(lval);
		trim(rval);

		for (i=0;i<CFG_MAX;i++) {
		    if (strstr(lval,items[i].name)){
			// found the item, now modify
			free( items[i].value );
			items[i].value = malloc(strlen(rval)+1);
			strcpy(items[i].value ,rval);
			break; // next line is up for reading, quit scanning
		    }
		    
		}

		fprintf(stderr,"lval = \"%s\"\nrval = \"%s\"\n",lval,rval);
		
	    }
	    // else skip line
	}
	
	fclose(fh);
    }
    /* build the commands 
     * "su - <user> -c '<cmd>'"
     * add 5 + 5 + 1 + strlen(user)
     *
     * change CFG_MUTE, CFG_VOLUP, CFG_VOLDWN
     * 
     */
    userlen = strlen(items[CFG_USER].value);

    for (i=CFG_MUTE; i<CFG_MAX; i++){       
	olditem = items[i].value;
	item = malloc(strlen(olditem)+11+userlen);
	strcpy(item,"su - ");
	strcat(item,items[CFG_USER].value);
	strcat(item," -c '");
	strcat(item,olditem);
	strcat(item,"'");
	items[i].value=item;
    }
    
    *iniItems = &items[0];
}
