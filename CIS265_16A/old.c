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
*  (1) I found the mktime and difftime functions restrictive and 
*      unreliable.
*  (2) Date constrained between Unix epoch (1/1/1970) and max value for
*      32 or 64-bit int (2038 or 3000).
*  (3) 64-bit version of program allows greater range for ending date.
*  (4) Compiled with MS Visual Studio 2017 Community (v141), using C
*      language options.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-265.
*************************************************************************
* Change Log:
*   09/21/2017: Initial release. JME
*************************************************************************/
#include <assert.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fenv.h>

// C/C++ Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

// Un-comment to use 32-bit version of mktime/difftime functions.
//#ifndef _USE_32BIT_TIME_T
//#define _USE_32BIT_TIME_T 
//#endif

// Definitions.
#define MAX_INPUT_ATTEMPTS 3    // Maximum input attempts before aborting.
#define STRING_LENGTH      20   // Length of string holding user input.
#define CONVERSION_FAILURE -1   // Failure return value from mktime().
#define MIN_YEAR           1970 // Start of UNIX epoch.
// The latest time that can be represented with signed 32-bit integer time format is 03:14:07 UTC on Tuesday, 19 January 2038.
#ifdef _USE_32BIT_TIME_T
#define MAX_YEAR           2037 // Maximum end date (32-bit function).
#else
// Use 64-bit time.
#define MAX_YEAR           3000 // Maximum end date (64-bit functions).
#endif

struct  Date_ {
	int year;
	unsigned int month;
	unsigned int day;
};
typedef struct Date_ Date;

// Leap year validator.
// Logic: if year is divisible by 4, then its a leap year.
// But if that year is divisible by 100, then its not a leap year.
// However, if the year is also divisible by 400, then its a leap year.
inline bool isLeapYear(unsigned int year) {
	return ((!(year % 4) && (year % 100)) || !(year % 400));
}

//
unsigned int lastDayOfMonth(int y, unsigned int m) {
	unsigned int eom[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	return m != 2 || !isLeapYear(y) ? eom[m] : 29U;
}

// Return true if day is within proper range for month & year. 
inline bool validateDay(int y, unsigned int m, unsigned d) {
	return(d >= 1 && d <= lastDayOfMonth(y, m));
}

// Return true if month is within range 1-12.
inline bool validateMonth(unsigned int m) {
	return (m >= 1 && m <= 12);
}

// Return true if year is within range.
inline bool validateYear(unsigned int y) {
	return (y >= MIN_YEAR && y <= MAX_YEAR);
}

/**********************************************************************
* Return true if user enters a valid date in the proper format of
* "mm/dd/yyyy". The date entered is checked for proper month, year and
* day bounds (including leap year).
*********************************************************************/
bool getDate(Date *date) {
	char s[STRING_LENGTH];             // String holds temporary user input.
	int attempts = MAX_INPUT_ATTEMPTS; // Input attempt counter.
	bool retVal = false;               // Return value from function, true == success.

	assert(attempts > 0); // Assert attempts valid non-zero, positive number.

	// Attempt only so many inputs.
	while (attempts--) {
		// Prompt and grab user input.
		fputs("Enter a date in the following format \"mm/dd/yyyy\": ", stdout);
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
			if (sscanf(s, "%2d/%3d/%4d", &date->month, &date->day, &date->year)) {
				if (!validateYear(date->year)) {
					fputs("Invalid year enetered.\n", stdout);
				}
				else if (!validateMonth(date->month)) {
					fputs("Invalid month enetered.\n", stdout);
				}
				else if (date->day >= 1 && date->day <= lastDayOfMonth(date->year, date->month)) {
					// Passed all checks, susccess! Return.
					retVal = true;
					break;
				}
				else {
					fputs("Invalid day enetered.\n", stdout);
				}
			}
		}
	}
	return retVal;
}

// Program starts here.
int main(void) {
	struct tm date;
	double seconds;
	Date start, end;
	// Begining time cannot be a date before midnight, January 1, 1970.  
	// If using 32-bits, end date cannot be after 23:59:59 January 18, 2038 UTC, 
	// If using 64-bits, end date cannot be after 23:59:59, December 31, 3000, UTC. 
	time_t startTime, endTime;

	startTime = endTime = CONVERSION_FAILURE; // Assume failure.

	date.tm_hour = 12; // Hours since midnight (0-23).
	date.tm_min = 0;   // Minutes since full hour (0-59).
	date.tm_sec = 0;   // Seconds since full minute (0-60).
	date.tm_isdst = 0; // Daylight savings, standard time in effect.

	if (getDate(&start)) {
		printf("Start date: %d/%d/%4d\n", start.month, start.day, start.year);
		date.tm_mon = start.month - 1;      // Month since January (0-11).
		date.tm_mday = start.day;           // Days of month (1-31).
		date.tm_year = (start.year - 1900); // Year since 1900.
		// Convert struct tm to time_t.
		startTime = mktime(&date);
	}

	if (startTime != CONVERSION_FAILURE && getDate(&end)) {
		printf("End date: %d/%d/%4d\n", end.month, end.day, end.year);
		date.tm_mon = end.month - 1;      // Month since January (0-11).
		date.tm_mday = end.day;           // Days of month (1-31).
		date.tm_year = (end.year - 1900); // Year since 1900.
		// Convert struct tm to time_t.
		endTime = mktime(&date);

		if (endTime != CONVERSION_FAILURE) {
			// Take the difference between start and end time_t, which yields 
			// the difference between dates in seconds.
			seconds = difftime(endTime, startTime);
			// Divide the difference in seconds by 86400 to convert to days.
			printf("%g days difference\n", seconds / 86400.);
		}
	}
}

/*
int dayCount(int year, int month, int day) {
	int y, m;

	// WARNING: Dates before Oct, 1582 are inaccurate!
	// For algorithm, zero day is March 1, year 0. Note: this year 0 is NOT Gregorian 
	// year 0, it is a reference date only. However, calculations involving dates 
	// after the establishment of the Gregorian calendar will be correct. Therefore, 
	// start = dayCount(1582, 10, 1);
	m = (month + 9)%12; // Mar = 0, Feb = 11
	y = year - m/10;    // If Jan or Feb, year--
	return 365*y + y/4 - y/100 + y/400 + (m*306 + 5)/10 + (day - 1);
}
int dayCount(Date date) {
	int y, m;

	m = (date.month + 9)%12; // Mar = 0, Feb = 11
	y = date.year - m/10;    // If Jan or Feb, year--
	return 365*y + y/4 - y/100 + y/400 + (m*306 + 5)/10 + (date.day - 1);
}

//printf("%d\n", dayCount(end) - dayCount(start));
*/