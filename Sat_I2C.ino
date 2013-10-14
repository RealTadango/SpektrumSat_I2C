#include <Wire.h>

//Timer offset between frames
long t;
//Frame data
byte data[14];
//Frame byte Index, default out of range to wait for header
int readIndex = 99;

//Ready indicator
bool ready = false;

void setup()
{
	//RX Sat at 115.2K baud
	Serial.begin(115200);
	//Setup wire to listen to id 2
    Wire.begin(2);
    Wire.onRequest(requestEvent);
	//Wait for first frame
	t = millis();
}

void loop()
{
	//Wait for tx data pair to be available
	while(Serial.available())
	{
		long t2 = millis();

		byte val = Serial.read();

		//If more then 10 mSec since last byte it is the new frame
		if(t2 - t > 10)
		{
			//reset index
			readIndex = 0;
		}
		
		if(readIndex >= 2 && readIndex < 16) //safety check to avoid array overrun
		{
			//store value
			data[readIndex - 2] = val;

			if(readIndex == 15)
			{
				ready = true;
			}
		}

		//Sync timer
		t = t2;
		//next index
		readIndex++;
	}
}

void requestEvent()
{
	if(ready)
	{
		//Send all data
		Wire.write(data, 14);
	}
}