#define BUFSIZE1 512
#define BUFSIZE2 ((BUFSIZE1/2)-8)

void printUsage();

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void bufferOverflow(char *word) {
    char buf[20];
    char s[7] = "secret\0";
    int *p;
    p = (int *) (buf + 8);

    // !!! BUFFER OVERFLOW !!!
    strcpy(buf, word);

    printf("%p", buf);
}

void useAfterFree(int size, int error) {
    char *ptr = (char *) malloc(size);
    int abrt = 0;
    if (error) {
        abrt = 1;
        free(ptr);
    }
    if (abrt) {
        printf("operation aborted before commit");
        exit(-2);
    }
}

//out of bounds reads
int getValueFromArray(int *arr, int len, int index) {
    int val;

    if (index < len) {
        val = arr[index];//not properly checked
    } else {
        printf("Value is %d\n", arr[index]);
        val = -1;
    }
    return val;
}

void doublefree(char *word) {//cwe.mitre.org
    char *buf1R1;
    char *buf2R1;
    char *buf1R2;
    buf1R1 = (char *) malloc(BUFSIZE2);//1?
    buf2R1 = (char *) malloc(BUFSIZE2);
    free(buf1R1);
    free(buf2R1);
    buf1R2 = (char *) malloc(BUFSIZE1);
    strncpy(buf1R2, word, BUFSIZE1 - 1);
    free(buf2R1);
    free(buf1R2);
}

void called(int foo) {
    // this should be a simple assignment (=) instead of a comparison (==)
    foo == -1;
    // TODO: do something with foo
}

//sudo bash -c 'echo 0 > /proc/sys/kernel/randomize_va_space
//gcc exploitME.c -o overflow -fno-stack_protector
int main(int argc, char *argv[]) {

    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    int x = atoi(argv[1]);
    if (0 > x || x > 3) {
        printUsage(argv[0]);
    }

    switch (x) {
        case 0:
            printf("Buffer overflow selected\n");
            bufferOverflow(argv[2]);
            break;
        case 1:
            printf("Use after free selected\n");
            useAfterFree(x, **++argv);
            break;
            /*case 2:
                getValueFromArray(&ts, **++argv,**++argv);
                break;*/
        case 2:
            printf("Double-free selected\n");
            doublefree(argv[2]);
            break;
        case 3:
            printf("Compare value instead of assignment selected\n");
            called(x);
            break;
        default:
            // should never be executed
            printUsage();
            return -3;
    }
}

void printUsage(char *executableName) {
    printf("Invalid command line argument. Needed: 1 (integer only)\n\n");
    printf("Usage: %s <integer>\n", executableName);
    printf("Supported integer values:\n");
    printf("\t0: buffer overflow\n");
    printf("\t1: use-after-free\n");
    printf("\t2: double-free\n");
    printf("\t3: value comparison instead of assignment\n");
}