/*
 * read volume keys and translate them to alsa commands (from the shell)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* strerror() */
#include <errno.h>      /* errno */

#include <fcntl.h>      /* open() */
#include <unistd.h>     /* close() */
#include <sys/ioctl.h>  /* ioctl() */
#include <linux/input.h>    /* EVIOCGVERSION ++ */

#include <unistd.h>
#include <sys/types.h>

#include "name_to_uid.h"
#include "trim.h"

#include "readIni.h"

#define EV_BUF_SIZE 16
#define NOT_FOUND -1






int main(int argc, char *argv[])
{
    int fd, sz;
    unsigned i;

    /* A few examples of information to gather */
    unsigned version;
    unsigned short id[4];                   /* or use struct input_id */
    char name[256] = "N/A";

    uid_t olduid;
    uid_t newuid;
    
    
    struct input_event ev[EV_BUF_SIZE]; /* Read up to N events ata time */

    
    if (argc < 2) {
        fprintf(stderr,
		"Usage: %s [configfile]\n",
		argv[0]
		);
        return EINVAL;
    }
    
    readIni(argv[1]);
    
    
    fprintf(stderr, "Settings: \n\titem: %s\n\tevent: %s\n\tuser: %s\n",
	    item,event,user);
    
    olduid = geteuid();
    newuid = name_to_uid(user);
    
    fprintf(stderr, "old userid %d\ntouid %d\n",olduid,newuid);
    
    
    if ((fd = open(event, O_RDONLY)) < 0) {
        fprintf(stderr,
            "ERR %d:\n"
            "Unable to open `%s'\n"
            "%s\n",
            errno, argv[1], strerror(errno)
        );
    }
    /* Error check here as well. */
    ioctl(fd, EVIOCGVERSION, &version);
    ioctl(fd, EVIOCGID, id); 
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);

    fprintf(stderr,
        "Name      : %s\n"
        "Version   : %d.%d.%d\n"
        "ID        : Bus=%04x Vendor=%04x Product=%04x Version=%04x\n"
        "----------\n"
        ,
        name,

        version >> 16,
        (version >> 8) & 0xff,
        version & 0xff,

        id[ID_BUS],
        id[ID_VENDOR],
        id[ID_PRODUCT],
        id[ID_VERSION]
    );

    /* Loop. Read event file and parse result. */
    for (;;) {
        sz = read(fd, ev, sizeof(struct input_event) * EV_BUF_SIZE);

        if (sz < (int) sizeof(struct input_event)) {
            fprintf(stderr,
                "ERR %d:\n"
                "Reading of `%s' failed\n"
                "%s\n",
                errno, argv[1], strerror(errno)
            );
            goto fine;
        }

        /* Implement code to translate type, code and value */
        for (i = 0; i < sz / sizeof(struct input_event); ++i) {
/*
            fprintf(stderr,
                "%ld.%06ld: "
                "type=%02x "
                "code=%02x "
                "value=%02x\n",
                ev[i].time.tv_sec,
                ev[i].time.tv_usec,
                ev[i].type,
                ev[i].code,
                ev[i].value
            );
*/
	    if (ev[i].type == EV_KEY &&
		ev[i].value ==1 )
	    {
		fprintf(stderr,"keyvalue = %d\n",ev[i].code);
		seteuid(newuid);
		/* do stuff */
		seteuid(olduid);
		
	    }
	    
        }
    }

fine:
    close(fd);

    return errno;
}
