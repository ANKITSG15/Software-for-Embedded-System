

void Task_Init(void);
void initializeValues(void);
void OS_Task_Init(void);
void SVC_Handler_main(unsigned int *svc_args);
int Find_Smallest(void);
void Update_Arrival(void);
void scheduler(void);
void SysTick_Handler(void);

//__asm void SVC_Handler(void);
//__asm void context(void);
//__asm void PendSV_Handler(void);


void task0(void);
void task1(void);
void task2(void);


//SVC function to raise SVC having Exception number 16
int __svc(0x10) svc_service_add(int x,int y);

