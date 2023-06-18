# Comparative-analysis-of-all-Preemptive-Scheduling
Comparative analysis of Round Robin, Preemptive SJF &amp; Preemptive Priority scheduling using graphs.

## Introduction:
In this mini project we are going to create a program in C which has comparative analysis of all preemptive scheduling with bar chart.

## Explanation:
To complete the mini project, we have used certain header files and some user defined functions which are given below:

(a) stdio.h: “stdio.h” is the header file for standard input and output. This is useful for getting the input from the user(Keyboard) and output result text to the monitor(screen). In this header file some functions are used which are given below:

i. printf():
printf() function is used to print the character, string, float, integer, octal and hexadecimal values onto the output screen. We use printf() function with different format specifier to display different types of value.

For eg:- %d format specifier to display the value of an integer variable, %f format specifier to display the value of an float variable, %c format specifier to display the value of an character variable, etc.

ii. scanf():
scanf is a function that reads data with specified format form a given string stream source. The scanf function accept input from standard in, which for us is generally the keyboard. The scanf function can do a lot of different things, but can be unreliable because it doesn’t handle human errors very well. But for simple programs it’s good enough and easy to use.

iii. sprint():
sprint() function Composes a string with the same text that would be printed if format was used on printf, but instead of being printed, the content is stored as a C string in the buffer pointed by str. The first argument to sprintf() function is a pointer to the target string. The rest of the arguments are the same as for printf() function. The function writes the data in the string pointed to by str and returns the number of characters written to str, excluding the null character.

(b) graphics.h: The graphics.h header file provides access to a simple graphics library that makes it possible to draw lines, rectangles, ovals, arcs, polygons, images and strings on a graphical window. In this header file some functions are used which are given below:

i. bar():
Bar function is used to draw a 2-dimensional, rectangular filled in bar. Coordinates of left top and right bottom corner are required to draw the bar. Left specifies the X-coordinate of top left corner, top specifies the Y-coordinate of top left corner, right specifies the X-coordinate of right bottom corner, bottom specifies the Y-coordinate of right bottom corner. Current fill pattern and fill color is used to fill the bar.

ii. bar3d():
bar3d function is used to draw a 2-dimensional, rectangular filled in bar . Coordinates of left top and right bottom corner of bar are required to draw the bar. left specifies the X-coordinate of top left corner, top specifies the Y-coordinate of top left corner, right specifies the X-coordinate of right bottom corner, bottom specifies the Y-coordinate of right bottom corner, depth specifies the depth of bar in pixels, topflag determines whether a 3 dimensional top is put on the bar or not. Current fill pattern and fill color is used to fill the bar.

iii. closegraph():
closegraph function closes the graphics mode, deallocates all memory allocated by graphics system and restores the screen to the mode it was in before you called initgraph.

iv. line():
line function is used to draw a line from a point(x1,y1) to point(x2,y2) i.e. (x1,y1) and (x2,y2) are end points of the line. The code given below draws a line.

v. outtextxy():
outtextxy function display text or string at a specified point(x,y) on the screen.

vi. rectangle():
rectangle function is used to draw a rectangle. Coordinates of left top and right bottom corner are required to draw the rectangle. left specifies the X-coordinate of top left corner, top specifies the Y-coordinate of top left corner, right specifies the X-coordinate of right bottom corner, bottom specifies the Y-coordinate of right bottom corner.

vii. setcolor():
In Turbo Graphics each color is assigned a number. Total 16 colors are available. Strictly speaking number of available colors depends on current graphics mode and driver .
For Example:- BLACK is assigned 0, RED is assigned 4 etc. Setcolor function is used to change the current drawing color. e.g. setcolor(RED) or setcolor(4) hanges the current drawing color to RED. Remember that default drawing color is WHITE.

viii. setfillstyle():
setfillstyle function sets the current fill pattern and fill color.

ix. setlinestyle():
setlinestyle function which sets the style for all lines drawn by line, lineto, rectangle, drawpoly, and so on.

x. settextstyle():
settextstyle function is used to change the way in which text appears, using it we can modify the size of text, change direction of text and change the font of text.

(c) stdlib.h: stdlib.h is the header of the general purpose standard library of C programming language which includes functions involving memory allocation, process control, conversions and others.

(d) conio.h: conio.h is the header used in C programming to clear screen, change color of text and background, move text, check whether a key is pressed or not and many more. It contains functions for console input/output. In this header file some functions are used which are given below:

i. getch():
getch function is a way to get a user inputted character. It can be used to hold program execution, but the “holding” is simply a side-effect of its primary purpose, which is to wait until the user enters a character. getch function is used to provide an acknowledgement for characters.

(e) math.h: math.h is a header file in the standard library of the C programming language designed for basic mathematical operations. In this header file some functions are used which are given below:

i. round():
round function in C returns the nearest integer value of the float/double/long double argument passed to this function.
E.g.: If decimal value is from “0.6 to 0.9”, it returns the integer value greater than the argument.

User defined functions:

i. float round_robin( struct Process p[], int n, int time_quantum, int rt[]):
This function is used to calculate round robin scheduling. It first selects a process (according to fcfs) and check whether its burst time is less than time quantum. If it is, the process finishes execution. Otherwise, it checks whether the burst time is more than time quantum. In that case it executes according to time quantum. If the process finish execution, the waiting time is calculated. Ultimately the average waiting time is returned.

ii. float preemptive_sjf(Process proc[], int n, int rt[]):
This function is used to calculate preemptive sjf(shortest job first) scheduling. It first finds the process with minimum burst time as long as arrival time is less than current time. It then executes 1 unit of the burst time (of that selected process) and check the current process is zero or not. If it is zero, it calculates the waiting time otherwise repeats the algorithm. It finally print the waiting time and return the average waiting time.

iii. float preemptive_priority(Process proc[], int n, int tempburstTime[], int totalbursttime):
This function is used to calculate preemptive priority scheduling. It first insert all the process which are possible to execute according (arrival time is less than current time) into the Heap the schedule them. If the heapsize reach 0 and the inserted process is n then the execution of process stops. The waiting is also printed here.

iv. void scheduling(Process Heap[], Process proc[], int n,int* heapsize, int* currentTime):
This function is responsible for executing the highest priority extracted from Heap[]. It calls the extract minimum function to find the process with highest priority then if the burst time of that process is more than 0 then calls the insertt function to make the process eligible for execution.

v. Process extractminimum(Process Heap[], int*
heapsize, int* currentTime):
This function is used to find the process with highest priority from the heap. It also reorders the heap after extracting the highest priority process.

vi. void order(Process Heap[], int\* heapsize, int start):
It is used to reorder the heap according to priority after insertion of new process. It uses heap sort to reorder the heap.

vii. void insertt(Process Heap[], Process value, int* heapsize, int* currentTime):
This function is used to include all the valid and eligible processes in the heap for execution. Heapsize defines the number of processes in execution depending on the current time currentTime keeps a record of the current CPU time.

viii. int set_scale(float a, float b, float c):
This function is used to set the scale of the graph. In this function, we find the maximum value between a,b & c and use it to calculate the scale of the graph.
