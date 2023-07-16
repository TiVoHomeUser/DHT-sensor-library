#include "Arduino.h"

#include "DHT.h"


/*
 * 				looking for sensors on all useable pins
 */
DHT Sensors[]
			{
				//DHT(D0, DHTAUTO),
				DHT(D1, DHTAUTO), DHT(D2, DHTAUTO), DHT(D3, DHTAUTO),
				DHT(D4, DHTAUTO), DHT(D5, DHTAUTO), DHT(D6, DHTAUTO), DHT(D7, DHTAUTO)
				//,DHT(D8, DHTAUTO)
			};
uint8_t number_of_sensors = 0;


void setup()
{
	delay(200);				// Some chips my need longer to stabilize
	Serial.begin(74880); 	// 74880 seems to be the boot default for the chips I have

	/* Setup Temperature sensor(s) */
	number_of_sensors = sizeof(Sensors) / sizeof(DHT);
	Serial.printf("Number of Sensors = %u", number_of_sensors);
	for(int i = 0; i < number_of_sensors; i++)
	{
		Sensors[i].begin();
	}
}

void loop()
{
	  if ( timeElapsed())     // Do every 5 seconds
	  {
		for(int i = 0; i < number_of_sensors; i++)
		{
			Serial.printf("Sensor %s %7s\ttemperature = %#5.2fF\tHumidity = %#5.2f%%\n",
					pinName(Sensors[i].pin()),
					sensor_type(&Sensors[i]),
					Sensors[i].readTemperature(false) ,Sensors[i].readHumidity(false));
			yield();
		}
		Serial.println("");
	  }
}

/*
 *
 * 		Sensor type as a c-string for display
 * 		attempts to re-initialize never connected pins
 *
 */
const char* sensor_type(DHT* s)
{
	static String retval;
	retval.clear();
	switch(s->dht_Type())
	{
	case DHT11:
		retval = "DHT11";
		break;
	case DHT12:
		retval = "DHT12";
		break;
	case DHT21:
		retval = "DHT21";
		break;
	case DHT22:
		retval = "DHT22";
		break;
	//case AM2301:				// same as DHT21
	//	retval = "AM2301";
	//	break;
	case DHTAUTO:
		retval = "DHTAUTO";
		break;
	case DHTERR:
		retval = "DHTERR";
		s->begin();				// Attempt recovery
		break;
	default:
		retval = "UnKwn";
	}

	return retval.c_str();
}

/*
 *
 *     			instead of using delay() use the timer for loop
 *
 *
 */
#define secondInterval 1000
static const unsigned long timeInterval = secondInterval * 5; // make it 5 seconds interval

static unsigned long currentmills = 0;
static unsigned long lastmills = 0;
static int m_overflow=0;							// Counter for the 49 day millis() overflow bug

boolean timeElapsed()
{
  currentmills = millis();
  if ((currentmills - lastmills) >= timeInterval) {
    lastmills = currentmills;
    return true;
  }
  return false;
}

/*
 *
 *			Convert the connected pin back to the macro label
 *
 */
const char* pinName(uint8_t pin)
{
	static String retval;
	retval.clear();
	switch(pin){
	case D0:	retval = "D0";
	break;
	case D1:	retval = "D1";
	break;
	case D2:	retval = "D2";
	break;
	case D3:	retval = "D3";
	break;
	case D4:	retval = "D4";
	break;
	case D5:	retval = "D5";
	break;
	case D6:	retval = "D6";
	break;
	case D7:	retval = "D7";
	break;
	case D8:	retval = "D8";
	break;
	default: retval = "??";
	}
	return retval.c_str();
}
