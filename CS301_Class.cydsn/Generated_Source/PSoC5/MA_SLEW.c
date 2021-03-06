/*******************************************************************************
* File Name: MA_SLEW.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "MA_SLEW.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 MA_SLEW__PORT == 15 && ((MA_SLEW__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: MA_SLEW_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void MA_SLEW_Write(uint8 value) 
{
    uint8 staticBits = (MA_SLEW_DR & (uint8)(~MA_SLEW_MASK));
    MA_SLEW_DR = staticBits | ((uint8)(value << MA_SLEW_SHIFT) & MA_SLEW_MASK);
}


/*******************************************************************************
* Function Name: MA_SLEW_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  MA_SLEW_DM_STRONG     Strong Drive 
*  MA_SLEW_DM_OD_HI      Open Drain, Drives High 
*  MA_SLEW_DM_OD_LO      Open Drain, Drives Low 
*  MA_SLEW_DM_RES_UP     Resistive Pull Up 
*  MA_SLEW_DM_RES_DWN    Resistive Pull Down 
*  MA_SLEW_DM_RES_UPDWN  Resistive Pull Up/Down 
*  MA_SLEW_DM_DIG_HIZ    High Impedance Digital 
*  MA_SLEW_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void MA_SLEW_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(MA_SLEW_0, mode);
}


/*******************************************************************************
* Function Name: MA_SLEW_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro MA_SLEW_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 MA_SLEW_Read(void) 
{
    return (MA_SLEW_PS & MA_SLEW_MASK) >> MA_SLEW_SHIFT;
}


/*******************************************************************************
* Function Name: MA_SLEW_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 MA_SLEW_ReadDataReg(void) 
{
    return (MA_SLEW_DR & MA_SLEW_MASK) >> MA_SLEW_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(MA_SLEW_INTSTAT) 

    /*******************************************************************************
    * Function Name: MA_SLEW_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 MA_SLEW_ClearInterrupt(void) 
    {
        return (MA_SLEW_INTSTAT & MA_SLEW_MASK) >> MA_SLEW_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
