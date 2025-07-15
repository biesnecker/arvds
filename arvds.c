#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int month_days[] = {0,   31,  59,  90,  120, 151, 181,
                           212, 243, 273, 304, 334, 365};

static int month_days_leap[] = {0,   31,  60,  91,  121, 152, 182,
                                213, 244, 274, 305, 335, 366};

static int is_leap_year(int year) {
    /* We're already checking to make sure that the date is > 0. */
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static int read_base_year(const char* year_str) {
    int base_year;
    if (sscanf(year_str, "%d", &base_year) != 1 || base_year < 0) {
        fprintf(stderr, "Invalid year: %s\n", year_str);
        exit(EXIT_FAILURE);
    }
    return base_year;
}

static void parse_datestamp(const char* datestamp, int* year, int* month,
                            int* day) {
    if (sscanf(datestamp, "%d-%d-%d", year, month, day) != 3) {
        fprintf(stderr, "Invalid date format: %s\n", datestamp);
        exit(EXIT_FAILURE);
    }
    if (*year < 0 || *month < 1 || *month > 12 || *day < 1 || *day > 31) {
        fprintf(stderr, "Invalid date values: %s\n", datestamp);
        exit(EXIT_FAILURE);
    }

    /* Check to make sure that the days are valid for the month. */
    if (*month == 2) {
        if (*day > 29 || (*day == 29 && !is_leap_year(*year))) {
            fprintf(stderr, "Invalid date values: %s\n", datestamp);
            exit(EXIT_FAILURE);
        }
    } else if (*month == 4 || *month == 6 || *month == 9 || *month == 11) {
        if (*day > 30) {
            fprintf(stderr, "Invalid date values: %s\n", datestamp);
            exit(EXIT_FAILURE);
        }
    } else if (*day > 31) {
        fprintf(stderr, "Invalid date values: %s\n", datestamp);
        exit(EXIT_FAILURE);
    }
}

static void parse_arvelie_date(const char* datestamp, int* year, char* period,
                               int* day) {
    int ayear, aday;
    char amonth;

    if (sscanf(datestamp, "%02d%c%02d", &ayear, &amonth, &aday) != 3) {
        fprintf(stderr, "Invalid arvelie date format: %s\n", datestamp);
        exit(EXIT_FAILURE);
    }

    if (ayear < 0 || aday < 0 || aday > 13 ||
        (amonth != '+' && (amonth < 'A' || amonth > 'Z'))) {
        fprintf(stderr, "Invalid arvelie date values1: %s\n", datestamp);
        exit(EXIT_FAILURE);
    }

    *year = ayear;
    *period = amonth;
    *day = aday;
}

static void handle_from_conversion(const char* year_str,
                                   const char* timestamp) {
    int year, month, day, base_year, day_of_year;
    int ayear, m, aday, leap_year;
    char amonth;

    base_year = read_base_year(year_str);
    parse_datestamp(timestamp, &year, &month, &day);

    if (year < base_year) {
        fprintf(stderr, "Year must be >= to the base year %d\n", base_year);
        exit(EXIT_FAILURE);
    }

    ayear = year - base_year;
    leap_year = is_leap_year(year);
    day_of_year =
        (leap_year ? month_days_leap[month - 1] : month_days[month - 1]) + day -
        1;

    m = day_of_year / 14;
    aday = day_of_year % 14;
    amonth = m < 26 ? (char)m + 'A' : '+';
    printf("%02d%c%02d\n", ayear, amonth, aday);
}

static void handle_to_conversion(const char* year_str, const char* timestamp) {
    int base_year, ayear, aday;
    int year, month, day, doy;
    int* md_ptr;
    char aperiod;

    base_year = read_base_year(year_str);
    parse_arvelie_date(timestamp, &ayear, &aperiod, &aday);

    year = base_year + ayear;
    doy = aday + (aperiod == '+' ? 26 * 14 : (aperiod - 'A') * 14);
    md_ptr = is_leap_year(year) ? month_days_leap : month_days;
    month = 0;
    while (month < 12 && doy >= (md_ptr)[month]) {
        month++;
    }
    day = doy - md_ptr[month - 1] + 1;
    if (month == 0) {
        month = 1;
    } else if (month > 12) {
        month = 12;
    }

    if (day < 1 || day > (md_ptr[month] - md_ptr[month - 1])) {
        fprintf(stderr, "Invalid arvelie date values: %s\n", timestamp);
        exit(EXIT_FAILURE);
    }

    printf("%04d-%02d-%02d\n", year, month, day);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr,
                "Usage: arvds <direction> <start-year> <timestamp>\n\n");

        fprintf(stderr,
                "arvds is a tool for converting an arvelie date to a\n"
                "YYYY-MM-DD date, and vice versa.\n\n");
        fprintf(stderr, "See: https://wiki.xxiivv.com/site/arvelie.html\n\n");

        fprintf(stderr, "Example usage:\n\n");

        fprintf(stderr, "  arvds from 2020 2020-01-01 -> 00A00\n");
        fprintf(stderr, "  arvds to 2020 03C07 -> 2024-02-04\n\n");
        fprintf(stderr, "Year must be greater than 0.\n\n");

        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "to") != 0 && strcmp(argv[1], "from") != 0) {
        fprintf(stderr, "Invalid direction: %s\n\n", argv[1]);
        fprintf(stderr,
                "'from' for YYYY-MM-DD to arvelie conversion; or\n\n"
                "'to'   for arvelie to YYYY-MM-DD conversion.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "to") == 0) {
        handle_to_conversion(argv[2], argv[3]);
    } else {
        handle_from_conversion(argv[2], argv[3]);
    }

    return EXIT_SUCCESS;
}
