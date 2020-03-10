# Create
remote/val/value

('Creating', 'waterLevelHigh')
2020-02-11 13:14:43,684 OctaveRP: creating input at val
2020-02-11 13:14:43,887 OctaveRP: sending frame, awaiting i
('input ', 'IB01Pval/value,Unum')
('crcValue ', '0x3378')
('input ', 'i@01')
('crcValue ', '0x102f')
2020-02-11 13:14:43,960 OctaveRP: recv frame: {'status': 'OK', 'responseType': 'i'}
2020-02-11 13:14:44,405 OctaveRP: checking message {'status': 'OK', 'responseType': 'i'}

Response
<packet type[1]><status[1]><segment[2]><contents[variable]>

i@01

# send bool
PB01Pval/value,DTrue

Response
p@01

// input to octave from remote
// createResource(packetType 'I' || 'O', dataType =  'T' || 'B' || 'N' || 'S' || 'J' <path>[,T<timestamp>][,D<data>]
// example
// 'IB01Pval/value,Unum'
int16_t createResource(char packetType, char dataType, char *path, char * units )
{
    int16_t retval = -1;
    retval = snprintf(payload,sizeof(payload),
            "%C%C01P%S/value,U%s",
            packetType,
            dataType,
            path,
            units 
        );
    
    hdlc.frameDecode(data, strlen(payload));
    
    /* Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
}

/*
 <P><'T' || 'B' || 'N' || 'S' || 'J' ><01><Ppath>,<Ttimestamp><Ddata>
 e.g. PB01Pval/value,DTrue
 <packet type[1]><data type[1]><segment[2]><path>[,<timestamp>][,<data>]
*/
void pushValueToOctave(uint8_t dataType, char *path, char * data )
{

}
 
 
 # captured data 
 /*
 * ~ 7E, 
 * I , B , 0 , 1 , P , v , a , l , / , v , a , l , u , e , 
 * , 2C, 
 * U 55, 
 * b 62, 
 * o 6F, 
 * o 6F, 
 * l 6C, 
 * } 7D, 
 * ^ 5E, 
 * ⸮ E8, 
 * ~ 7E,



~ 7E,  [0] ~ 7E,  [0] ~ 7E,  [0] ~ 7E,  
[0] I 49,  
[1] B 42,  
[2] 0 30,  
[3] 1 31,  
[4] P 50,  
[5] v 76,  
[6] a 61,  
[7] l 6C,  
[8] / 2F,  
[9] v 76,  
[10] a 61,  
[11] l 6C,  
[12] u 75,  
[13] e 65,  
[14] , 2C,  
[15] U 55,  
[16] n 6E,  
[17] u 75,  
[18] m 6D,  
[19] 3 33,  
[20] x 78,  
[21] ~ 7E, 
*/

## Arduino TX

                                                                        
7E 49 42 30 31 50 76 61 6C 2F 76 61 6C 75 65 2C 55 6E 75 6D 41 C6 7E    
                                                                    

## Python OP




init128
IN01Pval/te1/value,Unum
frame_length23
CRC
49 6DCRE
50657
done
128
PB01Pval/tel/value,D25.2
frame_length24
CRC
50 32CRE
60263
128
PB01Pval/tel/value,D25.2


// working data
2020-02-12 20:17:39,381 OctaveRP: creating input at val/tp
2020-02-12 20:17:39,585 OctaveRP: sending frame, awaiting i
('input ', 'IN01Pval/tp/value,Unum')
('crcValue ', '0x9f09')
('input ', 'i@01')
('crcValue ', '0x102f')
2020-02-12 20:17:39,658 OctaveRP: recv frame: {'status': 'OK', 'responseType': 'i'}
2020-02-12 20:17:40,105 OctaveRP: checking message {'status': 'OK', 'responseType': 'i'}
('Creating', 'waterLevelHigh')
2020-02-12 20:17:40,111 OctaveRP: creating input at val/wl
2020-02-12 20:17:40,313 OctaveRP: sending frame, awaiting i
('input ', 'IB01Pval/wl/value,Ubool')
('crcValue ', '0x178c')
('input ', 'i@01')
('crcValue ', '0x102f')
2020-02-12 20:17:40,377 OctaveRP: recv frame: {'status': 'OK', 'responseType': 'i'}
2020-02-12 20:17:40,825 OctaveRP: checking message {'status': 'OK', 'responseType': 'i'}
2020-02-12 20:17:40,826 OctaveRP: preparing to send: push bool val/wl/value True
argc: 3
('input ', 'PB01Pval/wl/value,DTrue')
('crcValue ', '0x8312')
2020-02-12 20:17:41,039 OctaveRP: preparing to send: push num val/tp/value 23.7
argc: 3
('input ', 'p@01')
('crcValue ', '0xf8ff')
2020-02-12 20:17:41,111 OctaveRP: recv frame: {'status': 'OK', 'responseType': 'p'}
('input ', 'PN01Pval/tp/value,D23.7')
('crcValue ', '0x1334')
('input ', 'p@01')
('crcValue ', '0xf8ff')
2020-02-12 20:17:41,321 OctaveRP: recv frame: {'status': 'OK', 'responseType': 'p'}


# Wakeup

~~
~~
~~
~~
~~
~~
~SJ01Psensors/set1ýN~~SJ01Psensors/set1ýN~~SJ01Psensors/set1ýN~
~SJ01Psensors/set1ýN~~SJ01Psensors/set1ýN~~SJ01Psensors/set1ýN~

