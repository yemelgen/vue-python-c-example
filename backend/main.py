#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Vue Python C example
# Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
# This code is under MIT license (see LICENSE.txt for details)

import json
from flask import Flask, request, jsonify
from werkzeug.exceptions import HTTPException
from datetime import datetime
from ctypes import create_string_buffer
import keygen

app = Flask( __name__ )

def get_date( isostring ):
    dt = datetime.strptime( isostring, '%Y-%m-%dT%H:%M:%S.%fZ' )
    return dt.strftime('%d.%m.%Y')

@app.route( '/api/menu', methods=['GET'] )
def get_options():
    data = []
    max_options = keygen.get_option_max_size()
    for idx in range(0, max_options):
        name = keygen.get_option_name( idx ).decode()
        text = keygen.get_option_description( idx ).decode()
        data.append( { 'id': name, 'label': text } )
    return jsonify( { 'items': data } )

@app.route( '/api/keys', methods=['POST'] )
def create_key():
    if request.json is None:
        return jsonify({
            'errors': {},
            'status': 400,
            'message': 'JSON format is required',
        }), 400
    version = request.json.get('version', None )
    code = request.json.get('code', None )
    dbcode = request.json.get('db', None )
    expdate = request.json.get('expdate', None )
    workplaces = request.json.get('workplaces', None )
    noexpdate = request.json.get('noexpdate', None )
    options = request.json.get('options', None )
    if None in [ version, code, dbcode, expdate, workplaces, options ]:
        return jsonify( 'not enough parameters'), 400
    version = bytes( version, 'ascii' )
    code = bytes( code, 'ascii' )
    dbcode = bytes( dbcode, 'ascii' )
    places = bytes( str(workplaces), 'ascii' )
    expiry = b'0' if noexpdate else bytes( get_date( expdate ), 'ascii')

    max_options = keygen.get_option_max_size()
    buf = keygen.create_string_buffer(1024)
    key = keygen.mykey_alloc()
    keygen.mykey_init( key )

    err =  keygen.mykey_set_version( key, version, len(version) )
    if err != 0:
        return jsonify({
            'status': 400,
            'message': keygen.get_error(err).decode()
        }), 400
    err =  keygen.mykey_set_product( key, code, len(code) )
    if err != 0:
        return jsonify({
            'status': 400,
            'message': keygen.get_error(err).decode()
        }), 400
    err =  keygen.mykey_set_dbcode( key, dbcode, len(dbcode) )
    if err != 0:
        return jsonify({
            'status': 400,
            'message': keygen.get_error(err).decode()
        }), 400
    err =  keygen.mykey_set_expiry( key, expiry, len(expiry) )
    if err != 0:
        return jsonify({
            'status': 400,
            'message': keygen.get_error(err).decode()
        }), 400
    err =  keygen.mykey_set_places( key, places, len(places) )
    if err != 0:
        return jsonify({
            'status': 400,
            'message': keygen.get_error(err).decode()
        }), 400
    if len(options) != max_options:
        for idx in range(0, max_options):
            if str(idx) in options:
                num = workplaces
            else:
                num = 0
            err =  keygen.mykey_set_option( key, idx, num )
            if err != 0:
                return jsonify({
                    'status': 400,
                    'message': keygen.get_error(err).decode()
                }), 400
    err = keygen.mykey_generate( key, buf, 1024 )
    if err != 0:
        return jsonify({
            'status': 400,
            'message': keygen.get_error(err).decode()
        }), 400

    keygen.mykey_free( key )
    
    return jsonify( {'item': buf.value.decode() } )

@app.errorhandler( ValueError )
def handle_exception(exc):
    response = {
        'error': 'Internal Server Error',
        'message': str(exc),
    }
    return jsonify( response ), 500

@app.errorhandler( HTTPException )
def handle_http_exception(exc):
    """ http errors """
    response = exc.get_response()
    response.content_type = "application/json"
    response.data = json.dumps({
        'error': exc.name,
        'message': exc.description,
    })
    return response

if __name__ == "__main__":
    from flask_cors import CORS
    CORS( app )
    app.run( debug=True, host="0.0.0.0", port=3001 );
