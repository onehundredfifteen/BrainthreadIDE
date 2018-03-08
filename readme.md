# brainthread IDE
A brainfuck language family development environment

Features:
* Syntax highlighting*
* Runs _every_ brainfuck derivative (if an interpreter provided)  
* Runs Brainfuck, pBrain, Brainfork and Brainthread code using built-in interpreter
* Has step-by-step _debugging_ tools
* Has plugin system. You can write plugins yourself
* Number and text generator included**. 


Brainthread language:
* is Brainfuck compatible
* has functions from pBrain (function call command is *, not : )
* has threading from Brainfork: { 'fork' inhanced by control commands } 'join' and ! 'terminate' 
* has heaps: the command & is 'push', ^ 'pop' and % 'swap'. A heap command preceded by ~ causes the shared heap to be used. Threads can commnicate this way.
* introduces integer input and output ( ; and : commands)

Some screenshots
![Screenshot](screen1.png)
![Screenshot](screen2.png)

/*Syntax highlighting only for Brainfuck, pBrain, Brainfork, Brainthread, Brainlove and Toadskin. If you want more, contact me.
** Strings aren't available yet./

# how to run
Download all files from _Release_ folder then run Brainthread.exe.
Application does not have to be installed.
