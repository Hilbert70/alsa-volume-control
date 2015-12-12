

/* dirty globals */

#define CFG_ITEM   0
#define CFG_EVENT  1
#define CFG_USER   2
#define CFG_MUTE   3
#define CFG_VOLUP  4
#define CFG_VOLDWN 5
#define CFG_MAX	   6

typedef struct iniItem {
    char * name;
    char * value;
} iniItem;
    

iniItem *initItems;

void readIni(char * ,iniItem **);
