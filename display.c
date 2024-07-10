#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define LW_BRIDGE_BASE 0xFF200000
#define LW_BRIDGE_SPAN 0x00005000
#define HEX5_BASE 0x10
#define HEX4_BASE 0x20
#define HEX3_BASE 0x30
#define HEX2_BASE 0x40
#define HEX1_BASE 0x50
#define HEX0_BASE 0x60

int numbers(char c) {
    switch (c) {
        case '1': return 0b00000110;
        case '2': return 0b01011011;
        case '3': return 0b01001111;
        case '4': return 0b01100110;
        case '5': return 0b01101101;
        case '6': return 0b01111101;
        case '7': return 0b00000111;
        case '8': return 0b01111111;
        case '9': return 0b01101111;
    }
}
int main(void) {
    volatile int *HEX0_BASE_ptr, *HEX1_BASE_ptr, *HEX2_BASE_ptr, *HEX3_BASE_ptr, *HEX4_BASE_ptr, *HEX5_BASE_ptr;
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

    HEX0_BASE_ptr = (volatile int *)(LW_virtual + HEX0_BASE);
    HEX1_BASE_ptr = (volatile int *)(LW_virtual + HEX1_BASE);
    HEX2_BASE_ptr = (volatile int *)(LW_virtual + HEX2_BASE);
    HEX3_BASE_ptr = (volatile int *)(LW_virtual + HEX3_BASE);
    HEX4_BASE_ptr = (volatile int *)(LW_virtual + HEX4_BASE);
    HEX5_BASE_ptr = (volatile int *)(LW_virtual + HEX5_BASE);
    
    HEX0_BASE_ptr = numbers('1');
    HEX1_BASE_ptr = numbers('2');
    HEX2_BASE_ptr = numbers('3');
    HEX3_BASE_ptr = numbers('4');
    HEX4_BASE_ptr = numbers('5');
    HEX5_BASE_ptr = numbers('6');

    munmap(LW_virtual, LW_BRIDGE_SPAN);
    close(fd);

    return 0;
}
