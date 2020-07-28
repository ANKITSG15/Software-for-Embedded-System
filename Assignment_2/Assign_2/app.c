#include "stm32f4xx.h"
#include "stdio.h"
#define tcb_size 12
#define PM_REG(ADDRESS)(*((volatile unsigned long*)(ADDRESS)))

volatile uint32_t systick_count=0;			// Counting systick interrupts : At each tick calling scheduler

typedef struct {
  uint32_t arr_time;
	uint32_t per_time;
	uint32_t cpu_time;
	uint32_t rem_time;
	uint32_t lex_time;
	uint32_t dead_time;
}process;

process STATE[3]; 							// Storing arrival,period,execution,laxity and cpu remaining for each tasks respectively.

//Stack of 64 frame each of 4 bytes (32 - bit) for 3 tasks.
uint32_t task0_stack[64],task1_stack[64],task2_stack[64];

uint32_t lex[3];								 // used to store laxity for intermediate computation (find the smallest)


uint32_t PSP_array[3]; 										// TCB Array to store the address of tasks
uint16_t task1_cnt,task2_cnt,task0_cnt;		// Varibles manipulate by respective tasks.
uint32_t svc_exc_return; 									// Storing EXC_RETURN 
uint32_t curr_task=0; 										// Current task
int32_t next_task=1; 											// Next task

//Saving the context for current task
__asm void context(void)
{
   MRS R0, PSP
	 STMDB R0!,{R4-R7}
	 LDR R1,=__cpp(&curr_task)
	 LDR R2,[R1]
	 LDR R3,=__cpp(PSP_array)
	 STR R0,[R3,R2,LSL#2]
}
__asm void PendSV_Handler(void)  // Loading context for next task
	{

	LDR R1,=__cpp(&curr_task)
	LDR R4,=__cpp(&next_task)
	LDR R3,=__cpp(&PSP_array)
	
	LDR R4,[R4]
	STR R4,[R1]
	
	LDR R0,[R3,R4,LSL#2]
	LDMIA r0!,{r4-r7}
	MSR PSP,R0
	BX LR
	ALIGN 4
}


//-----SVC Handler------------------------//

//int __svc(0x10) svc_service_add(int x,int y);


/*--------------- Tasks - [0-3]-----------------------------*/
void task0(void){
	
	while(1)
	{
		task0_cnt=task0_cnt+1;
	}
}

void task1(void){
	
	while(1)
	{
		task1_cnt=task1_cnt+1;
	}
}

void task2(void){
	
	while(1)
	{
		task2_cnt=task2_cnt+1;
	}
}
//---------------Tasks - [0-3]-----------------------------*/

//Initializing parameters for each tasks respectively
void initializeValues(void)
{
	//Initial Values
  STATE[0].arr_time=0;
	STATE[1].arr_time=1;
	STATE[2].arr_time=3;
	
	STATE[0].per_time=3;
	STATE[1].per_time=10;
	STATE[2].per_time=15;
	
	STATE[0].cpu_time=1;
	STATE[1].cpu_time=3;
	STATE[2].cpu_time=4;
	
	STATE[0].rem_time=STATE[0].cpu_time;
	STATE[1].rem_time=STATE[1].cpu_time;
	STATE[2].rem_time=STATE[2].cpu_time;
	
	STATE[0].lex_time = 9999;
	STATE[1].lex_time = 9999;
	STATE[2].lex_time = 9999;
	
	STATE[0].dead_time = STATE[0].per_time+STATE[0].arr_time; 
	STATE[1].dead_time = STATE[1].per_time+STATE[1].arr_time;
	STATE[2].dead_time = STATE[2].per_time+STATE[2].arr_time;
	
}

//Finding the task having the least laxity 
int Find_Smallest(void)
{
	int first,index=-1;
	
	 for (int i = 0; i < 3 ; i ++)  
    {  
       
        if (lex[i] < first)  
        {  
             
            first = lex[i];  
					  index = i;
        }  
  
     }
	return index;
}

//Start : If a task is completed, then its' new arrival time, deadline and remaining time is calculated 
void Update_Arrival(void)
{
	
  if( STATE[0].rem_time == 0)
	{
	  STATE[0].arr_time  += STATE[0].per_time;  
		STATE[0].rem_time = STATE[0].cpu_time;
		STATE[0].dead_time = STATE[0].arr_time + STATE[0].per_time;
	}
  if( STATE[1].rem_time == 0)
	{
	  STATE[1].arr_time += STATE[1].per_time;
		STATE[1].rem_time = STATE[1].cpu_time;
		STATE[1].dead_time = STATE[1].arr_time + STATE[1].per_time;
	}
	if( STATE[2].rem_time ==0)
	{
	  STATE[2].arr_time += STATE[2].per_time;
		STATE[2].rem_time = STATE[2].cpu_time;
	  STATE[2].dead_time = STATE[2].arr_time + STATE[2].per_time;
	}
}

//Computing Laxity for each task 
void scheduler(void)
{
	Update_Arrival();
	
	//printf("State0-%d %d\n",STATE[0].lex_time,STATE[0].rem_time);
	//printf("State1-%d %d\n",STATE[1].lex_time,STATE[1].rem_time);
  //printf("State2-%d %d\n",STATE[2].lex_time,STATE[2].rem_time);
	
	if( STATE[0].arr_time <= systick_count)
	{
	  STATE[0].lex_time= STATE[0].dead_time - systick_count - STATE[0].rem_time;
		lex[0]=STATE[0].lex_time;
	}
	else 
	{
		STATE[0].lex_time=9999;
		lex[0]=STATE[0].lex_time;
	}
  if( STATE[1].arr_time <= systick_count)
	{
	  STATE[1].lex_time= STATE[1].dead_time - systick_count - STATE[1].rem_time;
		lex[1]=STATE[1].lex_time;
	}
	else
	{
		STATE[1].lex_time=9999;
		lex[1]=STATE[1].lex_time;
	}
	if( STATE[2].arr_time <= systick_count)
	{
	  STATE[2].lex_time= STATE[2].dead_time - systick_count - STATE[2].rem_time;
		lex[2]=STATE[2].lex_time;
	}
	else 
	{
		STATE[2].lex_time=9999;
		lex[2]=STATE[2].lex_time;
	}
	
	if((lex[0] == 9999) && (lex[1] == 9999) && (lex[2]== 9999))
	{
			printf("Please ensure to have atleast one task always there in queue.\n");
	}
	
  next_task = Find_Smallest();	
}

//Start : Task Initialization 
void Task_Init(void) {
	// Process Stack Pointer (PSP) value
	PSP_array[0] =(((unsigned int) task0_stack) + (sizeof task0_stack)); 	// Storing Address in TCB Array for Task0 at index 0.
	PM_REG((PSP_array[0] - (4))) = 0x01000000  ; 													// xPSR : Enter in thumb mode( T bit set)
	PM_REG((PSP_array[0] - (8))) = (unsigned long) task0;								 //  PC
	PSP_array[0] = (((unsigned int) task0_stack) + (sizeof task0_stack)- (tcb_size*4));
	
	PSP_array[1] =(((unsigned int) task1_stack) + (sizeof task1_stack)); 
	PM_REG((PSP_array[1] - (4))) = 0x01000000  ; 
	PM_REG((PSP_array[1] - (8))) = (unsigned long) task1; 
	PSP_array[1] = (((unsigned int) task1_stack) + (sizeof task1_stack)- (tcb_size*4));
	
	PSP_array[2] =(((unsigned int) task2_stack) + (sizeof task2_stack)); 
	PM_REG((PSP_array[2] - (4))) = 0x01000000  ; 
	PM_REG((PSP_array[2] - (8))) = (unsigned long) task2; 
	PSP_array[2] = (((unsigned int) task2_stack) + (sizeof task2_stack)- (tcb_size*4));
	
	
	initializeValues();
	
}

//Step 2 : OS Initialization
void OS_Task_Init(void)
{
	  Task_Init();
	 
	  curr_task = 0; // (Current task)
		
		__set_PSP((PSP_array[curr_task] + 4*4)); 				  // Step 3 : Setup PSP
	  
	  NVIC_SetPriority(PendSV_IRQn , 0xFF);						  // Set PendSV to lowest level
		NVIC_SetPriority(SysTick_IRQn, 0x0); 							// Set Timer to highest level
		
		uint32_t returnCode = SysTick_Config(48000); 
		
		svc_exc_return = 0xFFFFFFFD;    									//Modifying EXEC_RETURN for Thread Unprivileged Mode
	 
		__set_CONTROL(0x3);															 // Switch to use Process Stack, unprivileged state 
		
	  __ISB(); 																				// Execute ISB after changing CONTROL (architectural recommendation)
		
}

//Based on given exception number calling OS services for initialization
void SVC_Handler_main(unsigned int *svc_args)
{

	 unsigned int svc_number;
	 svc_number = ((char *) svc_args[6])[-2]; 
	
		if(svc_number == 16)
		{
		  OS_Task_Init();
		}
		else
		{
		    printf("Invalid Exception Number");
		}
	return ;
}

//Step 1 : SVC is called for OS Services
__asm void SVC_Handler(void)
{
	MOVS R0, #4 // Extract stack frame location
	MOV R1, LR
	TST R0, R1
	BEQ stacking_used_MSP
	MRS R0, PSP ; first parameter - stacking was using PSP
	B SVC_Handler_cont
stacking_used_MSP
	MRS R0, MSP ; first parameter - stacking was using MSP
SVC_Handler_cont
	LDR R2,=__cpp(&svc_exc_return) // Save current EXC_RETURN
	MOV R1, LR
	STR R1,[R2]
	BL __cpp(SVC_Handler_main) 			
	LDR R2,=__cpp(&svc_exc_return) // Load new EXC_RETURN
	LDR R1,[R2]
	BX R1
	ALIGN 4
}

//Systick Handler 
void SysTick_Handler(void)
{ 
	// Simple task round robin scheduler
	if(systick_count==0)
	{
		STATE[0].rem_time = STATE[0].rem_time-1;
	}
	  
	systick_count++;
	

	scheduler();				// Calling scheduler for LLF


	if (curr_task!=next_task)
	{ // Context switching needed
		context();
		SCB->ICSR|=0x10000000;	//SET PENDSV TO PENDING
		STATE[curr_task].rem_time = STATE[curr_task].rem_time-1; // It means we are not doing switching so rem is not decremented.
		printf("Task %d called at %d th unit \n",curr_task ,systick_count);
  }
	else
	{
		STATE[curr_task].rem_time = STATE[curr_task].rem_time-1; // It means we are not doing switching so rem is not decremented.
		printf("Task %d called at %d th unit \n",curr_task ,systick_count);
	}
	
	return;
}




