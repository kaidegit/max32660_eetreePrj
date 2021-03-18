#include <stdbool.h>
#include "notifications.h"
#include "uart.h"
#include "main.h"
#include "max32660.h"
#include "stdio.h"
#include "string.h"

char notificationTitle[30];
char notificationMessage[255];