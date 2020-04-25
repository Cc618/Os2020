#include "int/interrupts.h"
#include "drivers/console.h"
#include "drivers/keyboard.h"
#include "drivers/fat32.h"
#include "fs/fs.h"
#include "apps/shell.h"
#include "apps/app.h"
#include "syscalls/syscalls.h"

#include "_libc.h"

// TODO :
#include <stdint.h>
#include <string.h>
#include <stdio.h>


// TODO : rm
void printEntries(FSEntry **entries)
{
    for (size_t i = 0; entries[i]; ++i)
        printf("%s ", entries[i]->name);
    puts("");
}


// Inits all modules
static void initKernel()
{
    initInterrupts();

    // File system init
    fatInit();
    fsInit();
    
    filesInit();

    // Inputs init
    keyboardInit();

    __libc_init();
}



// TODO : rm
#include "io/pipe.h"
#include "io/file.h"
#include "k/syscalls.h"
#include "k/vector.h"
#include "k/queue.h"


static void p(void *item)
{
    printf("- %d\n", *(int*)item);
}


int child(int argc, char **argv)
{
    puts("child");

    terminate();

    return 43;
}

int myApp(int argc, char **argv)
{
    printf("myApp: %d args, 1st = %s\n", argc, argv[0]);

    sys_enter(child, 0, NULL);

    // sys_terminate();

    puts("Exit with code 42");

    return 42;
}


// After init, the user can access the kernel
static void userAct()
{
    // --- DS --- //
    // Vector //
    // Vector *v = Vector_new();

    // int *_1 = malloc(4),
    //     *_2 = malloc(4),
    //     *_3 = malloc(4);

    // *_1 = 1;
    // *_2 = 2;
    // *_3 = 3;

    // Vector_add(v, _1);
    // Vector_add(v, _2);
    // Vector_add(v, _3);
    // p(Vector_pop(v));
    // puts("---");

    // Vector_iter(v, p);

    // Vector_del(v);


    // Queue //
    // Queue *v = Queue_new(512);

    // int *_1 = malloc(4),
    //     *_2 = malloc(4),
    //     *_3 = malloc(4);

    // *_1 = 1;
    // *_2 = 2;
    // *_3 = 3;

    // Queue_add(v, _1);
    // Queue_add(v, _2);
    // Queue_add(v, _3);
    // p(Queue_pop(v));
    // puts("---");

    // Queue_iter(v, p);

    // Queue_del(v);


    // Pipe //
    // File *p = Pipe_new(42);

    // u8 buf[] = { 1, 2, 3, 4, 5 };

    // printf("Written %d bytes\n", Pipe_write(p, buf, 5));
    // size_t n;
    // printf("Read %d bytes\n", n = Pipe_read(p, buf, 10));

    // for (size_t i = 0; i < n; i++)
    // {
    //     printf("%d ", buf[i]);
    // }

    // Pipe_del(p);
















    // --- INT --- //
    // printf("%d\n", sysc1(1));
    // printf("%d\n", sysc2(2, 3));
    // printf("%d\n", sysc3(4, 5, 6));

    // // eax = syscall id / return
    // // ebx = 1st arg
    // // ecx = 2nd arg
    // // edx = 3rd arg
    // // edi = 4th arg
    // int eax,
    //     ebx,
    //     ecx,
    //     edx,
    //     edi;

    // // Retrieve registers
    // __asm__ volatile("movl %%eax, %0" : "=g" (eax));
    // __asm__ volatile("movl %%ebx, %0" : "=g" (ebx));
    // __asm__ volatile("movl %%ecx, %0" : "=g" (ecx));
    // __asm__ volatile("movl %%edx, %0" : "=g" (edx));
    // __asm__ volatile("movl %%edi, %0" : "=g" (edi));

    // printf("eax %x ebx %x ecx %x edx %x edi %x\n", eax, ebx, ecx, edx, edi);

    // syscallArg1 = '?';
    // syscallArg2 = 1;
    // sys_putc();
    // putc('?', stdout);

    // putc('!', stdout);

    // printf("OK 0x%X", 0x42);
    // fatal("This works !");

    // // Test : terminate 
    // char *argv[2];
    // argv[0] = "ABC";
    // argv[1] = "DEF";

    // printf("Exited with code %d\n", enter(myApp, 2, argv));


    // // --- FS --- //
    // // Example : ls directories //
    // puts("* ls root :");
    // FSEntry **rootEntries = root->ops->list(root);
    // printEntries(rootEntries);

    // FSEntry dir = *findEntry(rootEntries, "dir");

    // delEntries(rootEntries);

    // puts("* ls dir :");
    // FSEntry **dirEntries = dir.ops->list(&dir);
    // printEntries(dirEntries);

    // puts("* ls .. :");
    // FSEntry *parent = findEntry(dirEntries, "..");
    // FSEntry **parentEntries = parent->ops->list(parent);
    // printEntries(parentEntries);
    // delEntries(parentEntries);

    // delEntries(dirEntries);


    // // Example : cat file //
    // // TODO : frees
    // // puts("* cat /file :");
    // FSEntry **rootEntries2 = root->ops->list(root);
    // FSEntry *dir2 = findEntry(rootEntries2, "dir");
    // FSEntry **dirEntries2 = dir2->ops->list(dir2);
    // FSEntry *file = findEntry(dirEntries2, "second");

    // // Read
    // char buf[4096];
    // size_t n = FSEntry_read(file, buf, 2000);
    // buf[n] = '\0';
    // printf("%s\n", buf + 500);
    // printf("Read %d bytes\n", n);

    // delEntries(dirEntries2);
    // delEntries(rootEntries2);


    // // Example : Follow path //
    // FSEntry *f = getEntry("/dir/../dir/second");

    // if (f == NULL)
    //     puts("NULL");
    // else
    //     printf("File : %s\n", f->name);




    // Kernel pipe
    // File *p = Pipe_new();

    // fd_t pFd = p->fd;

    // char inData[4];
    // inData[0] = 'H';
    // inData[1] = 'E';
    // inData[2] = 'L';
    // inData[3] = 'O';

    // // write(pFd, inData, 4);
    // File_write(getFile(pFd), inData, 4);

    // char outData[5];

    // // read(pFd, outData, 4);
    // File_read(getFile(pFd), outData, 4);
    // outData[4] = '\0';

    // printf("Data in pipe : %s\n", outData);

    // Pipe_del(p);


    // // User pipe
    // fd_t pFd = pipe();

    // printf("Pipe no %d\n", pFd);

    // char inData[4];
    // inData[0] = 'H';
    // inData[1] = 'E';
    // inData[2] = 'L';
    // inData[3] = 'O';

    // printf("%d bytes written\n", write(pFd, inData, 4));

    // char outData[5];

    // read(pFd, outData, 4);
    // outData[4] = '\0';

    // printf("Data in pipe : %s\n", outData);

    // close(pFd);


    // // User write to std streams with syscalls
    // char stdinBuf[] = "Stdin data !";
    // size_t written = write(0, stdinBuf, sizeof(stdinBuf));

    // write(1, "Written in stdin : ", 19);
    // char buf[written];
    // read(0, buf, written);
    // write(1, buf, written);

    // write(2, "\nI am an error", 14);


    // // stdio test
    // fprintf(stderr, "Hello world ! 0x%X %d %s %p\n", 0x46454443, 42, "Hey world !", 0);

    // fputs("This is an error\n", stderr);
    // puts("This is NOT an error");













    char buf[64];
    // fputs("TEST\n", stdin);

    // printf("<%c> ", fgetc(stdin));
    // printf("<%c> ", fgetc(stdin));
    // printf("<%c> ", fgetc(stdin));
    // printf("<%c> ", fgetc(stdin));

    // char c;
    // read(0, &c, 1);
    // printf("<%c> ", c);
    // read(0, &c, 1);
    // printf("<%c> ", c);
    // read(0, &c, 1);
    // printf("<%c> ", c);
    // read(0, &c, 1);
    // printf("<%c> ", c);


    gets(buf);

    printf("buf = %s\n", buf);




    // char buf[64];
    // char dummy;
    // buf[0] = 'A';
    // buf[1] = 'B';
    // buf[2] = 'C';
    // buf[3] = '\0';
    // File *p = Pipe_new();

    // // FILE *f = malloc(sizeof(FILE));
    // // f->_fileno = p->fd;

    // write(p->fd, buf, 4);

    // printf("Read %d\n", read(p->fd, buf + 6, 1));
    // puts(buf + 6);


    // // read(p->fd, &dummy, 1); printf("%c\n", dummy);
    // // read(p->fd, &dummy, 1); printf("%c\n", dummy);
    // // read(p->fd, &dummy, 1); printf("%c\n", dummy);


    // // printf("%c\n", fgetc(f));
    // // printf("%c\n", fgetc(f));

    // // TODO : Pipes don't delete queue

    // Pipe_close(p);













    // TMP
    for (;;);




    // TODO : remove execApp files
    // TODO : add shell as app

    // Launch the shell
    const char *a = "/";
    const char *b = "/";
    char *shellArgv[2];
    shellArgv[0] = a;
    shellArgv[1] = b;

    sys_enter(shellMain, 2, shellArgv);

    consoleNewLine();
    puts("No process running");

    puts("Exiting");

}

// Terminates all modules
// !!! This function doesn't return
void terminateKernel()
{
    keyboardTerminate();
    filesTerminate();
    fatTerminate();
    fsTerminate();

    while (1);
}

// Entry from stage2
void main()
{
    initKernel();

    userAct();

    terminateKernel();
}
