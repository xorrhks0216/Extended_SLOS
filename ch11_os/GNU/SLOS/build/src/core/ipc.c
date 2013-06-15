#include "../headers/api_types.h"
#include "mutex.h"
#include "ipc.h"

void initFIFO()
{
	int i,j;

	aWAIT;
	for(i = 0; i < MAX; i++)
	{
		for(j = 0; j < MAXQ; j++)
		{
			fifo_str[i].value[j] = USELESS;
		}
		fifo_str[i].length = -1;
		fifo_str[i].isUse = NOUSE;
		fifo_str[i].key = -1;
	}
	aSIGNAL;
}

UID createFIFO(int key)
{
	int i;

	aWAIT;
	//duplication key value detection
	for(i = 0; i < MAX; i++)
	{
		if(fifo_str[i].key == key)
		{
			aSIGNAL;
			return -1;
		}
	}

	for(i = 0; i < MAX; i++)
	{
		if(fifo_str[i].isUse == NOUSE)
		{
			fifo_str[i].isUse = USE;
			fifo_str[i].key = key;
			aSIGNAL;
			return i;
		}
	}
	aSIGNAL;
	return -1;
}

int readFIFO(int key)
{
	int val, i;
	UID id;

	aWAIT;
	//FIFO structure is not opened.
	if((id=getFIFO(key)) == -1)
	{
		return -1;
	}
	//id'th FIFO structure is open, but no data.
	if(fifo_str[id].length == -1)
	{
		aSIGNAL;
	   
		return -1;
	}
	
	val = fifo_str[id].value[0];
	for(i = 0; i < MAXQ; i++)
		fifo_str[id].value[i] = fifo_str[id].value[i+1];
	fifo_str[id].value[MAXQ-1] = USELESS;
	fifo_str[id].length--;

	aSIGNAL;
	return val;
}

int writeFIFO(int key, int value)
{
	UID id;
	int i;

	aWAIT;
	id = key;
	id = getFIFO(id);

	//FIFO structure is not opened.
	if(id == -1)
	{
		return -1;
	}
	//FIFO queue is full.
	if(fifo_str[id].length == MAXQ-1)
	{
		while(fifo_str[id].length != MAXQ-1);
		aSIGNAL;
		return -1;
	}
	i = ++fifo_str[id].length;
	fifo_str[id].value[i] = value;
	aSIGNAL;
	return 0;
}

int closeFIFO(int key)
{
	int i;
	UID id;

	aWAIT;
	//FIFO structure is not opened.
	if((id=getFIFO(key)) == -1)
		return -1;

	for(i = 0; i < MAXQ; i++)
	{
		fifo_str[id].value[i] = -1;
	}
	fifo_str[id].length = -1;
	fifo_str[id].isUse = NOUSE;
	aSIGNAL;
	return 0;
}

UID getFIFO(int key)
{
	int i;
	for(i = 0; i < MAX; i++)
	{
		if(fifo_str[i].key == key)
		{
			return i;
		}
	}
	return i;
}

int getLength(int key)
{
	UID id;

	id = getFIFO(key);

	return fifo_str[id].length;
}
