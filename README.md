# WorkOMeter
WorkOMeter is a command line tool to manage your pending tasks.
**********************
** NOTES **
**********************

This is a Tasks administrator. You can create Tasks and manage them.
This program uses github.com/nlohmann/json for JSON files management.

Feel free to use and edit this code as you want.


Compiling and Installing:

Dont forget to add -std=c++11 to the compiler for this to work.

Recommended G++ compiling line for linux: g++ -w *.cpp -o WorkOMeter.out -std=c++11


For usage i recommend creating an alias that calls 'WorkOMeter.out -show' and add it in .bashrc.
So when console starts-up it will show your current tasks

Commands:

WorkOMeter.out -Show
	Shows all your current tasks
WorkOMeter.out -add taskname number
Adds a task to the list. Number being the completion percentage %.
