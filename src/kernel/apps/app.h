#pragma once

// An app is a process

// Executes an application, returns -2 if not found
int exec(const char *app, int argc, char **argv);

// Executes an app by calling its main function
int execApp(int (*entry)(int argc, char **argv), int argc, char **argv);

// Terminates the current app
void terminateApp();

