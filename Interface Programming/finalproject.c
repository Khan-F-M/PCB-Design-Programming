//Program: labproject_p5
//Author: Khan, Muhammad
//Date: 04-11-2021
//Purpose: The purpose of this program is for the Optical Sensor to allow a specific amount of voltage to the buzzer and LED when it is interrupted.
//VS Version: Community 2019
#include <stdio.h> //These 4 are the libarys used in this code
#include <stdlib.h>
#include <Windows.h>
#include "C:\program files (x86)\labjack\drivers\LabJackUD.h" 

int main()
{
	// Section 1, This is where I initailized my variables for the labjack and program.
	LJ_ERROR lj_cue;
	LJ_HANDLE lj_handle = 0;
	double dblValueDAC1 = 0, dblValueAIN2 = 0, dblValueFIO4 = 0;
	int dutycycle = 0, counter  = 0;

	// Section 2, This is where I setup the handles and reset all the ports on the Labjack
	lj_cue = OpenLabJack(LJ_dtU3, LJ_ctUSB, "1", 1, &lj_handle);
	lj_cue = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);

	// Section 3, This is where I enabled a timer to use FIO6
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_COUNTER_PIN_OFFSET, 6, 0, 0);

	// Section 4, This is where I set up the timer clock base of 12,000,000 Hz per divisor
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_BASE, LJ_tc12MHZ_DIV, 0, 0);

	// Section 5, This is where I set up the timer clock divisor of 48 
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_DIVISOR, 48, 0, 0);

	// Section 6, This is where I turned on the timer
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chNUMBER_TIMERS_ENABLED, 1, 0, 0);

	// Section 7, This is where I set the timer mode to 8-bit
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_MODE, 0, LJ_tmPWM8, 0, 0);

	// Section 8, This is where I called the Go function to set up all requests before the loop begins
	lj_cue = Go();

	// I picked method 2 from my project, and set the counter so that it would run for 15 seconds
	while (counter < 60)
	{
			// Section 9, This is where I set the AIN input to 0V, and dsiplayed it on the terminal
			lj_cue = AddRequest(lj_handle, LJ_ioGET_AIN, 2, 0, 0, 0);
			lj_cue = Go();
			lj_cue = GetResult(lj_handle, LJ_ioGET_AIN, 2, &dblValueAIN2);
			printf("AIN2 = %.2lf\n", dblValueAIN2);

			// Section 10, This is where I set the dutycycle so that everytime the sensor is interrupted, the dutycycle value would increase to 50%
			if (dblValueAIN2 < 1)
			{
				dutycycle = 0;
			}
			else
			{
				dutycycle = 32768;
			}

			// Section 11, This is where I set the dutycycle value for the buzzer to turn on/off
			lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, dutycycle, 0, 0);
			lj_cue = Go();


			if (dblValueAIN2 < 1)
			{
				//Section 12, This is where I set the FIO7 value to 0 since the sensor was not interrupted
				lj_cue = AddRequest(lj_handle, LJ_ioPUT_DIGITAL_BIT, 4, 0, 0, 0);
				lj_cue = Go();
				lj_cue = GetResult(lj_handle, LJ_ioPUT_DIGITAL_BIT, 4, &dblValueFIO4);
				printf("FIO7 = %.2lf\n\n", dblValueFIO4);
			}
			else
			{
				//Section 13, Since the sensor was interrupted, the value of FIO7 was put to high and the LED turned on
				lj_cue = AddRequest(lj_handle, LJ_ioPUT_DIGITAL_BIT, 4, 4.5, 0, 0);
				lj_cue = Go();
				lj_cue = GetResult(lj_handle, LJ_ioPUT_DIGITAL_BIT, 4, &dblValueFIO4);
				printf("FIO7 = %.2lf\n\n", dblValueFIO4);

			}

			//In the project file, it was instructed I read and display the state of the sensor every 0.25s so I set the sleep function to 250ms
			counter++;
			Sleep(250);
	}


	// Section 14, This is where I paused the system to wait for a user to repond and exit the program
	system("pause");

	// Section 15
	lj_cue = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	lj_cue = eGet(lj_handle, LJ_ioGET_DIGITAL_BIT, 4, &dblValueFIO4, 0);

	// Section 16
	Close();
	return 0;
}
