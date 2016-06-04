// Arduino Micro Oric Keyboard Reader.

#include <HID.h>
#include <Keyboard.h>

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

#define NUM_KEYS 64
#define FUNCT_KEY 47

// Array of characters
// Refer to https://www.arduino.cc/en/Reference/KeyboardModifiers for modifier keys

char keycodes[NUM_KEYS] = { '7', 'n', '5', 'v', '3', 'x', '1', ' ',	                      // R0
							'j', 't', 'r', 'f', 'd', 'q', KEY_ESC, ' ',	                  // R1
							'm', '6', 'b', '4', 'c', '2', 'z', KEY_LEFT_CTRL,             // R2
							'k', '9', ';', '-', '\'', '\\', ' ', ' ',                     // R3
							' ', ',', '.', KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_LEFT_SHIFT,  // R4
							'u', 'i', 'o', 'p', '[', ']', KEY_BACKSPACE, KEY_TAB,         // R5  KEY_TAB is FUNCT, handled seperately
							'y', 'h', 'g', 'e', 'w', 's', 'a', ' ',                       // R6
							'8', 'l', '0', '/', '=', ' ', KEY_RETURN, KEY_RIGHT_SHIFT };  // R7

bool keys[NUM_KEYS] = { 0 };       // Current state of keyboard matrix
bool last_keys[NUM_KEYS] = { 0 };  // Previous state of keyboard matrix, for edge detection

void setup()
{
	// Set pin directions
	pinMode(PIN_INPUT_COL1,  INPUT_PULLUP);
	pinMode(PIN_INPUT_COL2,  INPUT_PULLUP);
	pinMode(PIN_INPUT_COL3,  INPUT_PULLUP);
	pinMode(PIN_INPUT_COL4,  INPUT_PULLUP);
	pinMode(PIN_INPUT_COL5,  INPUT_PULLUP);
	pinMode(PIN_OUTPUT_C6,   OUTPUT);
	pinMode(PIN_OUTPUT_B7,   OUTPUT);
	pinMode(PIN_OUTPUT_A8,   OUTPUT);
	pinMode(PIN_INPUT_COL9,  INPUT_PULLUP);
	pinMode(PIN_INPUT_COL11, INPUT_PULLUP);
	pinMode(PIN_INPUT_COL12, INPUT_PULLUP);
	pinMode(PIN_OUTPUT_LED,  OUTPUT);

	// Set default output states
	digitalWrite(PIN_OUTPUT_C6, LOW);
	digitalWrite(PIN_OUTPUT_B7, LOW);
	digitalWrite(PIN_OUTPUT_A8, LOW);

	digitalWrite(PIN_OUTPUT_LED, LOW);

	Keyboard.begin();
	Keyboard.releaseAll();
}

void loop()
{
	// 1. Scan
	Scan();

  // 1a. FUNCT key is handled specially
  if (keys[FUNCT_KEY] == 0) 
  {
    HandleFUNCT();   
  }
  else
  {
    // 2. Edge detection for normal key handling
    for (int k = 0; k < NUM_KEYS; k++)
    {
      if (k == FUNCT_KEY) continue;  // Ignore, handled above

      if (KeyPressed(k))   // Falling edge
      {
        Keyboard.press(keycodes[k]);
      }

      if (KeyReleased(k))  // Rising edge
      {
        Keyboard.release(keycodes[k]);
      }
    }
  }
                                                                                              
  // 3. Store key state for next iteration
  for (int k = 0; k < NUM_KEYS; k++)
  {
    last_keys[k] = keys[k];
  }

	// 4. Debounce - a simple delay seems to suffice	
	delay(10); 
}

void Scan()
{
	for (int row = 0; row <= 7; row++)
	{
	  bool a = bitRead(row, 0);
	  bool b = bitRead(row, 1);
	  bool c = bitRead(row, 2);

	  digitalWrite(PIN_OUTPUT_A8, a);
	  digitalWrite(PIN_OUTPUT_B7, b);
	  digitalWrite(PIN_OUTPUT_C6, c);

	  int offset = (row * 8);

	  keys[offset + 0] = digitalRead(PIN_INPUT_COL1);
	  keys[offset + 1] = digitalRead(PIN_INPUT_COL2);
	  keys[offset + 2] = digitalRead(PIN_INPUT_COL3);
	  keys[offset + 3] = digitalRead(PIN_INPUT_COL4);
	  keys[offset + 4] = digitalRead(PIN_INPUT_COL5);
	  keys[offset + 5] = digitalRead(PIN_INPUT_COL9);
	  keys[offset + 6] = digitalRead(PIN_INPUT_COL11);
	  keys[offset + 7] = digitalRead(PIN_INPUT_COL12);
	}
}

bool KeyPressed(int k)
{
  return ((last_keys[k] == 1) && (keys[k] == 0));
}

bool KeyReleased(int k)
{
  return ((last_keys[k] == 0) && (keys[k] == 1));
}

// Special functions with FUNCT
void HandleFUNCT()
{
  // For testing, a panic key (FUNCT + H)
  if (KeyPressed(49))
  {
    Keyboard.releaseAll();
  }

  // A fun macro
  if (KeyPressed(13))  // Q
  {
    Keyboard.print("My hovercraft is full of eels\n");
  }

  // TODO options for switching keyboard layouts, etc.

}