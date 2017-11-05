/*************************************************************************
* Title: (CIS265_16A) Date Difference Calculator
* File: date.c
* Author: James Eli
* Date: 10/28/2017
*
* Date functions.
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
#include "date.h"

// C/C++ Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

// Return -1, 1 or 0 based upon sign of input.
static inline double sign(const double x) {
	return (x > 0) - (x < 0);
}

// Leap year validator.
// Logic: if year is divisible by 4, then its a leap year.
// But if that year is divisible by 100, then its not a leap year.
// However, if the year is also divisible by 400, then its a leap year.
static inline bool isLeapYear(const unsigned int year) {
	return ((!(year % 4) && (year % 100)) || !(year % 400));
}

// Calculate last day of given year/month, taking leap year into account.
unsigned int lastDayOfMonth(const unsigned int m, const int y) {
	unsigned int eom[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	assert(m >= 1 && m <= 12);
	return m != 2 || !isLeapYear(y) ? eom[m] : 29U;
}

// Julian date from gregorian calendar year, month and day.
// Where:
//   year is (1801 <= y <= 2099), 
//   month is (1 <= m <= 12), 
//   day of the month is (1 <= d <= 31).
double julian(const Date date) {
	double jday;

	if (fesetround(FE_TOWARDZERO)) // Returns 0 if rounding mode set properly.
		fputs("Floating point rounding mode not set, results may be inaccurate.\n", stdout);
	// Compute julian date (see project notes for source of algorithm).
	jday = 367.*date.year - rint((7.*(date.year + rint((date.month + 9.) / 12.))) / 4.) + rint((275.*date.month) / 9.);
	jday += (date.day + 1721013.5 - 0.5*sign(100 * date.year + date.month - 190002.5) + 0.5);

	return jday;
}

// Converts a date to time_t value.
time_t convertDateToTime(const Date date) {
	struct tm tmDate;

	tmDate.tm_hour = 12; // Hours since midnight (0-23).
	tmDate.tm_min = 0;   // Minutes since full hour (0-59).
	tmDate.tm_sec = 0;   // Seconds since full minute (0-60).
	tmDate.tm_isdst = 0; // Daylight savings, standard time in effect.
	tmDate.tm_mon = date.month - 1;      // Month since January (0-11).
	tmDate.tm_mday = date.day;           // Days of month (1-31).
	tmDate.tm_year = (date.year - 1900); // Year since 1900.
	// Convert struct tm to time_t.
	return mktime(&tmDate);
}

// Expanded range algorithm.
double deltaDays(const Date start, const Date end) {
	double delta; // days between dates, return value.

	// If both dates prioir to Unix epoch, use only julian calculation.
	if (start.year < UNIX_EPOCH_YEAR && end.year < UNIX_EPOCH_YEAR)
		delta = julian(end) - julian(start);
	// Take the difference between start and end time_t, which yields the difference between dates 
	// in seconds. Divide the difference in seconds by 86400 (SECONDS_PER_DAY) to convert to days.
	else if (start.year < UNIX_EPOCH_YEAR)
		delta = julian(start) - JULIAN_UNIX_EPOCH + difftime(TIME_T_UNIX_EPOCH, convertDateToTime(end)) / SECONDS_PER_DAY;
	else if (end.year < UNIX_EPOCH_YEAR)
		delta = julian(end) - JULIAN_UNIX_EPOCH + difftime(TIME_T_UNIX_EPOCH, convertDateToTime(start)) / SECONDS_PER_DAY;
	else
		// For both dates after Unix epoch, use just the mktime/difftime functions.
		delta = difftime(convertDateToTime(start), convertDateToTime(end)) / SECONDS_PER_DAY;

	return fabs(delta);
}
