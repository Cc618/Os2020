#pragma once

// An app is a process

// Executes an application, returns -2 if not found
int exec(const char *app, int argc, char **argv);

// Executes an app by calling its main function
int execApp(int (*entry)(int argc, char **argv), int argc, char **argv);

// TODO :
// WIP Section //
// // Executes an app by calling its main function
// // #define execApp(ENTRY, ARGC, ARGV) _execApp((void*)__current_pc, ENTRY, ARGC, ARGV)
// #define execApp(ENTRY, ARGC, ARGV) _execApp((void*)0, ENTRY, ARGC, ARGV)
// // - from : %eip when we call _execApp
// int _execApp(void *from, int (*entry)(int argc, char **argv), int argc, char **argv);

// // Terminates the current app
// void terminateApp();

