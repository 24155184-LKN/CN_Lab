/*
3. Write a  C program to extract each byte from a given number and store them in separate variable. Print the content of each separate variable.
*/

#include <stdio.h>

int main() {
    int number = 0x12345678; // Example number
    char byte1 = (number >> 24) & 0xFF;
    char byte2 = (number >> 16) & 0xFF;
    char byte3 = (number >> 8) & 0xFF;
    char byte4 = number & 0xFF;

    printf("Byte 1: %02X\n", byte1);
    printf("Byte 2: %02X\n", byte2);
    printf("Byte 3: %02X\n", byte3);
    printf("Byte 4: %02X\n", byte4);

    return 0;
}
