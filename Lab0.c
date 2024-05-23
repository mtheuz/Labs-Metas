#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>       
#include <sys/mman.h>
#include <unistd.h>         
#include "../../LabExemples/auxFiles/address_map_arm.h"

int main(void)
{
    volatile int *LEDR_ptr; 
    int fd = -1;            
    void *LW_virtual;       
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return -1;
    }

    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return -1;
    }
    
    LEDR_ptr = (volatile int *)(LW_virtual + LEDR_BASE);
    *LEDR_ptr = 0x1;
    while (1) {

        for (; *LEDR_ptr < 0x200; *LEDR_ptr <<= 1) {
            usleep(100000); 
        }
        for (; *LEDR_ptr > 0x1; *LEDR_ptr >>= 1) {
            usleep(100000); 
        }
    }

    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}