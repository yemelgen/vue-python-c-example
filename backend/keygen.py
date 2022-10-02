#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Vue Python C example
# Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
# This code is under MIT license (see LICENSE.txt for details)

import sys
import os
from ctypes import *

curdir = os.path.dirname( os.path.abspath( __file__ ) )
lib = cdll.LoadLibrary( os.path.join( curdir, 'libsecret.x64.so') )

get_error = lib.get_error
get_error.restype = c_char_p
get_error.argtypes = ( c_int, )

get_option_max_size = lib.get_option_max_size
get_option_max_size.restype = c_int

get_option_name = lib.get_option_name
get_option_name.argtypes = ( c_short, )
get_option_name.restype = c_char_p

get_option_description = lib.get_option_description
get_option_description.argtypes = ( c_short, )
get_option_description.restype = c_char_p

mykey_alloc = lib.mykey_alloc
mykey_alloc.restype = c_void_p

mykey_free = lib.mykey_free
mykey_free.argtypes = ( c_void_p, )

mykey_init = lib.mykey_init
mykey_init.argtypes = ( c_void_p, )

mykey_get_option = lib.mykey_get_option
mykey_get_option.restype = c_short
mykey_get_option.argtypes = ( c_void_p, c_short )

mykey_set_option = lib.mykey_set_option
mykey_set_option.restype = c_int
mykey_set_option.argtypes = ( c_void_p, c_short, c_short )

mykey_set_product = lib.mykey_set_product
mykey_set_product.restype = c_int
mykey_set_product.argtypes = ( c_void_p, c_char_p, c_size_t )

mykey_set_dbcode = lib.mykey_set_dbcode
mykey_set_dbcode.restype = c_int
mykey_set_dbcode.argtypes = ( c_void_p, c_char_p, c_size_t )

mykey_set_version = lib.mykey_set_version
mykey_set_version.restype = c_int
mykey_set_version.argtypes = ( c_void_p, c_char_p, c_size_t )

mykey_set_expiry = lib.mykey_set_expiry
mykey_set_expiry.restype = c_int
mykey_set_expiry.argtypes = ( c_void_p, c_char_p, c_size_t )

mykey_set_places = lib.mykey_set_places
mykey_set_places.restype = c_int
mykey_set_places.argtypes = ( c_void_p, c_char_p, c_size_t )

mykey_generate = lib.mykey_generate
mykey_generate.restype = c_int
mykey_generate.argtypes = ( c_void_p, POINTER(c_char), c_size_t )

if __name__ == '__main__':
    def check_error( errno ):
        if errno != 0:
            print( 'Error: {}'.format( get_error( errno ).decode()) )
            sys.exit(errno)
    def print_help( program ):
        print('Usage:')
        print('  {} VERSION CODE DB EXPIRY WORKPLACES'.format(program) )
        print('Example:')
        print('  {} 7521 AEAEFEFE W745 13.06.2020 200'.format(program) )
        print('Features:')
        max_params = get_option_max_size()
        for idx in range(0, max_params):
            name = get_option_name( idx ).decode()
            text = get_option_description( idx ).decode()
            print( '  {} {}'.format( name, text)  )
    
    if len(sys.argv) < 5 or '-h' in sys.argv:
        print_help( sys.argv[0] )
        sys.exit(1)
    version = bytes(sys.argv[1], 'ascii')
    product = bytes(sys.argv[2], 'ascii')
    code = bytes(sys.argv[3], 'ascii')
    expiry = bytes(sys.argv[4], 'ascii')
    workplaces = bytes(sys.argv[5], 'ascii')

    buf = create_string_buffer(1024)
    key = mykey_alloc()
    mykey_init( key )
    check_error( mykey_set_product( key, product, len(product) ) )
    check_error( mykey_set_dbcode( key, code, len(code) ) )
    check_error( mykey_set_version( key, version, len(version) ) )
    check_error( mykey_set_expiry( key, expiry, len(expiry) ) )
    check_error( mykey_set_places( key, workplaces, len(workplaces) ) )
    check_error( mykey_generate( key, buf, c_size_t(1024) ) )
    mykey_free(key)
    print( buf.value.decode() )
