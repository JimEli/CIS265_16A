# Pima CC CIS265 Week #16 Project A

Write a program that prompts the user to enter two dates, then prints the difference between them, measured in days. 

## Hint: 

Use the `mktime` and `difftime` functions.

## Notes:

1. I found the mktime and difftime functions restrictive (earliest date is Unix epoch, 32-bit limit for max year is 2038 and 64-bit limit for year is 3000). And I wanted greater range of dates than what mktime/difftime allows.
2. The Julian date conversion is only accurate between years 1801 and 2099. Outside this range, julian date differs with gregorian calendar due to leap year determination.
3. Program combines MSVC mktime/difftime functions with Julian date  algorithm from U.S. Naval Observatory's "Almanac for Computers",  published 1990 (discontinued). This is transparent to the user.
4. As such, date constrained between 1801-3000 to insure accuracy.
5. Accuraccy confirmed with embedded unit tests, see comments inside main() function.
6. Compiled with MS Visual Studio 2017 Community (v141), using C language options.

*Submitted in partial fulfillment of the requirements of PCC CIS-265.*
