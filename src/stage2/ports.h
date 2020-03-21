#pragma once

extern void outb(char data, short port);
extern void outw(short data, short port);

extern char inb(short port);
extern short inw(short port);
