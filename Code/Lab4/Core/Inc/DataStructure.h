/*
 * DataStructure.h
 *
 *  Created on: Nov 19, 2024
 *      Author: Mr Duc Anh
 */

#ifndef INC_DATASTRUCTURE_H_
#define INC_DATASTRUCTURE_H_
#include "main.h"
#include "stdint.h"
#define MAX_TASKS 40
#define NO_TASK_ID 0

// Function prototypes
typedef struct {
    void (*pTask)(void);  // Hàm thực thi của tác vụ
    uint32_t delay;       // Thời gian trì hoãn (đơn vị: ticks)
    uint32_t period;      // Khoảng thời gian giữa hai lần thực thi (đơn vị: ticks)
    uint8_t runMe;         // Cờ hiệu cho biết tác vụ có nên được thực thi hay không
    uint32_t taskId;      // ID duy nhất của tác vụ
} sTask;

void SCH_Init(void);
void SCH_Update(void);
unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Delete_Task(const unsigned int TASK_INDEX);
//void SCH_Go_To_Sleep(void);  

void SCH_Report_Status(void);

// Global variables
extern sTask SCH_tasks_G[MAX_TASKS];
extern unsigned char Error_code_G;


#endif /* INC_DATASTRUCTURE_H_ */
