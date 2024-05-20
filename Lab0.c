#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../../LabExemples/auxFiles/address_map_arm.h"

int main(void)
{
    volatile int *LEDR_ptr; // virtual address pointer to red LEDs
    int fd = -1;            // used to open /dev/mem
    void *LW_virtual;       // physical addresses for light-weight bridge

    // open /dev/mem to give access to physical addresses
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return (-1);
    }

    // Get a mapping from physical addresses to virtual addresses
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return (-1);
    }

    // Set virtual address pointer to I/O port
    LEDR_ptr = (volatile int *)(LW_virtual + LEDR_BASE);
    *LEDR_ptr = *LEDR_ptr + 1; // Add 1 to the I/O register

    while (1) {
        int sentido_direita_esquerda = 1;

        if (*LEDR_ptr == *LW_virtual) {
            sentido_direita_esquerda = 1;
        }

        if (*LEDR_ptr == LW_virtual + 9*(LEDR_BASE)) {
            sentido_direita_esquerda = 0;
        }

        while (sentido_direita_esquerda) {
            *LEDR_ptr = *LEDR_ptr + LEDR_BASE;
            usleep(10000);
        }

        while (!sentido_direita_esquerda) {
            *LEDR_ptr = *LEDR_ptr - LEDR_BASE;
            usleep(10000);
        }
    }
    return 0;
}