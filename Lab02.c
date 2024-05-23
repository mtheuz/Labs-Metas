#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "../../LabExemples/auxFiles/address_map_arm.h"


int char_to_segment(char c) {
    switch(c) {
        case 'P': return 0b01110011;
        case 'b': return 0b01111100;
        case 'l': return 0b00111000;
        case '-': return 0b01000000;
        case 'S': return 0b01110110;
        case 'd': return 0b01111010;
        case 't': return 0b01111000;
        case 'p': return 0b01110011;
        case '0': return 0b00111111;
        case '2': return 0b11011011;
        default: return 0;
    }
}

void display_message(volatile int *seg_base, const char *message, int length) {
    int i = 0;
    for (i; i < length; ++i) {
        int segment = char_to_segment(message[i]);
        *(seg_base + i) = segment;
    }
}

int main(void) {
    volatile int *SEG1_ptr, *SEG2_ptr, *KEY_ptr;
    int fd = -1;
    void *LW_virtual;

    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERRO: não foi possível abrir \"/dev/mem\"...\n");
        return (-1);
    }
    LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (LW_virtual == MAP_FAILED) {
        printf("ERRO: mmap() falhou...\n");
        close(fd);
        return (-1);
    }

    SEG1_ptr = (volatile int *) (LW_virtual + HEX3_HEX0_BASE);
    SEG2_ptr = (volatile int *) (LW_virtual + HEX5_HEX4_BASE);
    KEY_ptr = (volatile int *) (LW_virtual + KEY_BASE);

    const char message[] = "Pbl-Sdtp02";
    int msg_len = sizeof(message) - 1;
    int idx = 0;
    int running = 1;

    while (1) {
        if ((*KEY_ptr & 0x1) == 0x1) { 
            running = !running;
            while ((*KEY_ptr & 0x1) == 0x1); 
        }

        if (running) {
            display_message(SEG1_ptr, &message[idx], 2);
            display_message(SEG2_ptr, &message[idx + 4], 4);

            idx = (idx + 1) % msg_len;
            usleep(500000); 
        }
    }

    if (munmap(LW_virtual,LW_BRIDGE_SPAN) != 0)) {
        printf("ERRO: munmap() falhou...\n");
        return (-1);
    }

    close(fd);
    return 0;
}
