# Pima CC CIS265 Week #16 Project A

Write a program that prompts the user to enter two dates, then prints the difference between them, measured in days. Hint: Use the `mktime` and `difftime` functions.

Notes:
* I found the `mktime` and `difftime` functions restrictive and unreliable.
* Program uses Julian date algorithm from U.S. Naval Observatory's "Almanac for Computers", published 1990 (discontinued).
* Date conservatively constrained between 1801-2099 to insure accuracy inside the julian date conversion function.
* Accuraccy of julian date function confirmed with embedded unit tests, see comments inside `main` function.
* Compiled with MS Visual Studio 2017 Community (v141), using C language options.
