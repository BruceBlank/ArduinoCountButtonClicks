#include <Arduino.h>
#include "CountingButtons.h"

// The main class. One instance will be created in the program
class CCountButtonClicks {
private:
	CCountingButtons &m_CountButton;
	CCountingButtons &m_ResetButton;

	// clear reset and count button after user clicked reset button
	void resetButtons();

public:
	// the constructor to initialize things
	CCountButtonClicks();
	// do things
	void doit();
};

void CCountButtonClicks::resetButtons()
{
	// configure buttons: button0 is for counting, button1 is for reset
	// reset button can be clicked a lot of times to reset
	// will be cleared again, after the user clicked it
	m_ResetButton.configButton(0, 255);
	// reset the count button, so it will count from 0 again
	m_CountButton.configButton(0, 15); // counts from 0 to 15 (4 bit)
}

CCountButtonClicks::CCountButtonClicks() :
		m_CountButton(CCountingButtons::instance0()), m_ResetButton(CCountingButtons::instance1())
{
	// configure buttons
	resetButtons();
	// configure button pins
	pinMode(2 , INPUT); // interrupt pin 0 -> Button 0
	pinMode(3 , INPUT); // interrupt pin 1 -> Button 1
	// configure LED pins
	pinMode(8 , OUTPUT);
	pinMode(9 , OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
}

void CCountButtonClicks::doit()
{
	// reset button clicked at least one time
	if(m_ResetButton.getValue() > 0){
		resetButtons();
	}
	// read value, convert to binary form and show with LEDs (pin 8 is highest value)
	int value = m_CountButton.getValue();
	for(int i=3; i>=0; --i){
		digitalWrite(11-i, (((value >> i) & 1 == 0) ? LOW : HIGH));
	}
}

int main(void)
{
	// general initialization
	init();

	// create instance
	CCountButtonClicks app;

	while (true)
	{
		app.doit();
		// this delay is for testing: instead, a longer calculation could take place here
		delay(200);
	}
	return 0; // never reached
}
