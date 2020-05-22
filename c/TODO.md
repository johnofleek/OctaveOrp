Change this
```
    orp_protocol(
        orpProtocol_txBuffer,       // encoder input payload 
        ORP_PROTOCOL_TX_BUFFER_LENGTH,       // encoder input payload size

        serial_txByte,         		// encoder - bind TX a byte to UART                                      

        hdlc_rxBuffer,            	// decoder - working buffer
        HDLC_RX_BUFFER_LENGTH,      // decoder - length of working buffer

        crc_tabccitt, 				// size is fixed by crc calculator
        CCITT_TABLE_SIZE,			

        app_requestResponse_cbh,	// decoder request responses from Octave 
        app_notification_cbh		// decoder notifications from Octave
    );
```
So that an array access is passed and not the array  
This is to enable FLASH based storage to be used

Also needs a way precalculated version of
```
static void init_crcccitt_tab( void );
```

So that the FLASH or RAM can be initialised with the correct values without run time calculation  