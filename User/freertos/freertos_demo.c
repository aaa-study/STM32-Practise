#include "./freertos/freertos_demo.h"

/*Freertos相关的头文件*/
#include "FreeRTOS.h"
#include "task.h"

/*需要用到的其他头文件*/
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"
#include "./usart/bsp_debug_usart.h"


/*启动任务的配置*/
#define START_TASK_STACK 512 
#define START_TASK_PRIORITY 1 
TaskHandle_t start_task_handle = NULL; 
void start_task(void *pvParameters);

/*任务1的配置*/
#define TASK1_STACK 512 
#define TASK1_PRIORITY 2 
TaskHandle_t task1_handle = NULL; 
void task1(void *pvParameters);

/*任务2的配置*/
#define TASK2_STACK 512 
#define TASK2_PRIORITY 3 
TaskHandle_t task2_handle = NULL; 
void task2(void *pvParameters);

/*任务3的配置*/
#define TASK3_STACK 512 
#define TASK3_PRIORITY 4 
TaskHandle_t task3_handle = NULL; 
void task3(void *pvParameters);


/**
 * @description: 启动FreeRTOS
 * @return {*}
 */
void freertos_start(void )
{
    /*1、创建一个启动任务*/
    xTaskCreate((TaskFunction_t)start_task, 								/*任务函数*/
                (const char *)"start_task",					 				/*任务名称*/
                (configSTACK_DEPTH_TYPE)START_TASK_STACK, 	/*任务栈大小*/
                (void *)NULL, 															/*传递给任务的参数*/
                (UBaseType_t)START_TASK_PRIORITY, 					/*任务优先级*/
                (TaskHandle_t *)&start_task_handle); 				/*任务句柄的地址*/

    /*2、启动调度器,会自动创建空闲任务*/
    vTaskStartScheduler(); 
}

/**
 * @description: 启动任务，用来创建其他Task
 * @param {void} *pvParameters 任务参数
 * @return {*}
 */
void start_task(void *pvParameters)
{
    /*进入临界区*/
    taskENTER_CRITICAL(); 
	
	printf ("这是一个动态创建任务的工程模板\r\n");
	
		/*创建3个任务*/
    xTaskCreate((TaskFunction_t)task1, 
                (char *)"task1", 
                (configSTACK_DEPTH_TYPE)TASK1_STACK, 
                (void *)NULL, 
                (UBaseType_t)TASK1_PRIORITY, 
                (TaskHandle_t *)&task1_handle);
	printf ("Task1创建成功\r\n");
								
    xTaskCreate((TaskFunction_t)task2, 
                (char *)"task2", 
                (configSTACK_DEPTH_TYPE)TASK2_STACK, 
                (void *)NULL, 
                (UBaseType_t)TASK2_PRIORITY, 
                (TaskHandle_t *)&task2_handle); 
	printf ("Task2创建成功\r\n");

		xTaskCreate((TaskFunction_t)task3, 
                (char *)"task3", 
                (configSTACK_DEPTH_TYPE)TASK3_STACK, 
                (void *)NULL,
                (UBaseType_t)TASK3_PRIORITY, 
                (TaskHandle_t *)&task3_handle); 
	printf ("Task3创建成功\r\n");
    
    /*启动任务只需要执行一次即可，用完就删，删除启动任务*/
    vTaskDelete(start_task_handle); 

		/*退出临界区*/
    taskEXIT_CRITICAL(); 

}
/**
 * @description: 任务一：实现LED1每500ms闪烁一次
 * @param {void} *pvParameters 任务参数
 * @return {*}
 */

void task1(void *pvParameters)
{
    while(1)
    {
        /*任务1的代码*/
        printf("task1运行...\r\n"); 
				LED1_TOGGLE;
				vTaskDelay(500); /*延时500ms*/
			  printf("task1运行成功...\r\n"); 

    }
}
/**
 * @description: 任务二：实现LED2每500ms闪烁一次
 * @param {void} *pvParameters 任务参数
 * @return {*}
 */

void task2(void *pvParameters)
{
    while(1)
    {
        /*任务2的代码*/
        printf("task2运行...\r\n"); 
				LED2_TOGGLE;
				vTaskDelay(500); /*延时500ms*/
				printf("task2运行成功...\r\n"); 

    }
} 
/**
 * @description: 任务三：判断按键KEY1是否按下，按下删除task1
 * @param {void} *pvParameters 任务参数
 * @return {*}
 */
void task3(void *pvParameters)
{
    uint8_t key_value = 0; 
    while(1)
    {
         printf("task3运行...\r\n"); 
        
         key_value = Key_Scan(GPIOA,GPIO_PIN_0); 
         if(key_value == KEY_ON) 
         {
            /*判断是否已经删除，避免重复执行删除*/
            if(task1_handle != NULL)
							 /*删除任务1*/
            vTaskDelete(task1_handle); 
            task1_handle = NULL; /*置空任务句柄*/
						printf("task1已删除...\r\n"); /*打印任务1已删除*/
         }
            vTaskDelay(500); /*延时100ms*/
				 		printf("task3运行成功...\r\n"); 

    }

}
