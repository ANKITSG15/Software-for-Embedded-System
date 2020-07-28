# Software-for-Embedded-System
This course introduces the issues and challenges in developing software for embedded system.

For Assignment-1 : HAL Implementation(Without using library)

    RCC->AHB1ENR = 0x000001FF;(Choose according to the GPIO ports(A-I) you want to enable the bus here all 9 bits are one)
		
    1. Open the DTH22 folder and choose open project and select DHT11(uVision5 Project) in Keil.
       
    2. dht.c file method- int main(void)
    
		//Start :- Q-1. Select the pin number and port number you want to configure.
    
		ght.moder_pin = 15;   		// Pin you want to configure example- 7(0111) means pin 0,1,2 to configure. 8(1000) means pin 3 to configure
		ght.moder_pin_type='A';   // Moder Options     : O - 01 : Output pin, I - 00 : Input pin, A - 11 : Analog mode, F - 10: Function mode
	  ght.otype_pin_type='O';	  // Output Type       : P - 0 : Push-Pull, O - 1 : Output open-drain
		ght.ospeed_pin_type='H';  // Output Speed      : L - 00 : Low-speed, M - 01 : Medium speed, H - 10 : High Speed, V - 11 : Very High Speed 
		ght.pupd_pin_type='D';    // Pull-up/Pull-down : N - 00 : No pull-up, pull-down, U - 01 : Pull-up, D - 10 : Pull-down, R - 11 : Reserved
		ght.afr_type=15;					// Activation Funct  : function from f0-f15 : [0-15]
	  ght.lck_pin_type='N';     // Locking Register  : L - 1 : Locking, N - 0 : Non Locking
    
    3. If you have hardware, test the pins configuration by blinking of LEDs.
    
    4. In case of no hardware, run the code in debug mode and open the watch window and check the value of variable and register accordingly.
