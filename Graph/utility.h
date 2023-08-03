/* utility functions for parsing graph specificiation from strings */
/* copyright 2020 jamie twycross, jamie.twycross AT nottingham.ac.uk */
/* released under GNU GPL version 3 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>

#define	DELIM	':' /* delimiter in edge specification */

int convert_integer(char *);
double convert_double(char *);
int parse_edge(char *, int *, int *, double *);

/* convert a string to an integer */
int convert_integer(char *str)
{
	char *endptr;
	long val; /* converted number */

	/* return error if string NULL */
	if(!str)
		return INT_MAX;

	/* check if string empty or contains leading spaces */
	if (str[0] == '\0' || isspace((unsigned char) str[0]))
        return INT_MAX; /* return error */

	/* convert string to long integer */
	errno = 0;
	val = strtol(str, &endptr, 10);

	/* check if error converting number */
	if(errno != 0)
        return INT_MAX; /* return error */

	/* check entered value in int range */
	if(val <= INT_MIN || val >= INT_MAX)
        return INT_MAX; /* return error */

	/* check converted number bigger than minimum integer */
	if(val <= INT_MIN || errno == ERANGE)
        return INT_MAX; /* return error */

	/* check entire string successfully converted */
	if(endptr == str || *endptr != '\0')
        return INT_MAX; /* return error */

	/* success - return converted integer */
	return (int) val;
}

/* convert a string to a double */
double convert_double(char *str)
{
	char *endptr;
	double val; /* converted number */

	/* return error if string NULL */
	if(!str)
		return DBL_MAX;

	/* check if string empty or contains leading spaces */
	if (str[0] == '\0' || isspace((unsigned char) str[0]))
        return DBL_MAX; /* return error */

	/* convert string to long integer */
	errno = 0;
	val = strtod(str, &endptr);

	/* check if error converting number */
	if(errno != 0)
        return DBL_MAX; /* return error */

	/* check entire string successfully converted */
	if(endptr == str || *endptr != '\0')
        return DBL_MAX; /* return error */

	/* success - return converted integer */
	return val;
}

/* get vertices and weight from edge specification */
/* return 0 if parsing failed, 1 if parsing successful */
int parse_edge(char *spec, int *v1, int *v2, double *w)
{
	int ccount, dcount;
	char *s;
	void *vals[2];

	/* fail on empty string or NULL pointers */
	if(!spec || !strlen(spec))
		return 0;

	/* parse vertices */
	s = spec;
	ccount = 0; /* number of characters read */
	dcount = 0; /* number of delimiters found */
	while(*s) {
		/* reached delimiter */
		if(*s == DELIM) {
			/* fail if no characters read for empty substring */
			if(!ccount)
				break;
			if(!(vals[dcount] = calloc(ccount + 1, sizeof(char))))
				break;
			/* copy string */
			memcpy(vals[dcount], (void *) (s - ccount), ccount);
			ccount = 0;
			dcount++;
		} else
			ccount++;
		s++;
	}

	/* parse weight */
	if(dcount == 2) {
		*w = convert_double(s - ccount);
		if(*w != DBL_MAX)
			dcount++;
	}

	/* test if spec correctly parsed */
	if(dcount != 3) {
		/* not correctly parsed, so free allocated memory */
		while(dcount--)
			free(vals[dcount]);
		return 0;
	}

	/* correctly parsed, so set return pointers */
	*v1 = convert_integer((char *) vals[0]);
	*v2 = convert_integer((char *) vals[1]);

	/* free memory */
	free(vals[1]);
	free(vals[0]);

	if(*v1 == INT_MAX || *v2 == INT_MAX)
		return 0;

	return 1;
}
