#include <SoftwareSerial.h>
#define LEDPIN 13

SoftwareSerial hidSerial(3, 4);


const byte uid_cards[] = { 0xFB, 0x19, 0xC2, 0x1D, 0xC4, };
const int Waiting = 3000;
const byte OpenCommand[7] = {0xAA, 0x02, 0x00, 0x00, 0x00, 0xA8, 0x03, };
const byte StartCommand[9] = {0xAA, 0x02, 0x10, 0x00, 0x02, 0x01, 0x01, 0x71, 0xAD, };
const byte StopCommand[7] = {0xAA, 0x02, 0xFF, 0x00, 0x00, 0xA4, 0x0F, };

byte buffer[100];
uint8_t incomingByte = 0;


void clearBufferArray()
{
    for (int i = 0; i < sizeof(buffer); i++) buffer[i] = NULL;
    incomingByte = 0;
}


int CheckCard()
{
    for (int i = 0; i < sizeof(buffer); i++)
    {
        for( int card = 0; card < sizeof(uid_cards); card++)
        {
            if( buffer[i + 0] == 0x00 && buffer[i + 11] == uid_cards[card] )
            {
                return 1;   
            }
        }    
    }
    
    return 0;
}

void setup()
{
    Serial.begin(115200);

    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
    
    hidSerial.begin(115200);
    hidSerial.write(OpenCommand, 7);
    hidSerial.write(StartCommand, 9);
}

void loop() 
{
    while(1)
    {
        if (hidSerial.available() > 0) 
        {       
            delay(200);
            
            while(hidSerial.available() > 0)
            {
                buffer[incomingByte++] = hidSerial.read();
                delay(5);
                if(incomingByte == sizeof(buffer)) break;
            }
               
            Serial.write(buffer, sizeof(buffer));
  
            if(CheckCard() > 0)
            {
                digitalWrite(LEDPIN, HIGH);
                delay(Waiting);
                digitalWrite(LEDPIN, LOW);
            }
  
            clearBufferArray();
        }
        
        hidSerial.flush();      
    }
}
