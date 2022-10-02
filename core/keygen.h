/*
 * Vue Python C example
 * Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
 * This code is under MIT license (see LICENSE.txt for details)
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#define PROGRAM "keygen"
#define VERSION "0.1"
#define MAX_BUF_SIZE 1024

enum myerrno {
	INVALID_VERSION_SIZE = -10,
	INVALID_VERSION_NUMBER = -11,
	INVALID_PRODUCT_SIZE = -12,
	INVALID_PRODUCT_NUMBER = -13,
	INVALID_WORKPLACES_SIZE = -14,
	INVALID_WORKPLACES_NUMBER = -15,
	INVALID_DBCODE_SIZE = -16,
	INVALID_EXPIRY_FORMAT = -17,
	INVALID_EXPIRY_DATE = -18,
	INVALID_OPTION_NUMBER = -19,
};

enum myparams {
	SYSTEM_KERNEL = 0,
	SCHEDULE = 1,
	SERVICE_ACCOUNTING = 2,
	PHARMACY = 3,
	IMAGE_PROCESSING = 4,
	CASHBOX = 5,
	CALL_ACCOUNTING = 6,
	REMOTE_ACCESS = 7,
	LABORATORY = 8,
	MY_PARAMS_SIZE,
};

struct mykey {
	char product[5];
	char dbcode[11];
	unsigned short version;
	unsigned short expiry;
	short workplaces;
	short params[MY_PARAMS_SIZE];
};

struct myoption {
	char *name;
	char *text;
};

int isparams( int n );
char *get_error( int errnum );
int get_option_max_size( void );
char *get_option_name( short index );
char *get_option_description( short index );

struct mykey *mykey_alloc( void );
void mykey_free( struct mykey *key );
int mykey_init( struct mykey *key );
short mykey_get_option( struct mykey *key, short index );
int mykey_set_option( struct mykey *key, short index, short value );
int mykey_set_version( struct mykey *key, char *src, size_t size );
int mykey_set_product( struct mykey *key, char *src, size_t size );
int mykey_set_expiry( struct mykey *key, char *src, size_t maxsize );
int mykey_set_dbcode( struct mykey *key, char *src, size_t maxsize );
int mykey_set_places( struct mykey *key, char *src, size_t size );
int mykey_generate( struct mykey *key, char *dst, size_t maxsize );

extern const struct myoption myoptions[];
