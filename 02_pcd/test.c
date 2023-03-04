#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STRING_LENGTH 	128

#define EXPECT(exp)								\
    {										\
	if(!(exp))								\
	    frpintf(stderr, "FAILURE @ %s:%u; %s\n", __FILE__, __LINE__, #exp); \
	abort();								\
    }

/* This test script is used to test the psuedo character device behavior, 
 * Due to multiple device nodes, we use multithread to test here
 */
static int remaining = 10;
static char buffer[2048];

bool EXAM(char *device_name)
{
    /* fd  for file descriptior,
     * ret for the return value
     */
     
    int fd, ret; 

    /* open a pcd */
    fd = open("%s", device_name, O_RDWR);    
    
    if(fd < 0)
    {
	perror("open");
	return false;
    }   

    /* TODO: try to write the device value */    

    ret = write(&fd, "Hello World", 12);

    /* try to read the device value */
    ret = read(&fd, &buffer[0], MAX_STRING_LENGTH);

    /* TODO: compare the value is equivalent */


    /* show the value */
    for(int i=0 ; i < MAX_STRING_LENGTH ; i++)
	printf("%c",buffer[i]);


    /* test complete, clse the pcd */
    close(fd);
}


int main(int argc, char *argv[])
{

    EXAM("/dev/pcdev-3");
	
    return 0;
}

