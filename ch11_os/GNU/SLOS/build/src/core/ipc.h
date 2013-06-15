#include "../headers/api_types.h"
#include "mutex.h"
#define MAX 10
#define MAXQ 20
#define USE 1
#define NOUSE 0
#define USELESS -1

typedef struct fifo{
	int value[MAXQ];
	int length;
	int isUse;
	int key;
} _fifo_str;
static _fifo_str fifo_str[MAX];

void initFIFO();
UID createFIFO(int key);
int readFIFO(int key);
int writeFIFO(int key, int value);
int closeFIFO(int key);
UID getFIFO(int key);
int getLength(int key);
