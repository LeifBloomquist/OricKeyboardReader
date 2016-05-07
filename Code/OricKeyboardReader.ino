
// Arduino Nano pinout.  Port to Arduino Micro for USB HID support, TODO.

#define PIN_INPUT_COL1   2  
#define PIN_INPUT_COL2   3  
#define PIN_INPUT_COL3   4  
#define PIN_INPUT_COL4   5  
#define PIN_INPUT_COL5   6  
#define PIN_OUTPUT_C6    7
#define PIN_OUTPUT_B7    8
#define PIN_OUTPUT_A8    9
#define PIN_INPUT_COL9   10  
#define PIN_INPUT_COL11  11  
#define PIN_INPUT_COL12  12
#define PIN_OUTPUT_LED   13

// Connector pin 10 to GND
// Connector pin 13 to GND
// Connector pin 14 to 5V

byte matrix[8] = { 0 };

void setup()
{
	Serial.begin(115200);

	// Set pin directions
	pinMode(PIN_INPUT_COL1,   INPUT_PULLUP);
	pinMode(PIN_INPUT_COL2,   INPUT_PULLUP);
	pinMode(PIN_INPUT_COL3,   INPUT_PULLUP);
	pinMode(PIN_INPUT_COL4,   INPUT_PULLUP);
	pinMode(PIN_INPUT_COL5,   INPUT_PULLUP);
	pinMode(PIN_OUTPUT_C6,    OUTPUT);
	pinMode(PIN_OUTPUT_B7,    OUTPUT);
	pinMode(PIN_OUTPUT_A8,    OUTPUT);
	pinMode(PIN_INPUT_COL9,   INPUT_PULLUP);
	pinMode(PIN_INPUT_COL11,  INPUT_PULLUP);
	pinMode(PIN_INPUT_COL12,  INPUT_PULLUP);
	pinMode(PIN_OUTPUT_LED,   OUTPUT);

	// Set default output states
	digitalWrite(PIN_OUTPUT_C6, LOW);
	digitalWrite(PIN_OUTPUT_B7, LOW);
	digitalWrite(PIN_OUTPUT_A8, LOW);

	digitalWrite(PIN_OUTPUT_LED,   LOW);
}

void loop()
{	
	for (int row = 0; row <= 7; row++)
	{
		matrix[row]=scan(row);
		Serial.print(row);
		Serial.print(":0x");
		Serial.print(matrix[row], HEX);
		Serial.print("\t");
	}
	Serial.println();
	
	delay(10); 
}

byte scan(int row)
{
	bool a = bitRead(row, 0);
	bool b = bitRead(row, 1);
	bool c = bitRead(row, 2);

	digitalWrite(PIN_OUTPUT_A8, a);
	digitalWrite(PIN_OUTPUT_B7, b);
	digitalWrite(PIN_OUTPUT_C6, c);

	bool c0 = digitalRead(PIN_INPUT_COL1);
	bool c1 = digitalRead(PIN_INPUT_COL2);
	bool c2 = digitalRead(PIN_INPUT_COL3);
	bool c3 = digitalRead(PIN_INPUT_COL4);
	bool c4 = digitalRead(PIN_INPUT_COL5);
	bool c5 = digitalRead(PIN_INPUT_COL9);
	bool c6 = digitalRead(PIN_INPUT_COL11);
	bool c7 = digitalRead(PIN_INPUT_COL12);

	byte val =
		(c0 << 0) |
		(c1 << 1) |
		(c2 << 2) |
		(c3 << 3) |
		(c4 << 4) |
		(c5 << 5) |
		(c6 << 6) |
		(c7 << 7);

	return val;
}