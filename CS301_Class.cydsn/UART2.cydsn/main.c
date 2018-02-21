/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    UART_Start();
    uint16 count1 = 864;
    char str[15];
    
    for(;;)
    {
        /* Place your application code here. */
        
        //sprintf(str, "%d", count1);
        UART_PutString("f");
        CyDelay(1);
        
    }
}

/* [] END OF FILE */
