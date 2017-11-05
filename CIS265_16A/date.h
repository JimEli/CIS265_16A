/*************************************************************************
* Title: (CIS265_16A) Date Difference Calculator
* File: date.h
* Author: James Eli
* Date: 10/28/2017
*
* Date function header file.
*
* Notes:
*  (1) The Julian date conversion is only accurate between years 1801 and
*      2099. Outside this range, julian date differs with gregorian
*      calendar due to leap year determination.
*  (2) Program combines MSVC mktime/difftime functions with Julian date
*      algorithm from U.S. Naval Observatory's "Almanac for Computers",
*      published 1990 (discontinued). This is transparent to the user.
*  (3) As such, date constrained between 1801-3000 to insure accuracy.
*  (4) Compiled with MS Visual Studio 2017 Community (v141), using C
*      language options.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-265.
*************************************************************************
* Change Log:
*   09/21/2017: Initial release. JME
*   10/15/2017: Combined mktime/difftime & julian date algorithms. JME
*   10/28/2017: Moved date functions to separate files. JME
*************************************************************************/
#include <assert.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fenv.h>

// Un-comment to use 32-bit version of mktime/difftime functions.
//#ifndef _USE_32BIT_TIME_T
//#define _USE_32BIT_TIME_T 
//#endif

// Definitions.
#define MAX_INPUT_ATTEMPTS 3         // Maximum input attempts before aborting.
#define STRING_LENGTH      20        // Length of string holding user input.

#define JULIAN_UNIX_EPOCH  2440587.5 // Julian date representing 1/1/1970 (midday).
#define UNIX_EPOCH_YEAR    1970      // Year of Unix Epoch.
#define TIME_T_UNIX_EPOCH  68400     // time_t value representing 1/1/1970.

#define SECONDS_PER_DAY    86400.    // Number of seconds in a 24-hour day.
#define MIN_YEAR           1801      // Minimum acceptable date (year).
// The latest time that can be represented with signed 32-bit integer time format is 03:14:07 UTC on Tuesday, 19 January 2038.
#ifdef _USE_32BIT_TIME_T
#define MAX_YEAR           2037      // Maximum end date (32-bit function).
#else
// Use 64-bit time.
#define MAX_YEAR           3000      // Maximum end date (64-bit functions).
#endif

#define INSTRUCTIONS	"This program calculates the difference in days between two calendar" \
						" dates.\nThe dates must fall between the year %d and %d.\n\n"

// Simple date structure.
struct  Date_ {
	unsigned int month;
	unsigned int day;
	int year;
};

typedef struct Date_ Date;

// Return -1, 1 or 0 based upon sign of input.
static inline double sign(const double x);

// Leap year validator.
static inline bool isLeapYear(const unsigned int year);
// Calculate last day of given year/month, taking leap year into account.
unsigned int lastDayOfMonth(const unsigned int m, const int y);

// Return true if day is within proper range for month & year. 
inline bool validateDay(const unsigned int, const unsigned int, const int);
// Return true if month is within range 1-12.
inline bool validateMonth(const unsigned int);
// Return true if year is within range.
inline bool validateYear(const unsigned int);

// Julian date from gregorian calendar year, month and day.
double julian(const Date date);
// Converts a date to time_t value.
time_t convertDateToTime(const Date date);
// Expanded range algorithm.
double deltaDays(const Date start, const Date end);

/* Following functions included here to allow inlining. */

// Return true if day is within proper range for month & year. 
inline bool validateDay(const unsigned int m, const unsigned int d, const int y) {
	return(d >= 1 && d <= lastDayOfMonth(m, y));
}

// Return true if month is within range 1-12.
inline bool validateMonth(const unsigned int m) {
	return (m >= 1 && m <= 12);
}

// Return true if year is within range.
inline bool validateYear(const unsigned int y) {
	return (y >= MIN_YEAR && y <= MAX_YEAR);
}
