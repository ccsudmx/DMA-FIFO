#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "bsp_uart.h"
#include "stdio.h"
#include "string.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "rfid.h"
#include "wifi.h"

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define TASK1_TASK_PRIO		2
//�����ջ��С	
#define TASK1_STK_SIZE 		128  
//������
TaskHandle_t Task1Task_Handler;
//������
void task1_task(void *pvParameters);
TaskHandle_t Task2Task_Handler;
//������
void task2_task(void *pvParameters);

TaskHandle_t Task3Task_Handler;
//������
void task3_task(void *pvParameters);


TaskHandle_t Task4Task_Handler;
//������
void task4_task(void *pvParameters);

//�������ȼ�
#define TASK2_TASK_PRIO		3
//�����ջ��С	
#define TASK2_STK_SIZE 		128  
//������
TaskHandle_t Task2Task_Handler;
//������
void task2_task(void *pvParameters);


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
    Uart_Init();
    
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
   // ����TASK1����
   xTaskCreate((TaskFunction_t )task1_task,             
               (const char*    )"task1_task",           
               (uint16_t       )TASK1_STK_SIZE,        
               (void*          )NULL,                  
               (UBaseType_t    )2,        
               (TaskHandle_t*  )&Task1Task_Handler); 
               

 
                
    xTaskCreate((TaskFunction_t )task4_task,             
                (const char*    )"task4_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )3,        
                (TaskHandle_t*  )&Task4Task_Handler);
                                
                   
 
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}
SemaphoreHandle_t xSemaphore;
SemaphoreHandle_t xSemaphore_2;
SemaphoreHandle_t xSemaphore_3;
SemaphoreHandle_t  xSemaphore_4;

#define RX_SIZE  1024

uint8_t RxBuffer[RX_SIZE];

uint8_t TxBuffer[RX_SIZE];
//task1������
void task1_task(void *pvParameters)
{

	uint16_t len;
	Uart_Init();
 
	//Usart2_Init();					//��ʼ������
	xSemaphore = xSemaphoreCreateCounting( 10, 0 );
	//xSemaphore_2 = xSemaphoreCreateCounting( 10, 0 );
  
	while (1)
   {      

       
		xSemaphoreTake(xSemaphore, portMAX_DELAY);

		len = Uart_Get(USART1, RxBuffer, RX_SIZE) ;	//������������
		if((len>2))	//����ַ
		{
			memset((char *)TxBuffer,0,RX_SIZE);
			sprintf((char *)TxBuffer,"������freetros �Ĵ��� handler:");
			memcpy(TxBuffer+strlen((const char*)TxBuffer),RxBuffer,len);
			sprintf((char *)TxBuffer,"%s\r\n",TxBuffer);
			Uart_Send(USART1, TxBuffer,strlen((const char*)TxBuffer));  
		}
	}
}

void task2_task(void *pvParameters)
{
     //xSemaphoreTake(Init_MQTT, portMAX_DELAY);
     usart3_init(9600);
     xSemaphore_3 = xSemaphoreCreateCounting( 1, 0 );
    while(1)
    {
     // vTaskDelay(100);  
    // xSemaphoreTake(xSemaphore_3, portMAX_DELAY);
      ReadId();
    
    
     }


}
void task3_task(void *pvParameters)
{  
   // DMA_Cmd (DMA1_Channel3,ENABLE); 

  while(1)
  {
  
      RFID_CMD();
     // vTaskDelay(100);
   
  }


}
void task4_task(void *pvParameters)
{
    Uart4_Init();
    xSemaphore_4 = xSemaphoreCreateCounting( 1, 0 );
    Connect_MQTT();
    
    xTaskCreate((TaskFunction_t )task2_task,             
               (const char*    )"task2_task",           
               (uint16_t       )TASK1_STK_SIZE,        
               (void*          )NULL,                  
               (UBaseType_t    )3,        
               (TaskHandle_t*  )&Task2Task_Handler); 
               
    xTaskCreate((TaskFunction_t )task3_task,             
               (const char*    )"task3_task",           
               (uint16_t       )TASK1_STK_SIZE,        
               (void*          )NULL,                  
               (UBaseType_t    )3,        
               (TaskHandle_t*  )&Task3Task_Handler);
     
     
    while(1)
    {
     
     
   
    }


}
