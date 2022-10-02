/*
 * Vue Python C example
 * Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
 * This code is under MIT license (see LICENSE.txt for details)
 */

#include "keygen.h"

unsigned char mysecret[128] = {
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,

};
const struct myoption myoptions[] = {
	[SYSTEM_KERNEL] = { "0", "System kernel" },
	[SCHEDULE] = { "1", "Schedule" }, 
	[SERVICE_ACCOUNTING] = { "2", "Service accounting" },
	[PHARMACY] = { "3", "Pharmacy" },
	[IMAGE_PROCESSING] = { "4", "Image processing" },
	[CASHBOX] = { "5", "Cashbox"},
	[CALL_ACCOUNTING] = { "6", "Call accounting" },
	[REMOTE_ACCESS] = { "7", "Remote access" },
	[LABORATORY] = { "8", "Laboratory" },
};

static int
encrypt( unsigned char *dst, size_t maxsize,
		unsigned char *src, size_t size )
{
	int i, j;
	unsigned char d;
	for ( i = 0, j = 0; i < size; i++, j++ ) {
		d = (unsigned char)i;
		dst[i] = src[i] ^ ( mysecret[j] ^ ( d * mysecret[j] ) );
		if ( j >= ( sizeof(mysecret) - 1 ) ) j = 0;
	}
	return i;
}

int isparams( int n )
{
	return 1 ? n >= 0 && n < MY_PARAMS_SIZE : 0;
}

struct mykey *mykey_alloc( void )
{
	struct mykey *key = malloc( sizeof(struct mykey) );
	return key;
}

void mykey_free( struct mykey *key )
{
	if ( key != NULL )
		free( key );
	return;
}

int mykey_init( struct mykey *key )
{
	int i;
	memset( key->product, 0, sizeof(((struct mykey*)0)->product) );
	memset( key->dbcode, 0, sizeof(((struct mykey*)0)->dbcode) );
	key->version = 0;
	key->expiry = 0;
	key->workplaces = 0;
	for ( i = 0; i < MY_PARAMS_SIZE; i++ )
		key->params[i] = -1;
	return 0;
}

int get_option_max_size( void )
{
	return MY_PARAMS_SIZE;
}

char *get_option_name( short index )
{
	return isparams(index) ? myoptions[index].name : NULL;
}

char *get_option_description( short index )
{
	return isparams(index) ? myoptions[index].text : NULL;
}

short mykey_get_option( struct mykey *key, short index )
{
	return isparams(index) ? key->params[index] : INVALID_OPTION_NUMBER;
}

int mykey_set_option( struct mykey *key, short index, short value )
{
	if ( isparams(index) ) {
		key->params[index] = value;
		return 0;
	}
	return INVALID_OPTION_NUMBER;
}

int mykey_set_version( struct mykey *key, char *src, size_t size )
{
	unsigned long n;
	char *endptr;

	n = strtoul( src, &endptr, 10 );

	if ( src == endptr )
		return INVALID_VERSION_NUMBER;
	else if ( endptr != src + size )
		return INVALID_VERSION_NUMBER;
	else if ( n == -1 && endptr != 0)
		return INVALID_VERSION_NUMBER;

	key->version = (short)n;

	return 0;
}

int mykey_set_product( struct mykey *key, char *src, size_t size )
{
	size_t i, j;
	char xstr[3];
	char *endptr;
	unsigned long n, z;

	z = sizeof(((struct mykey*)0)->product);
	memset( key->product, 0, z );

	if ( size != 0x8 )
		return INVALID_PRODUCT_SIZE;

	for ( i = 0, j = 0; j < z && i < size; ) {
		xstr[0] = src[i++];
		xstr[1] = src[i++];
		xstr[2] = 0;
		n = strtoul( xstr, &endptr, 16 );

		if ( endptr == xstr )
			return INVALID_PRODUCT_NUMBER;
		else if ( endptr != xstr + 2 )
			return INVALID_PRODUCT_NUMBER;
		else if ( n == -1 && endptr != 0)
			return INVALID_PRODUCT_NUMBER;

		key->product[j++] = (unsigned char)n;
	}

	return 0;
}

int mykey_set_expiry( struct mykey *key, char *src, size_t maxsize )
{
	struct tm cal;
	time_t t1, t2;
	double diff;
	unsigned int n;
	int year, month, day;

	cal.tm_year = 1989 - 1900;
	cal.tm_mon = 9 - 1; 
	cal.tm_mday = 17;
	cal.tm_hour = 0;
	cal.tm_min = 0;
	cal.tm_sec = 0;
    cal.tm_isdst = -1;

	t1 = mktime ( &cal );

	if ( strcmp( src, "0" ) == 0 ) {
		key->expiry = 0;
		return 0;
	}

   	if ( sscanf( src, "%2d.%2d.%4d", &day, &month, &year ) != 3 )
		return INVALID_EXPIRY_FORMAT;

	cal.tm_year = year - 1900;
	cal.tm_mon = month - 1; 
	cal.tm_mday = day;
	cal.tm_hour = 0;
	cal.tm_min = 0;
	cal.tm_sec = 0;
    cal.tm_isdst = -1;

	t2 = mktime ( &cal );

	if ( t2 == (time_t) -1 )
		return INVALID_EXPIRY_DATE;

	if ( t2 < t1  )
		return INVALID_EXPIRY_DATE;

	diff = difftime( t2, t1 );
	n  = (unsigned int)( diff / 60 / 60 / 24);

	key->expiry = n;

	return 0;
}

int mykey_set_dbcode( struct mykey *key, char *src, size_t maxsize )
{
	size_t i, z;

	z = sizeof(((struct mykey*)0)->dbcode);

	memset( key->dbcode, 0, z );

	if ( maxsize > z )
		return INVALID_DBCODE_SIZE;

	for ( i = 0; i < z && src[i] != '\0' && src[i] != '\n'; i++ ) 
		key->dbcode[i] = src[i];
	key->dbcode[i] = '\0';

	return 0;
}

int mykey_set_places( struct mykey *key, char *src, size_t size )
{
	int i;
	unsigned long n;
	char *endptr;

	n = strtoul( src, &endptr, 10 );

	if ( src == endptr )
		return INVALID_WORKPLACES_NUMBER;
	else if ( endptr != src + size )
		return INVALID_WORKPLACES_NUMBER;
	else if ( n == -1 && endptr != 0)
		return INVALID_WORKPLACES_NUMBER;

	if ( n >  0x8000 || n < 0 )
		return INVALID_WORKPLACES_SIZE;

	key->workplaces = (short)n;

	for ( i = 0; i < MY_PARAMS_SIZE; i++ ) {
		if ( key->params[i] == -1 )
			key->params[i] = key->workplaces;
	}
	return 0;
}

int mykey_generate( struct mykey *key, char *dst, size_t maxsize )
{
	unsigned char *buf, *enc, *p;
	size_t i, z;
	char xstr[3] = {0};
	short n;
	unsigned char b[2];
	unsigned char c;
	unsigned char eob[] = { 0xff, 0xff }; /* end of block */

	buf = malloc( MAX_BUF_SIZE );
	memset( buf, 0, MAX_BUF_SIZE * sizeof(char));
	enc = malloc( MAX_BUF_SIZE );
	memset( enc, 0, MAX_BUF_SIZE * sizeof(char));
	p = buf;
	
	p += 1;
	memcpy( p, key->product, 4 );
	p += 4;
	memcpy( p, &key->expiry, 2 );
	p += 2;

	for ( i = 0; i < MY_PARAMS_SIZE; i++ ) {
		n = key->params[i];
		if ( n < 0 ) {
			b[0] = 0;
			memcpy( p, b, 1 );
			p += 1;
		} else if ( n < 0x80 ) {
			b[0] = n;
			memcpy( p, b , 1 );
			p += 1;
		} else {
			b[0] = (n | 0x80 ) & 0xFF;
			b[1] = (n >> 7 )& 0xFF;
			memcpy( p, b , 2 );
			p += 2;
		}
	}
	
	memcpy( p, eob, 2 );
	p += 2;
	memcpy( p, &key->version, 2 );
	p += 2;

	c = strlen( key->dbcode ) & 0xFF;
	memcpy( p, &c, sizeof(char) );
	p += 1;
	memcpy( p, key->dbcode, c );
	p += c;
	memcpy( p, &(unsigned char){0}, 1 );
	p += 1;

	dst[0] = '\0';
	z = p - buf;

	encrypt( enc , z, buf, z );

	for ( i = 0; i < z; i++ ) {
		sprintf( xstr, "%02X", enc[i] );
		strcat( dst, xstr );
	}

	free( buf );
	free( enc );

	return 0;
}

char *get_error( int errnum )
{
	switch ( errnum ) {
		case INVALID_VERSION_SIZE:
			return "Invalid length of the version number";
		case INVALID_VERSION_NUMBER:
			return "Invalid number in the version number";
		case INVALID_PRODUCT_SIZE:
			return "Invalid length of the product code";
		case INVALID_PRODUCT_NUMBER:
			return "Invalid number in the product code";
		case INVALID_WORKPLACES_SIZE:
			return "Invalid number of workplaces. Must be no more than 255";
		case INVALID_WORKPLACES_NUMBER:
			return "Invalid number in the workplaces";
		case INVALID_DBCODE_SIZE:
			return "Invalid length of the database code";
		case INVALID_EXPIRY_FORMAT:
			return "Invalid expiry date format";
		case INVALID_EXPIRY_DATE:
			return "Expiry date must be more than 02/10/1989";
		case INVALID_OPTION_NUMBER:
			return "Invalid option number";
		default:
			return "unknown error";
	}
}
