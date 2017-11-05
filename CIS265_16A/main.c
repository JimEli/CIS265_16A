/*************************************************************************
* Title: (CIS265_16A) Date Difference Calculator
* File: main.c
* Author: James Eli
* Date: 9/21/2017
*
* Write a program that prompts the user to enter two dates, then prints the 
* difference between them, measured in days. Hint: Use the mktime and 
* difftime functions.
*
* Notes:
*  (1) I found the mktime and difftime functions restrictive (earliest
*      date is Unix epoch, 32-bit limit for max year is 2038 and 64-bit
*      limit for year is 3000). And I wanted greater range of dates than 
*      what mktime/difftime allows.
*  (2) The Julian date conversion is only accurate between years 1801 and
*      2099. Outside this range, julian date differs with gregorian 
*      calendar due to leap year determination.
*  (3) Program combines MSVC mktime/difftime functions with Julian date 
*      algorithm from U.S. Naval Observatory's "Almanac for Computers", 
*      published 1990 (discontinued). This is transparent to the user.
*  (4) As such, date constrained between 1801-3000 to insure accuracy.
*  (5) Accuraccy confirmed with embedded unit tests, see comments inside 
*      main() function.
*  (6) Compiled with MS Visual Studio 2017 Community (v141), using C
*      language options.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-265.
*************************************************************************
* Change Log:
*   09/21/2017: Initial release. JME
*   10/15/2017: Combined mktime/difftime & julian date algorithms. JME
*   10/28/2017: Moved date functions to separate files. JME
*************************************************************************/
#include "date.h"

// C/C++ Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

/**********************************************************************
* Return true if user enters a valid date in the proper format of
* "mm/dd/yyyy". The date entered is checked for proper month, year and
* day bounds (including leap year).
*********************************************************************/
bool getDate(Date *date, const char *prompt) {
	char s[STRING_LENGTH];             // String holds temporary user input.
	int attempts = MAX_INPUT_ATTEMPTS; // Input attempt counter.
	bool retVal = false;               // Return value from function, true == success.

	assert(attempts > 0); // Assert attempts valid non-zero, positive number.

	// Attempt only so many inputs.
	while (attempts--) {
		// Prompt and grab user input.
		fprintf(stdout, "Enter a %s date in the following format \"mm/dd/yyyy\": ", prompt);
		if (!fgets(s, STRING_LENGTH, stdin)) {
			fputs("\nFatal program error!\n", stderr);
			exit(EXIT_FAILURE);
		}
		else if (!strchr(s, '\n')) {
			// input too long, eat remainder.
			while (fgets(s, STRING_LENGTH, stdin) && !strchr(s, '\n'))
				; // Empty body.
			fputs("Too many characters input.\n", stdout);
		}
		else {
			// Attempt to convert from string to float, and validate.
			if (sscanf_s(s, "%2d/%2d/%4d", &date->month, &date->day, &date->year)) {
				if (!validateYear(date->year))
					fputs("Invalid year enetered.\n", stdout);
				else if (!validateMonth(date->month))
					fputs("Invalid month enetered.\n", stdout);
				else if (date->day >= 1 && date->day <= lastDayOfMonth(date->month, date->year)) {
					// Passed all checks, susccess! Return.
					retVal = true;
					break;
				}
				else
					fputs("Invalid day enetered.\n", stdout);
			}
		}
	}
	return retVal;
}

// Program starts here.
int main() {
	Date start = { 0, 0, 0 }, end;

#ifndef NDEBUG
	// Unit tests.
	fputs("DEBUG VERSION: Conducting validity check of consecutive dates. ", stdout);

	// Julian dates retrieved from: http://aa.usno.navy.mil/data/docs/JulianDate.php
	assert(2378861 == (int)julian((Date){ 1, 1, 1801 }));   // Julian date for 1/1/1801 = 2378861.
	assert(2440587 == (int)julian((Date){ 1, 1, 1970 }));   // Julian date for 1/1/1970 = 2440587.
	assert(2488068 == (int)julian((Date){ 12, 31, 2099 }));	// Julian date for 12/31/2099 = 2488068.

	int numDays = 0; // Consectutive date counter

	// Iterate consecutively through entire date range (1801 - 3000).
	for (int y = MIN_YEAR; y < (MAX_YEAR - 1); y++) {
		end.year = y;
		for (unsigned m = 1; m <= 12; m++) {
			end.month = m;
			for (unsigned int d = 1; d <= lastDayOfMonth(m, y); d++) {
				end.day = d;
				assert(numDays++ == (int)deltaDays((Date){ 1, 1, 1801 }, end));
			}
		}
	}
	fputs("Check passed.\n\n", stdout);
	// Re-initalize to invalid date.
	end = (Date){ 0, 0, 0 };
#endif

	fprintf(stdout, INSTRUCTIONS, MIN_YEAR, MAX_YEAR);

	// Get start & end dates and calculate delta.
	if (getDate(&start, "starting") && getDate(&end, "ending")) {
		double delta = deltaDays(start, end);
		fprintf(stdout, "Number of days between %d/%d/%4d", start.month, start.day, start.year);
		fprintf(stdout, " and %d/%d/%4d", end.month, end.day, end.year);
		fprintf(stdout, " = %g\n", delta);
	}
}

