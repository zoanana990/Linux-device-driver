/* include libraries */
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MACRO DEFINE */
#define MAX_STRING_LENGTH 	128
#define MAX_DEVICE_COUNT	4

#define toString(s)		#s
#define concatenate(a, b)	a##b

/* MACRO FUNCTION DEFINITION */
#define EXPECT(exp)								\
    {										\
	if(!(exp)){								\
	    printf("FAILURE @ %s:%u; %s\n", __FILE__, __LINE__, #exp);          \
	    abort();								\
	}									\
    }

/* This test script is used to test the psuedo character device behavior, 
 * Due to multiple device nodes, we use multithread to test here
 */
static int remaining = 10;
char buffer[2048];
static char *test_string[] = 
{
    "This is a test string\n",
    "The year 2022 marks seven years since the stable version of the Rust language was officially released."
    " Since its release, Rust has been popular among developers. In a Stack Overflow poll of over 73,000 developers from 180 countries,"
    " Rust was voted the most popular programming language for the seventh consecutive year, with 87% of developers expressing a desire to use it.",
};

int EXAM(char *device_name)
{
    /* fd  for file descriptior,
     * ret for the return value
     */
     
    int fd, ret, n = strlen(test_string); 

    printf("device name = %s\n", device_name);

    /* open a pcd */
    fd = open(device_name, O_RDWR);    
    
    if(fd < 0)
    {
	perror("open");
	goto error_handling;
    }   

/* -------------------------------------------------------------- MULTI THREAD TEST LOOP HERE */
    /* TODO: try to write the device value */    
    ret = write(fd, test_string, n);
    
    /* let the position be 0 */
    ret = lseek(fd, 0, SEEK_SET); 

    /* try to read the device value */
    ret = read(fd, &buffer[0], n);
    

    /* TODO: compare the value is equivalent */


    /* show the value */
    printf("Show the read value\n");
    for(int i=0 ; i < n ; i++)
	printf("%c",buffer[i]);

    printf("\n");

    /* test complete, close the pcd */

/* ---------------------------------------------------------------MULTI THREAD TEST LOOP COMPLETE */
    close(fd);

    return 0;

error_handling:
    return 1;

}


int main(int argc, char *argv[])
{
    printf("%s\n","/dev/pcdev-" toString(1));

#if 0
    pthread_t id;

    /* Use multiple threads to test the pseudo character device */
    for(int i=0; i<MAX_DEVICE_COUNT; i++){
	char temp[] = 
	pthread_create(&id, NULL, &EXAM, "/dev/pcdev-%d", i);
    }

    if(EXAM("/dev/pcdev-0"))
    {
	printf("Test Failed\n");
	abort();
    }
    printf("Test complete");

    pthread_join(id, NULL);
#endif
    return 0;
}

