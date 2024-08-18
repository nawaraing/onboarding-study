#include <stdio.h>

int main() {
    // int arr[] = {1, 2, 3};
    // // int* ptr = arr;

    // printf("addr: %p\n", arr);
    // printf("arr0: %d\n", *arr);
    // printf("arr1: %p\n", arr + 1);
    // printf("arr1: %d\n", *(arr + 1));
    // printf("arr2: %p\n", arr + 2);
    // printf("arr2: %d\n", arr[2]);

    struct Student {
        char name[5]; // 8 byte
        int age;      // 4 byte
        char address[10];

    };

    struct Student boy;
    struct Student* pointer = &boy;
    long addr = pointer;

    // boy.name = strncpy("junkang");
    boy.age = 30;

    printf("realPtr: %p\n", &(boy.age));
    printf("agePtr : %p\n", ((int *)(addr + 8)));
    printf("age : %d\n", *((int *)(addr + 8)));

    
    // printf("name: %s\n", boy.name);
    // for (int i = 0; i < 10; i++) {
    //     printf("name[%d]: %c\n", i, boy.name[i]);
    // }
    printf("age: %d\n", boy.age);


    

    /*

    01
    -> 0100
    0123456789
    -> 0016
    0123456789abcdef
    -> 0x0010
    
    0x7ff7ba93e44c
    0x7ff7ba93e450
    0x7ff7ba93e454

    [1][2][3]
    
    */
}