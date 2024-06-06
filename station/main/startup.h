#ifndef __START_UP_H__
#define __START_UP_H__


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t preStartupSemaphore;

void preStartup();
void midStartup();
void postStartup();

#endif