
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readIni.h"
#include "trim.h"

/*
 * ini file
 */
char default_item[] = "xMaster";
char default_event[] = "x/dev/input/by-id/usb-Adafruit_Trinket_HID_Combo-event-mouse";
char default_user[]  = "xmpd";

void readIni(char * filename)
{
    FILE * fh;
    char line[1024];	// tricky but I guess the lines are not longer
    char lval[1024];
    char rval[1024];
    
    char *pos;
    
    /* defaults */
    item = malloc(strlen(default_item)+1);
    strcpy(item,default_item);
    
    event = malloc(strlen(default_event)+1);
    strcpy(event, default_event);
    
    user = malloc(strlen(default_user)+1);
    strcpy(user, default_user);
   
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
		if (strstr(lval,"item"))
		{
		    item = malloc(strlen(rval)+1);		   
		    strcpy(item,rval);
		}
		if (strstr(lval,"event"))
		{
		    event = malloc(strlen(rval)+1);
		    strcpy(event,rval);
		}
		if (strstr(lval,"user"))
		{
		    user = malloc(strlen(rval)+1);
		    strcpy(user,rval);
		}
		
		fprintf(stderr,"lval = \"%s\"\nrval = \"%s\"\n",lval,rval);
		
	    }
	    // else skip line
	}
	
	fclose(fh);
    }
}
