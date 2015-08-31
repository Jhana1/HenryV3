/*******************************************************************************
* File Name: OV9650_1_SIOD.c  
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
#include "OV9650_1_SIOD.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 OV9650_1_SIOD__PORT == 15 && ((OV9650_1_SIOD__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: OV9650_1_SIOD_Write
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
void OV9650_1_SIOD_Write(uint8 value) 
{
    uint8 staticBits = (OV9650_1_SIOD_DR & (uint8)(~OV9650_1_SIOD_MASK));
    OV9650_1_SIOD_DR = staticBits | ((uint8)(value << OV9650_1_SIOD_SHIFT) & OV9650_1_SIOD_MASK);
}


/*******************************************************************************
* Function Name: OV9650_1_SIOD_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  OV9650_1_SIOD_DM_STRONG     Strong Drive 
*  OV9650_1_SIOD_DM_OD_HI      Open Drain, Drives High 
*  OV9650_1_SIOD_DM_OD_LO      Open Drain, Drives Low 
*  OV9650_1_SIOD_DM_RES_UP     Resistive Pull Up 
*  OV9650_1_SIOD_DM_RES_DWN    Resistive Pull Down 
*  OV9650_1_SIOD_DM_RES_UPDWN  Resistive Pull Up/Down 
*  OV9650_1_SIOD_DM_DIG_HIZ    High Impedance Digital 
*  OV9650_1_SIOD_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void OV9650_1_SIOD_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(OV9650_1_SIOD_0, mode);
}


/*******************************************************************************
* Function Name: OV9650_1_SIOD_Read
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
*  Macro OV9650_1_SIOD_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 OV9650_1_SIOD_Read(void) 
{
    return (OV9650_1_SIOD_PS & OV9650_1_SIOD_MASK) >> OV9650_1_SIOD_SHIFT;
}


/*******************************************************************************
* Function Name: OV9650_1_SIOD_ReadDataReg
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
uint8 OV9650_1_SIOD_ReadDataReg(void) 
{
    return (OV9650_1_SIOD_DR & OV9650_1_SIOD_MASK) >> OV9650_1_SIOD_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(OV9650_1_SIOD_INTSTAT) 

    /*******************************************************************************
    * Function Name: OV9650_1_SIOD_ClearInterrupt
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
    uint8 OV9650_1_SIOD_ClearInterrupt(void) 
    {
        return (OV9650_1_SIOD_INTSTAT & OV9650_1_SIOD_MASK) >> OV9650_1_SIOD_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
