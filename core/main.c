/*
 * Vue Python C example
 * Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
 * This code is under MIT license (see LICENSE.txt for details)
 */

#include "keygen.h"
#include <getopt.h>

int debug = 0;

void print_help( const char *name )
{
	int i;

	fprintf( stderr,"%s %s\n", PROGRAM, VERSION );
	fprintf( stderr, "Usage(s):\n"
					"  %s [options...] "
					"VERSION CODE DB EXPIRY WORKPLACES\n", name );
    fprintf( stderr, "Example(s):\n" );
    fprintf( stderr, "  %s 7521 AEAEFEFE W74 20.06.2018 128\n", name );
    fprintf( stderr, "  %s 7521 EE22FF00 T501 0 200\n", name );
    fprintf( stderr, "  %s --2 100 --3 200 7521 AAEEFF11 W70 11.12.2018 365\n",
			name );
	fprintf(stderr,
        "Options:\n"
        "  -h  --help            Display this message and exit.\n"
        "  -V  --version         Print program version and exit.\n"
        "  -v  --verbose         Explain what is being done.\n"
        "      --NUM VALUE       Set value for a feature.\n"
		"Arguments:\n"
		"  VERSION               Set the product version.\n"
		"  CODE                  Set the product code.\n"
		"  DB                    Set the database code.\n"
        "  EXPIRY                Set the expiry date (dd.mm.yyyy or 0).\n"
        "                        Zero means unlimited.\n"
        "  WORKPLACES            Set the number of workplaces.\n"
		"Features:\n"
        );

	for ( i = 0 ; i < MY_PARAMS_SIZE; i++ ) {
		fprintf( stderr, "  %s %s\n", myoptions[i].name, myoptions[i].text );
	}
}


int dialog( struct mykey *key, char *features ) {
	char *buf;
	size_t i;
	short n, v;
	int err, rv = 0;

	buf = malloc( MAX_BUF_SIZE * sizeof(char) );
	memset( buf, 0, MAX_BUF_SIZE * sizeof(char));
	mykey_init( key );

	printf("_\n");
	printf("| | _____ _   _  __ _  ___ _ __\n");
	printf("| |/ / _ \\ | | |/ _` |/ _ \\ '_ \\\n");
	printf("|   <  __/ |_| | (_| |  __/ | | |\n");
	printf("|_|\\_\\___|\\__, |\\__, |\\___|_| |_|\n");
	printf("          |___/ |___/\n");
	printf( "\n" );
	do {
		printf( "Enter your product version (e.g., 8101): " );
		if ( scanf( "%s", buf ) == EOF )
			goto EXIT;
		err = mykey_set_version( key, buf, strlen(buf) );
		if ( err < 0 ) 
			printf("Error: %s\n", get_error( err ) );
	} while ( err < 0 );

	do {
		printf( "Enter your product code (e.g., 414E51A3): " );
		if ( scanf( "%s", buf ) == EOF )
			goto EXIT;
		err = mykey_set_product( key, buf, strlen(buf) );
		if ( err < 0 ) 
			printf("Error: %s\n", get_error( err ) );
	} while ( err < 0 );

	do {
		printf( "Enter your database code (e.g., W648): " );
		if ( scanf( "%s", buf ) == EOF )
			goto EXIT;
		err = mykey_set_dbcode( key, buf, strlen(buf) );
		if ( err < 0 ) 
			printf("Error: %s\n", get_error( err ) );
	} while ( err < 0 );

	do {
		printf( "Enter expiration date (e.g., 31.12.2018 or 0): " );
		if ( scanf( "%s", buf ) == EOF )
			goto EXIT;
		err = mykey_set_expiry( key, buf, strlen(buf) );
		if ( err < 0 ) 
			printf("Error: %s\n", get_error( err ) );
	} while ( err < 0 );

	do {
		printf( "Enter the number of workplaces (e.g., 200): " );
		if ( scanf( "%s", buf ) == EOF )
			goto EXIT;
		err = mykey_set_places( key, buf, strlen(buf) );
		if ( err < 0 ) 
			printf("Error: %s\n", get_error( err ) );
	} while ( err < 0 );

	printf( "Want to customise features manually? (y/n): " );
	if ( scanf( "%s", buf ) == EOF )
		goto EXIT;

	if ( strcmp( buf, "y" ) == 0 ) {
		while ( 1 ) {
			printf( "Enter feature number or "
					"'L' to list or 'q' to finish: " );
			if ( scanf( "%s", buf ) == EOF )
				goto EXIT;

			if ( strcmp( buf, "q" ) == 0 )
				break;
			if ( strcmp( buf, "L" ) == 0 ) {
				for ( i = 0 ; i < MY_PARAMS_SIZE; i++ ) 
					printf( "  %s %s = %d\n",
							myoptions[i].name,
							myoptions[i].text,
							key->params[i] );
				continue;
			}

			if ( sscanf( buf, "%hd", &n ) == 0 ) {
				printf("Error: Invalid number. Only digits are available\n" );
				continue;
			}

			if ( !isparams(n) ) {
				printf("Error: Invalid feature '%hd'.\n", n );
				continue;
			}

			printf( "Enter value for %s (%s): ",
					myoptions[n].name,
 					myoptions[n].text );
			if ( scanf( "%hd", &v ) == 0 ) {
				printf("Error: Invalid number. Only digits are available\n" );
				continue;
			}
			key->params[n] = v;
		}
	} 

	err = mykey_generate( key, buf, MAX_BUF_SIZE );
	if ( err < 0 ) {
		printf("Error: %s\n", get_error( err ) );
		rv = abs(err);
		goto END;
	}

	printf( "Your code is right here:\n" );
	printf( "%s\n", buf );
	printf( "Have a nice day!\n" );

END:
	printf( "Press ENTER to continue..." );
	getchar();
	getchar();
EXIT:
	return rv;
}


int main( int argc, const char *argv[] )
{
	struct mykey key;
	char *buf;
	char *features = NULL;
	int c, err, ind;
	int long_index = 0;
	short n;

	const char* short_options = "hVvp:d:e:n:";
	const struct option long_options[] = {
		{ "help", no_argument, NULL,'h' },
    	{ "version", no_argument, NULL, 'V' },
    	{ "verbose", no_argument, NULL, 'v' },
		{ myoptions[SYSTEM_KERNEL].name,
			required_argument, NULL, SYSTEM_KERNEL },
		{ myoptions[SCHEDULE].name,
			required_argument, NULL, SCHEDULE },
		{ myoptions[SERVICE_ACCOUNTING].name,
			required_argument, NULL, SERVICE_ACCOUNTING },
		{ myoptions[PHARMACY].name, required_argument, NULL, PHARMACY },
		{ myoptions[IMAGE_PROCESSING].name,
			required_argument, NULL, IMAGE_PROCESSING },
		{ myoptions[CASHBOX].name,
			required_argument, NULL, CASHBOX },
		{ myoptions[CALL_ACCOUNTING].name,
			required_argument, NULL, CALL_ACCOUNTING },
		{ myoptions[REMOTE_ACCESS].name,
			required_argument, NULL, REMOTE_ACCESS },
		{ myoptions[LABORATORY].name,
			required_argument, NULL, LABORATORY },
		{ NULL, no_argument, NULL, 0 }
	};


	mykey_init( &key );

	while ( ( c = getopt_long( argc,
					(char**)argv,
					short_options,
					long_options,
					&long_index ) ) != -1 ) {
		switch ( c ) {
            case 'h':
				print_help( argv[0] );
				return 1;
            case '?':
				fprintf( stderr,
						"Try '%s --help' for more information.\n",
						argv[0] );
                return 1;
            case 'V':
				fprintf( stdout,"%s %s\n", PROGRAM, VERSION );
                return 0;
            case 'v':
                debug += 1;
                break;
			default: 
				if ( isparams(c) ) {
					if ( sscanf( optarg, "%hd", &n ) == 0 ) {
						fprintf( stderr,
								"%s: Invalid number. "
								"Only digits are available.\n",
								argv[0] );
						return 1;
					}
					mykey_set_option( &key, c, n );
				}
				break;
		}
	}

	if ( argc - optind == 0 ) {
		return dialog( &key, features );
	} else if ( argc - optind != 5 ) {
			fprintf( stderr,
					"%s: Not enough arguments.\n",
					argv[0] );
			fprintf( stderr,
					"Try '%s --help' for more information.\n",
					argv[0] );
		return 1;
	}

	buf = malloc( sizeof(char) * MAX_BUF_SIZE );
	memset( buf, 0, MAX_BUF_SIZE );

	ind = optind;
	if ( debug > 0 )
		printf( "Product version: %s\n", argv[ind] );
	err = mykey_set_version( &key, (char*)argv[ind], strlen(argv[ind]) );
	if ( err < 0 ) { 
		printf("%s: %s\n", argv[0], get_error( err ) );
		return abs(err);
	}

	ind++;
	if ( debug > 0 )
		printf( "Product code: %s\n", argv[ind] );
	err = mykey_set_product( &key, (char*)argv[ind], strlen(argv[ind]) );
	if ( err < 0 ) { 
		printf("%s: %s\n", argv[0], get_error( err ) );
		return abs(err);
	}

	ind++;
	if ( debug > 0 )
		printf( "Database code: %s\n", argv[ind] );
	err = mykey_set_dbcode( &key, (char*)argv[ind], strlen(argv[ind]) );
	if ( err < 0 ) { 
		printf("%s: %s\n", argv[0], get_error( err ) );
		return abs(err);
	}

	ind++;
	if ( debug > 0 )
		printf( "Expiry date: %s\n", argv[ind] );
	err = mykey_set_expiry( &key, (char*)argv[ind], strlen(argv[ind]) );
	if ( err < 0 ) { 
		printf("%s: %s\n", argv[0], get_error( err ) );
		return abs(err);
	}

	ind++;
	if ( debug > 0 )
		printf( "Workplaces: %s\n", argv[ind] );
	err = mykey_set_places( &key, (char*)argv[ind], strlen(argv[ind]) );
	if ( err < 0 ) { 
		printf("%s: %s\n", argv[0], get_error( err ) );
		return abs(err);
	}

	err = mykey_generate( &key, buf, MAX_BUF_SIZE );
	if ( err < 0 ) { 
		printf("%s: %s\n", argv[0], get_error( err ) );
		return abs(err);
	}
	if ( debug > 0 )
		printf( "Code: " );
	printf("%s\n", buf );

	return 0;
}
