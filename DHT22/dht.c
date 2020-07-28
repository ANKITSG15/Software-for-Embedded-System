#include "stm32f4xx.h"
#include "stm32f407xx.h"

/*
Team-11 
Ankit Singh Gautam
Surbhi Gupta 
*/



typedef struct {

	uint32_t moder;
	uint16_t moder_pin;
	char moder_pin_type;	 
	uint32_t otype;
	char otype_pin_type;  
	uint32_t ospeed;
	char ospeed_pin_type;  
	uint32_t pupd;
	char pupd_pin_type; 
	uint16_t afr_type; 
	char lck_pin_type;
  uint32_t prev_odr;
	
}GPIO_InitdataDef;  

GPIO_InitdataDef GPIO_RR ; //Structure for reading the register values

/*
Q-5. For Toggling the GPIO port
*/
void GPIO_TOGGLE(GPIO_TypeDef *GPIOx)
{
	
	GPIOx->ODR ^= 0xFFFF;
}

/*
Q-4. Setting/Resetting individual bits of mentioned port using BSSR
*/
void GPIO_SET_BSSR(GPIO_TypeDef *GPIOx, char SETR, uint16_t pin)
{
   if(SETR == 'R')
	 {
	   GPIOx->BSRR |= 1U << (16+pin) ;
		 	 
	 }else
	 if(SETR == 'S')
	 {
		 GPIOx->BSRR |= 1U << pin;
	 }
	
}

/*
Q-3. Writing the GPIO data register (ODR) of any port
*/
void GPIO_Write_ODR(GPIO_TypeDef  *GPIOx, uint16_t data)
{
	
		GPIO_RR.prev_odr=	GPIOx->ODR;  // Reading the previous value of ODR register
		GPIOx->ODR = data;
 
}

/*
Q-2. Reading the GPIO configuration registers of the Port
*/
GPIO_InitdataDef * GPIO_Read(GPIO_TypeDef  *GPIOx)
{


	GPIO_RR.moder  = GPIOx->MODER;
	GPIO_RR.otype = GPIOx->OTYPER;
	GPIO_RR.pupd = GPIOx->PUPDR;
	GPIO_RR.ospeed = GPIOx->OSPEEDR;
	
	return &GPIO_RR;
}



/*
Q-1. For initializing the GPIO port
*/
void GPIO_Init1(GPIO_TypeDef  *GPIOx,  GPIO_InitdataDef *GPIO_Init) 
{
 
	uint16_t set;
		
	for(uint16_t i=0;i<16;i++)
  {
    set = (GPIO_Init->moder_pin >>i) & 1U;
		
    if(set==1)
		{
					//Start :----------------Moder Register------------------//
			
		if(GPIO_Init->moder_pin_type =='O')
		{
		 GPIOx->MODER |= (1U<< i*2);
    }
	else
	if(GPIO_Init->moder_pin_type == 'I')
  {
	  GPIOx->MODER |= (0U<< i*2);
	}
	else
	if(GPIO_Init->moder_pin_type == 'A')
	{
	 GPIOx->MODER |= (3U<< i*2);
	}
	else
	if(GPIO_Init->moder_pin_type == 'F')
	{
	  GPIOx->MODER |= (2U<< i*2);
	}
	
						//End :----------------Moder Register------------------//
	
	
	
						//Start :--------------Output Register---------------//
	
	if(GPIO_Init->otype_pin_type =='P')
	{
    GPIOx->OTYPER |= (0U<< i);
	}
	else
	if(GPIO_Init->otype_pin_type == 'O')
	{
		GPIOx->OTYPER |= (1U<< i);

	}
			
						//End :---------------Output Register----------------//
  

  
						//Start :--------------Speed Register----------------//
	
	if(GPIO_Init->moder_pin_type != 'I')
	{
	  if(GPIO_Init->ospeed_pin_type == 'L' )
		{
		 GPIOx->OSPEEDR |= (0U<< i*2);
		}else
		if(GPIO_Init->ospeed_pin_type == 'M' )
		{
		  GPIOx->OSPEEDR |= (1U<< i*2);
		}else
		if(GPIO_Init->ospeed_pin_type == 'H' )
		{
			 GPIOx->OSPEEDR |= (2U<< i*2);
	
		}else
		if(GPIO_Init->ospeed_pin_type == 'V' )
		{
			 GPIOx->OSPEEDR |= (3U<< i*2);
		}
}
	
						   //End :-----------------Speed Register---------------//

	

							//Start :---------------Pull-up Pull-down Register-------//
	
	if(GPIO_Init->pupd_pin_type == 'N' )
	{
		GPIOx->PUPDR |= (0U << i*2);
	}else
	if(GPIO_Init->pupd_pin_type == 'U' )
	{
		GPIOx->PUPDR |= (1U << i*2);
	}else
	if(GPIO_Init->pupd_pin_type == 'D' )
	{
    GPIOx->PUPDR |= (2U << i*2);	
	}else
	if(GPIO_Init->pupd_pin_type == 'R' )
	{
		GPIOx->PUPDR |= (3U << i*2);
	}
	
								//End :---------------Pull-up Pull-down Register-------//
  
	
	
								//Start :------------Alternate Function---------------//
	
	if(GPIO_Init->afr_type <16)
	{
		if(i <8)
	  {
		   GPIOx->AFR[0] |=((0U | GPIO_Init->afr_type)<< (i*4)) ;
		}
		else
		{
			 uint16_t temp1 = i%8;	
			 GPIOx->AFR[1] |=((0U | GPIO_Init->afr_type)<< (temp1*4)) ;
		}
	}
							//End :------------Alternate Function---------------//
	
	
							//Start :-----------Locker Register-----------------//
	
	if(GPIO_Init->lck_pin_type == 'L' )
	{
		
		GPIOx->LCKR = 65536U | ((1U << i)|GPIOx->LCKR);
		GPIOx->LCKR = 0U | ((1U << i)|GPIOx->LCKR);
		GPIOx->LCKR = 65536U | ((1U << i)|GPIOx->LCKR);

	}
	
							//End :-----------Locker Register-------------------//
}
	  }
	
}

int main(void)
	{
	 
		GPIO_InitdataDef ght;
		uint32_t mod;
	  uint32_t pup;		
		
		
	  RCC->AHB1ENR = 0x000001FF;
		
		//Start :- Q-1. Inputs
		ght.moder_pin = 15;   		// Pin you want to configure example- 7(0111) means pin 0,1,2 to configure. 8(1000) means pin 3 to configure
		ght.moder_pin_type='A';   // Moder Options     : O - 01 : Output pin, I - 00 : Input pin, A - 11 : Analog mode, F - 10: Function mode
	  ght.otype_pin_type='O';	  // Output Type       : P - 0 : Push-Pull, O - 1 : Output open-drain
		ght.ospeed_pin_type='H';  // Output Speed      : L - 00 : Low-speed, M - 01 : Medium speed, H - 10 : High Speed, V - 11 : Very High Speed 
		ght.pupd_pin_type='D';    // Pull-up/Pull-down : N - 00 : No pull-up, pull-down, U - 01 : Pull-up, D - 10 : Pull-down, R - 11 : Reserved
		ght.afr_type=15;					// Activation Funct  : function from f0-f15 : [0-15]
	  ght.lck_pin_type='N';     // Locking Register  : L - 1 : Locking, N - 0 : Non Locking
			  
		
		  GPIO_TypeDef *ptr1=(GPIO_TypeDef *)GPIOD_BASE;   // Port Name
		  
			GPIO_Init1(ptr1, &ght);				//Q-1. Initializing the registers
	    		
		  GPIO_Read(ptr1);							//Q-2. For reading the values 
			
				
			GPIO_Write_ODR(ptr1,256);     // Q-3. 2nd Arguement : Data to write in ODR regsiter
			
			
			GPIO_SET_BSSR(ptr1,'R',8);    // Q-4. 2nd Arguement : SET - 'S',RESET - 'R' 3rd Arguement : Bit of the ODR Register to set [0-15] 
      
			GPIO_TOGGLE(ptr1);				    // Q-5. Calling for toggling 
		 
		return 0;
	}
	
