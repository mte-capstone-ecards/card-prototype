
#include "button.h"

#if FTR_BUTTON

#include "cmsis_os.h"

// Button Queue, when a button that is not currently in the queue is pressed, add it to the queue
extern osMessageQueueId_t buttonEventQueueHandle;




#endif
