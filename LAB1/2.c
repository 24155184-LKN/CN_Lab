/*
2. Write a C program to assign values to each member of the following structure. Pass the populated structure to a function using call by value,
and another function using call by address, and print the value of each member of the structure.
*/

#include <stdio.h>

struct dob {
    int day;
    int month;
    int year;
};

typedef struct Student {
    int roll;
    char name[20];
    int cgpa;
    struct dob age;
} stu;

void printByValue(stu s) {
    printf("Roll: %d\n", s.roll);
    printf("Name: %s\n", s.name);
    printf("CGPA: %d\n", s.cgpa);
    printf("DOB: %d/%d/%d\n", s.age.day, s.age.month, s.age.year);
}

void printByAddress(stu *s) {
    printf("Roll: %d\n", s->roll);
    printf("Name: %s\n", s->name);
    printf("CGPA: %d\n", s->cgpa);
    printf("DOB: %d/%d/%d\n", s->age.day, s->age.month, s->age.year);
}

int main(){
    stu s1;

    printf("Enter roll number: ");
    scanf("%d", &s1.roll);

    printf("Enter name: ");
    scanf("%s", s1.name);

    printf("Enter CGPA: ");
    scanf("%d", &s1.cgpa);

    printf("Enter date of birth (dd/mm/yyyy): ");
    scanf("%d/%d/%d",&s1.age.day, &s1.age.month, &s1.age.year);

    printf("\nPrinting by value:\n");
    printByValue(s1);

    printf("\nPrinting by address:\n");
    printByAddress(&s1);

    return 0;
}

