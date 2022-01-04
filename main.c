#define BUFSIZE1 512
#define BUFSIZE2 ((BUFSIZE1/2)-8)

void printUsage(char *executableName);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

FILE *fp;
char *file_contents;
struct stat filestatus;
int file_size;

void bufferOverflow(char *word, int randomNumber) {
    char buf[randomNumber];
    char s[7] = "secret";
    int *p;
    p = (int *) (buf + 8);

    // !!! BUFFER OVERFLOW !!!
    strcpy(buf, word);

    printf("%p", buf);
}

void useAfterFree(int size, int error) {
    char *ptr = (char *) malloc(size);
    int abort = 0;
    if (error) {
        abort = 1;
        free(ptr);
    }
    if (abort) {
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

void doubleFree(char *word) {//cwe.mitre.org
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

    int inputNumber;
    if (argc == 2) {

        char *filename = argv[1];
        fp = fopen(filename, "rt");
        if (fp == NULL) {
            fprintf(stderr, "Unable to open %s\n", filename);
            fclose(fp);
            free(file_contents);
            return 1;
        }

        if (stat(filename, &filestatus) != 0) {
            fprintf(stderr, "File %s not found\n", filename);
            return 1;
        }

        file_size = filestatus.st_size;
        file_contents = (char *) malloc(filestatus.st_size);

        if (fread(file_contents, file_size, 1, fp) != 1) {
            fprintf(stderr, "Unable t read content of %s\n", filename);
            fclose(fp);
            free(file_contents);
            return 1;
        }
        fclose(fp);

        inputNumber = atoi(file_contents);
        printf("Input number: %d\n", inputNumber);
    } else {
        printf("Enter an integer: ");
        scanf("%d", &inputNumber);
    }


    if (inputNumber < 0) {
        printUsage(argv[0]);
        exit(0);
    }

    int five;

    fflush(stdout);

    if (inputNumber % 10 == 1) {
        if (inputNumber % 100 / 10 == 2) {
            if (inputNumber % 1000 / 100 == 2) {
                // DIVIDE BY ZERO ERROR
                int divideByZeroError = inputNumber / 0;
                return divideByZeroError;
            }
        }
    } else {
        printf("not XXX221; changed from exit(0)");
    }

    if (inputNumber % 1000 == 666) {
        useAfterFree(inputNumber, inputNumber);
    }

    if (inputNumber >> 3 == 3){
        doubleFree("Hello world");
    }


    int random = (rand() % 1000) + 1;

    char *test = (char*) malloc(inputNumber*sizeof(char));
    for (int i = 0; i < inputNumber; i++) {
        test[i] = 'A';
    }
    bufferOverflow(test, random);
    free(test);


}

void printUsage(char *executableName) {
    printf("Invalid command line argument. Needed: 1 (integer only)\n\n");
    printf("Usage: %s <integer>\n", executableName);
    printf("Supported integer values:\n");
    printf("\t0: division-by-zero\n");
    printf("\t1: use-after-free\n");
    printf("\t2: double-free\n");
    printf("\t3: value comparison instead of assignment\n");
    printf("\t4: buffer overflow\n");
}
