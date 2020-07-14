#----------------------------------------------------------------------------
#
# Filename:  sbr_Protocol_v1.py
#
# Purpose:   Python test functions to encode and decode packets for the
#            Southbound Resource Protocol
#
# Copyright: (c) 2018 Sierra Wireless, Inc.
#            All rights reserved
#
#----------------------------------------------------------------------------
#
# VERSION: 0.0.1
#
# NOTES:
#

#
# Packet type field - byte 0
#
SBR_PKT_RQST_INPUT_CREATE   = 'I'   # type[1] d_type[1] pad[2] path[] units[]
SBR_PKT_RESP_INPUT_CREATE   = 'i'   # type[1] status[1] pad[2]

SBR_PKT_RQST_OUTPUT_CREATE  = 'O'   # type[1] d_type[1] pad[2] path[] units[]
SBR_PKT_RESP_OUTPUT_CREATE  = 'o'   # type[1] status[1] pad[2]

SBR_PKT_RQST_DELETE         = 'D'   # type[1] pad[1]    pad[2] path[]
SBR_PKT_RESP_DELETE         = 'd'   # type[1] status[1] pad[2]

SBR_PKT_RQST_HANDLER_ADD    = 'H'   # type[1] pad[1]    pad[2] path[] 
SBR_PKT_RESP_HANDLER_ADD    = 'h'   # type[1] status[1] pad[2]

SBR_PKT_RQST_HANDLER_REMOVE = 'K'   # type[1] pad[1]    pad[2] path[] 
SBR_PKT_RESP_HANDLER_REMOVE = 'k'   # type[1] status[1] pad[2]

SBR_PKT_RQST_PUSH           = 'P'   # type[1] d_type[1] pad[2] time[] path[] data[]
SBR_PKT_RESP_PUSH           = 'p'   # type[1] status[1] pad[2]

SBR_PKT_RQST_GET            = 'G'   # type[1] pad[1]    pad[2] path[]
SBR_PKT_RESP_GET            = 'g'   # type[1] status[1] pad[2] time[] data[]

SBR_PKT_RQST_EXAMPLE_SET    = 'E'   # type[1] d_type[1] pad[2] path[] data[]
SBR_PKT_RESP_EXAMPLE_SET    = 'e'   # type[1] status[1] pad[2]

SBR_PKT_RQST_SENSOR_CREATE  = 'S'   # type[1] d_type[1] pad[2] path[] units[]
SBR_PKT_RESP_SENSOR_CREATE  = 's'   # type[1] status[1] pad[2]

SBR_PKT_RQST_SENSOR_REMOVE  = 'R'   # type[1] pad[1]    pad[2] path[]
SBR_PKT_RESP_SENSOR_REMOVE  = 'r'   # type[1] status[1] pad[2]

SBR_PKT_NTFY_HANDLER_CALL   = 'c'   # type[1] d_type[1] pad[2] time[] path[] data[]
SBR_PKT_RESP_HANDLER_CALL   = 'C'   # type[1] status[1] pad[2]

SBR_PKT_NTFY_SENSOR_CALL    = 'b'   # type[1] pad[1]    pad[2] path[]
SBR_PKT_RESP_SENSOR_CALL    = 'B'   # type[1] status[1] pad[2]

SBR_PKT_RESP_UNKNOWN_RQST   = '?'   # type[1] status[1] pad[2]


#
# Data type field - byte 1
#
SBR_DATA_TYPE_TRIGGER       = 'T'   # trigger - no data
SBR_DATA_TYPE_BOOLEAN       = 'B'   # Boolean - 1 byte:  't' | 'f'
SBR_DATA_TYPE_NUMERIC       = 'N'   # numeric - null-terminated ASCII string, representing double
SBR_DATA_TYPE_STRING        = 'S'   # string  - null-terminated ASCII string
SBR_DATA_TYPE_JSON          = 'J'   # JSON    - null-terminated ASCII string, representing JSON
SBR_DATA_TYPE_UNDEF         = ' '   # not specified


#
# Variable length field identifiers
#
SBR_FIELD_ID_PATH           = 'P'
SBR_FIELD_ID_TIME           = 'T'
SBR_FIELD_ID_UNITS          = 'U'
SBR_FIELD_ID_DATA           = 'D'

# Variable length field separator
SBR_VARLENGTH_SEPARATOR     = ','



#
# Packet type descriptions
#
ptypes = [
    [ SBR_PKT_RQST_INPUT_CREATE,   'request create input'     ],
    [ SBR_PKT_RESP_INPUT_CREATE,   'response create input'    ],

    [ SBR_PKT_RQST_OUTPUT_CREATE,  'request create output'    ],
    [ SBR_PKT_RESP_OUTPUT_CREATE,  'response create output'   ],

    [ SBR_PKT_RQST_DELETE,         'request delete resource'  ],
    [ SBR_PKT_RESP_DELETE,         'response delete resource' ],

    [ SBR_PKT_RQST_HANDLER_ADD,    'request add handler'      ],
    [ SBR_PKT_RESP_HANDLER_ADD,    'response add handler'     ],

    [ SBR_PKT_RQST_HANDLER_REMOVE, 'request remove handler'   ],
    [ SBR_PKT_RESP_HANDLER_REMOVE, 'response remove handler'  ],

    [ SBR_PKT_RQST_PUSH,           'request push'             ],
    [ SBR_PKT_RESP_PUSH,           'response push'            ],

    [ SBR_PKT_RQST_GET,            'request get'              ],
    [ SBR_PKT_RESP_GET,            'response get'             ],

    [ SBR_PKT_RQST_EXAMPLE_SET,    'request set example'      ],
    [ SBR_PKT_RESP_EXAMPLE_SET,    'response set example'     ],

    [ SBR_PKT_RQST_SENSOR_CREATE,  'request create sensor'    ],
    [ SBR_PKT_RESP_SENSOR_CREATE,  'response create sensor'   ],

    [ SBR_PKT_RQST_SENSOR_REMOVE,  'request remove sensor'    ],
    [ SBR_PKT_RESP_SENSOR_REMOVE,  'response remove sensor'   ],

    [ SBR_PKT_NTFY_HANDLER_CALL,   'handler call'             ],
    [ SBR_PKT_RESP_HANDLER_CALL,   'handler ack'              ],

    [ SBR_PKT_NTFY_SENSOR_CALL,    'sensor poll'              ],
    [ SBR_PKT_RESP_SENSOR_CALL,    'sensor poll ack'          ],

    [ SBR_PKT_RESP_UNKNOWN_RQST,   'unknown packet type'      ],
]


#
# Status field
#
status_list = [
    'OK',
    'NOT FOUND',
    'NOT POSSIBLE',   # deprecated
    'OUT OF RANGE',
    'NO MEMORY',
    'NOT PERMITTED',
    'FAULT',
    'COMM ERROR',
    'TIMEOUT',
    'OVERFLOW',
    'UNDERFLOW',
    'WOULD BLOCK',
    'DEADLOCK',
    'FORMAT ERROR',
    'DUPLICATE',
    'BAD PARAMETER',
    'CLOSED',
    'BUSY',
    'UNSUPPORTED',
    'IO_ERROR',
    'NOT IMPLEMENTED',
    'UNAVAILABLE',
    'TERMINATED'
]


#
# Data types
#
data_types = [
    [ 'trig',   SBR_DATA_TYPE_TRIGGER ],
    [ 'bool',   SBR_DATA_TYPE_BOOLEAN ],
    [ 'num',    SBR_DATA_TYPE_NUMERIC ],
    [ 'str',    SBR_DATA_TYPE_STRING  ],
    [ 'json',   SBR_DATA_TYPE_JSON    ]
]


#
# Syntax
#
syntax_list = [
    '  create input|output|sensor  trig|bool|num|str|json <path> [<units>]',
    '  delete resource|handler|sensor <path>',
    '  add handler <path>',
    '  push trig|bool|num|str|json <path> [<data>]',
    '  get <path>',
    '  example json <path> [<data>]'
]


#
# Usage
#
def print_usage():

    print 'Usage:'
    for i in range(len(syntax_list)):
        print syntax_list[i]
    print


#
# Encode data type
#
def encode_dtype(data_type):

    field = ''
    dtype = data_type.lower()

    if   dtype[0] == 't':
        field = field + SBR_DATA_TYPE_TRIGGER

    elif dtype[0] == 'b':
        field = field + SBR_DATA_TYPE_BOOLEAN

    elif dtype[0] == 'n':
        field = field + SBR_DATA_TYPE_NUMERIC

    elif dtype[0] == 's':
        field = field + SBR_DATA_TYPE_STRING

    elif dtype[0] == 'j':
        field = field + SBR_DATA_TYPE_JSON

    else:
        print 'Invalid data type'
        return

    return field

#
# Encode segment number and segment count
#
def encode_segment():

    return '01'


#
# Encode path
#
def encode_path(path):

    return SBR_FIELD_ID_PATH + path


#
# Encode units
#
def encode_units(units):

    return SBR_FIELD_ID_UNITS + units


#
# Encode data
#
def encode_data(data):

    return SBR_FIELD_ID_DATA + data


#
# create input|output|sensor data-type path [units]
#
def encode_create(argc, args):

    packet = ''
    dtype = ''
    syntax = syntax_list[0]


    if argc < 3 :
        print 'Invalid number of arguments'
        print syntax_list[0]
        return

    if argc > 3 :
        what,data_type,path,units = args.split(' ')

    else:
        what,data_type,path = args.split(' ')
        units = None

    what = what.lower()

    if what[0] == 'i':
        packet = packet + SBR_PKT_RQST_INPUT_CREATE
    elif what[0] == 'o':
        packet = packet + SBR_PKT_RQST_OUTPUT_CREATE
    elif what[0] == 's':
        packet = packet + SBR_PKT_RQST_SENSOR_CREATE
    else:
        print 'Invalid request'
        print syntax_list[0]
        return

    dtype = encode_dtype(data_type)
    if dtype == '': 
        return
    packet = packet + dtype

    packet = packet + encode_segment()
    packet = packet + encode_path(path)
    if units != None:
        packet = packet + SBR_VARLENGTH_SEPARATOR
        packet = packet + encode_units(units)

    return packet


#
# delete resource|handler|sensor path
#
def encode_delete(argc, args):

    packet = ''

    if argc < 2 :
        print 'Invalid number of arguments'
        print syntax_list[1]
        return

    what,path = args.split(' ')
    what = what.lower()

    # packet type
    if what[0] == 'r':
        packet = packet + SBR_PKT_RQST_DELETE
    elif what[0] == 'h':
        packet = packet + SBR_PKT_RQST_HANDLER_REMOVE
    elif what[0] == 's':
        packet = packet + SBR_PKT_RQST_SENSOR_REMOVE
    else:
        print 'Invalid request'
        print syntax_list[1]
        return

    packet = packet + '.'
    packet = packet + encode_segment()
    packet = packet + encode_path(path)

    return packet


#
# add handler path
#
def encode_add(argc, args):

    packet = ''


    if argc < 2 :
        print 'Invalid number of arguments'
        print syntax_list[2]
        return

    what,path = args.split(' ')
    what = what.lower()

    if what[0] != 'h':
        print 'Invalid request ' + what
        print syntax_list[2]
        return

    packet = packet + SBR_PKT_RQST_HANDLER_ADD
    # data type - ignored
    packet = packet + '.'
    packet = packet + encode_segment()
    packet = packet + encode_path(path)

    return packet


#
# push data-type path [data]
#
def encode_push(argc, args):

    packet = ''
    dtype = ''

    if argc < 2 :
        print 'Invalid number of arguments'
        print syntax_list[3]
        return

    print 'argc: ' + str(argc)
    if argc > 2 :
        data_type,path,data = args.split(' ', 2)

    else:
        data_type,path = args.split(' ')
        data = ''

    packet = packet + SBR_PKT_RQST_PUSH

    dtype = encode_dtype(data_type)
    if dtype == '': 
        return
    packet = packet + dtype

    packet = packet + encode_segment()
    packet = packet + encode_path(path)
    if data != '':
        packet = packet + SBR_VARLENGTH_SEPARATOR
        packet = packet + encode_data(data)

    return packet


#
# get path
#
def encode_get(argc, args):

    packet = ''
    dtype = ''

    if argc < 1 :
        print 'Invalid number of arguments'
        print syntax_list[4]
        return

    packet = packet + SBR_PKT_RQST_GET
    # data type ignored
    packet = packet + '.'
    packet = packet + encode_segment()
    packet = packet + encode_path(args)

    return packet



#
# example data-type path [data]
#
def encode_example(argc, args):

    packet = ''
    dtype = ''

    if argc < 2 :
        print 'Invalid number of arguments'
        print syntax_list[5]
        return

    if argc > 2 :
        data_type,path,data = args.split(' ', 2)

    else:
        data_type,path = args.split(' ')
        data = ''

    packet = packet + SBR_PKT_RQST_EXAMPLE_SET

    dtype = encode_dtype(data_type)
    if dtype == '':
        return
    packet = packet + dtype

    packet = packet + encode_segment()
    packet = packet + encode_path(path)
    if data != '':
        packet = packet + SBR_VARLENGTH_SEPARATOR
        packet = packet + encode_data(data)

    return packet


#
# Parse command and build a request packet
#
def encode_request(request):

    if request.find(' ') < 0 :
        print_usage()
        return

    argc = len(request.split(' ')) - 1

    # all commands take at least one argument
    if argc < 1 :
        print_usage()
        return

    request_type,args = request.split(' ', 1)
    request_type = request_type.lower()

    if request_type[0] == 'c':
        p = encode_create(argc, args)

    elif request_type[0] == 'd':
        p = encode_delete(argc, args)

    elif request_type[0] == 'a':
        p = encode_add(argc, args)

    elif request_type[0] == 'p':
        p = encode_push(argc, args)

    elif request_type[0] == 'g':
        p = encode_get(argc, args)

    elif request_type[0] == 'e':
        p = encode_example(argc, args)

    elif request_type[0] == 'r':
        p = args

    else:
        print_usage()
        return

    return p


#
# Decode and print contents of an incoming packet
#
def decode_response(response):

    resp = {}

    # Positional fields:
    ptype      = response[0]
    status     = response[1]
    seg_number = response[2]
    seg_count  = response[3]

    # Labeled, variable length fields
    var_length = response[4:]

    for i in range(len(ptypes)):
        test = ptypes[i]
        if test[0] == ptype:
            resp['responseType'] = test[0]

    # Status is represented in ASCII, starting with '@' (0x40) for OK.
    # Subtract 0x40 to index into the table, above
    #
    i = ord(status[0]) - ord('\x40')
    resp['status'] = status_list[i]

    if len(var_length):
        var_fields = var_length.split(',')
        for i in range(len(var_fields)):
            field = var_fields[i]
            if field[0] == 'P':
                resp['path'] = field[1:]
            if field[0] == 'T':
                resp['timestamp'] = field[1:]
            if field[0] == 'D':
                resp['data'] = field[1:]
    return resp