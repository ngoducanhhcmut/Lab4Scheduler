/*
 * DataStructure.c
 *
 *  Created on: Nov 19, 2024
 *      Author: Mr Duc Anh
 */


#include"DataStructure.h"
#include "main.h"
// Cấu trúc đại diện cho một tác vụ


// Số lượng tối đa các tác vụ
#define SCH_MAX_TASKS 40
#define NO_TASK_ID 0

#define ERROR_SCH_TOO_MANY_TASKS   (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)
#define RETURN_ERROR               (-1)
#define RETURN_NORMAL              (0)

uint8_t Last_error_code_G;
uint8_t Error_port;
uint32_t Error_tick_count_G;

unsigned char Error_code_G;

// Mảng các tác vụ
sTask SCH_tasks_G [SCH_MAX_TASKS];

void SCH_Init(void) {
    unsigned char i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_Delete_Task(i);
    }
    // Reset the global error variable
    // SCH_Delete_Task() will generate an error code,
    // (because the task array is empty)
    Error_code_G = 0;
    //Timer_init();
    //Watchdog_init();  

}

void SCH_Update(void) {
    unsigned char Index;

    // NOTE: calculations are in *TICKS* (not milliseconds)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        // Check if there is a task at this location
        if (SCH_tasks_G[Index].pTask) {
            if (SCH_tasks_G[Index].delay == 0) {
                // The task is due to run
                // Inc. the 'RunMe' flag
                SCH_tasks_G[Index].runMe += 1;
                if (SCH_tasks_G[Index].period) {
                    // Schedule periodic tasks to run again
                    SCH_tasks_G[Index].delay = SCH_tasks_G[Index].period;
                }
            } else {
                // Not yet ready to run: just decrement the delay
                SCH_tasks_G[Index].delay -= 1;
            }
        }
    }
}


unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
  unsigned char Index = 0;
  // First find a gap in the array (if there is one)
  while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
    Index++;
  }
  // Have we reached the end of the list?
  if (Index == SCH_MAX_TASKS) {
    // Task list is full
    // Set the global error variable
    Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
    // Also return an error code
    return SCH_MAX_TASKS;
  }
  // If we're here, there is a space in the task array  

  SCH_tasks_G[Index].pTask = pFunction;
  SCH_tasks_G[Index].delay = DELAY;
  SCH_tasks_G[Index].period = PERIOD;
  SCH_tasks_G[Index].runMe = 0;
  SCH_tasks_G[Index].taskId = Index;
  // return position of task (to allow later deletion)
  return Index;
}

void SCH_Dispatch_Tasks(void) {
    unsigned char Index;

    // Dispatches (runs) the next task (if one is ready)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].runMe > 0) {
            (*SCH_tasks_G[Index].pTask)(); // Run the task
            SCH_tasks_G[Index].runMe -= 1; // Reset / reduce RunMe flag
            // Periodic tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
            if (SCH_tasks_G[Index].period == 0) {
                SCH_Delete_Task(Index);
            }
        }
    }

    // Report system status
    SCH_Report_Status();

    // The scheduler enters idle mode at this point
    //SCH_Go_To_Sleep();
}


unsigned char SCH_Delete_Task(const unsigned int TASK_INDEX) {
    unsigned char Return_code;

    if (SCH_tasks_G[TASK_INDEX].pTask == 0) {
        // No task at this location...

        // Set the global error variable
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

        // ... also return an error code
        Return_code = RETURN_ERROR;
    } else {
        Return_code = RETURN_NORMAL;
    }

    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].delay = 0;
    SCH_tasks_G[TASK_INDEX].period = 0;
    SCH_tasks_G[TASK_INDEX].runMe = 0;

    return Return_code; // return status
}


void SCH_Report_Status(void) {
//#ifdef SCH_REPORT_ERRORS // Kiểm tra xem định nghĩa SCH_REPORT_ERRORS có được khai báo hay không
    // Chỉ thực hiện khi báo cáo lỗi được bật
    if (Error_code_G != Last_error_code_G) { // Kiểm tra xem có lỗi mới xảy ra không
        // Giả định logic đảo ngược trên LED
        Error_port = 255 - Error_code_G; // Cập nhật cổng lỗi
        Last_error_code_G = Error_code_G; // Cập nhật mã lỗi cũ
        if (Error_code_G != 0) { // Nếu có lỗi
            Error_tick_count_G = 60000; // Khởi tạo bộ đếm thời gian lỗi
        } else {
            Error_tick_count_G = 0; // Reset bộ đếm thời gian lỗi
        }
    } else {
        if (Error_tick_count_G != 0) { // Nếu bộ đếm thời gian lỗi khác 0
            if (--Error_tick_count_G == 0) { // Giảm bộ đếm và kiểm tra xem đã đạt 0 chưa
                Error_code_G = 0; // Reset mã lỗi
            }
        }
    }
//#endif
}
//void SCH_Go_To_Sleep() {
//    //: Optional
//}

//void SCH_Report_Status(void) {
//#ifdef SCH_REPORT_ERRORS
//    // ONLY APPLIES IF WE ARE REPORTING ERRORS
//    // Check for a new error code
//    if (Error_code_G != Last_error_code_G) {
//        // Negative logic on LEDs assumed
//        Error_port = 255 - Error_code_G;
//        Last_error_code_G = Error_code_G;
//        if (Error_code_G != 0) {
//            Error_tick_count_G = 60000;
//        } else {
//            Error_tick_count_G = 0;
//        }
//    } else {
//        if (Error_tick_count_G != 0) {
//            if (--Error_tick_count_G == 0) {
//                Error_code_G = 0; // Reset error code
//            }
//        }
//    }
//#endif  

//}



