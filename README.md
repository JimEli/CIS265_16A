# Pima CC CIS265 Week #16 Project A

Write a program that prompts the user to enter two dates, then prints the difference between them, measured in days. 

## Hint: 

Use the `mktime` and `difftime` functions.

## Notes:

1. I found the mktime and difftime functions restrictive. The earliest date is the Unix epoch (1/1/1970), and the 32-bit limit for the maximum year is 2038 with the 64-bit limit for being the year of 3000. I wanted a greater range of dates than what mktime/difftime allowed.
2. The included Julian date conversion is only accurate between the years 1801 and 2099. Outside this range, the julian date differs with the gregorian calendar due to differneces in leap year determination.
3. Therefore, the program combines MSVC mktime/difftime functions with a Julian date algorithm from the U.S. Naval Observatory's "Almanac for Computers", published 1990 (discontinued). This functionality is transparent to the user.
4. As such, the date is constrained between 1801-3000 to insure accuracy.
5. Accuraccy is confirmed with embedded unit tests, see comments inside main() function. These tests are only compiled under the debug version.
6. Compiled with MS Visual Studio 2017 Community (v141), using C language options.

*Submitted in partial fulfillment of the requirements of PCC CIS-265.*
