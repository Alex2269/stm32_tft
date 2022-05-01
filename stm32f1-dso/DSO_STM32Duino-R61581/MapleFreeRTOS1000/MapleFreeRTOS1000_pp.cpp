/*
    Thin wrapper on top of FreeRTOS to be more C++ friendly
 */

#include "MapleFreeRTOS1000_pp.h"

#define  fos_ms2tick(ms) (((ms)+portTICK_PERIOD_MS-1)/portTICK_PERIOD_MS)

extern "C" void do_assert(const char *a)
{
     __asm__  ("bkpt 1");  
    while(1)
    {
        
    };
    
}
#define xAssert(a) if(!(a)) {do_assert(#a);}
/**
 * 
 */
extern "C"   void vApplicationMallocFailedHook( void )
{
    do_assert("Malloc");
}

//-- Binary Semaphor --

/**
 * 
 */
xBinarySemaphore::xBinarySemaphore()
{
    _handle=xSemaphoreCreateBinary();
    xAssert(_handle);
}
/**
 * 
 * @return 
 */
bool xBinarySemaphore::take()
{
  return (bool) xSemaphoreTake(_handle,portMAX_DELAY);
}
/**
 * 
 * @param timeoutMs
 * @return 
 */
bool xBinarySemaphore::take(int timeoutMs)
{
  int ticks=1+(timeoutMs*configTICK_RATE_HZ+500)/1000;
  return (bool) xSemaphoreTake(_handle,ticks);
}
/**
 * 
 * @return 
 */
 bool xBinarySemaphore::give()
{
  xAssert(xSemaphoreGive(_handle)); // should never fail
  return true;
}
/**
 * 
 * @return 
 */
 bool xBinarySemaphore::giveFromInterrupt()
{
  BaseType_t awake;
  xSemaphoreGiveFromISR(_handle,&awake); // should never fail... 
  portYIELD_FROM_ISR(awake); // reschedule
  return true;
}
 /**
  * 
  * @param ms
  */
 void xDelay(int ms)
 {
      const TickType_t xDelay = 1+( ms/ portTICK_PERIOD_MS);
      vTaskDelay(xDelay);
 }

 
 xMutex::xMutex()
 {
     _handle=xSemaphoreCreateRecursiveMutex();
 }
 bool xMutex::lock()
 {
    xAssert(xSemaphoreTakeRecursive(_handle,portMAX_DELAY)); // should never fail
    return true;
 }
 bool xMutex::unlock()
 {
    xAssert(xSemaphoreGiveRecursive(_handle)); // should never fail
    return true;
 }

// Task
/**
 * @brief Construct a new x Task::X Task object
 * 
 * @param name 
 * @param entryPoint 
 * @param priority 
 * @param taskSize 
 */
xTask::xTask(const char *name,  int priority, int taskSize)
{
    BaseType_t er=xTaskCreate(xTask::Trampoline,name,taskSize, this,priority,&_taskHandle);
    xAssert(er==pdPASS);    
}
/**
 * @brief Destroy the x Taskx Task object
 * 
 */
xTask::~xTask()
{
  xAssert(0);
}
/**
 * 
 */
xEventGroup::xEventGroup()
{
    _handle=xEventGroupCreate();
    
}
/**
 * 
 */
xEventGroup::~xEventGroup()
{
//    xEventGroupDelete(_handle); No delete !
    xAssert(0);
    _handle=0;
}
/**
 * 
 * @param events
 */
void        xEventGroup::setEvents(uint32_t events)
{
    xEventGroupSetBits(_handle,events);
}
/**
 * 
 * @param events
 */
void        xEventGroup::setEventsFromISR(uint32_t events)
{
    BaseType_t wakeUp ;
    xEventGroupSetBitsFromISR(_handle,events,&wakeUp);
    portYIELD_FROM_ISR(wakeUp); 
}
/**
 * 
 * @param maskint
 * @param timeout
 * @return 
 */
uint32_t    xEventGroup::waitEvents(uint32_t maskint, int timeout)
{
    if(timeout==0) 
        timeout=portMAX_DELAY-1;
    else
        timeout=fos_ms2tick(timeout);
    uint32_t res=xEventGroupWaitBits(
                       _handle,
                       maskint,
                       pdTRUE, // auto clear
                       pdFALSE, // any but
                       timeout );
    return res;
        
}
/**
 * 
 * @param maskInt
 * @return 
 */
uint32_t    xEventGroup::readEvents(uint32_t maskInt) // it is also cleared automatically !
{
     EventBits_t ev=xEventGroupGetBits( _handle );
     ev=ev & maskInt;
     if(ev)
     {
         xEventGroupClearBits(_handle,ev); // Race ?
     }     
     return ev;

}

 //EOF
