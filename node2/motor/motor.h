#ifndef __MOTOR_MOTOR_H__
#define __MOTOR_MOTOR_H__

#include <stdint.h>

void Motor_Init();
int16_t Motor_GetCount();
void Motor_SetSpeed(int32_t speed);
void Motor_SetPosition(uint16_t pos);
int32_t Motor_Loop();
int32_t Motor_GetLastError();

#endif//__MOTOR_MOTOR_H__