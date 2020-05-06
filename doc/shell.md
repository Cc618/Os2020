# Shell

Os2020 provides a custom shell inspired by bash.

## Notes

You can terminate any program with Ctrl + C.

## Commands

Here is a list of usefull commands :

- cat
- cd
- color : Sets the color of the background / foreground of the console
- echo
- exit
- ls
- mkdir

## Redirections

You can redirect either stdout, stdin or stderr to a file by adding > (for stdout), < (for stdout) or 2> (for stderr) in a command :

```sh
-> echo Hello world > hw.txt
-> cat hw.txt
Hello world
```
