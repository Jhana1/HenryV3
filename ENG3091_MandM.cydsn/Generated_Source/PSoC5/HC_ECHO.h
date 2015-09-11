/*******************************************************************************
* File Name: HC_ECHO.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_HC_ECHO_H) /* Pins HC_ECHO_H */
#define CY_PINS_HC_ECHO_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "HC_ECHO_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 HC_ECHO__PORT == 15 && ((HC_ECHO__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    HC_ECHO_Write(uint8 value) ;
void    HC_ECHO_SetDriveMode(uint8 mode) ;
uint8   HC_ECHO_ReadDataReg(void) ;
uint8   HC_ECHO_Read(void) ;
uint8   HC_ECHO_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define HC_ECHO_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define HC_ECHO_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define HC_ECHO_DM_RES_UP          PIN_DM_RES_UP
#define HC_ECHO_DM_RES_DWN         PIN_DM_RES_DWN
#define HC_ECHO_DM_OD_LO           PIN_DM_OD_LO
#define HC_ECHO_DM_OD_HI           PIN_DM_OD_HI
#define HC_ECHO_DM_STRONG          PIN_DM_STRONG
#define HC_ECHO_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define HC_ECHO_MASK               HC_ECHO__MASK
#define HC_ECHO_SHIFT              HC_ECHO__SHIFT
#define HC_ECHO_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define HC_ECHO_PS                     (* (reg8 *) HC_ECHO__PS)
/* Data Register */
#define HC_ECHO_DR                     (* (reg8 *) HC_ECHO__DR)
/* Port Number */
#define HC_ECHO_PRT_NUM                (* (reg8 *) HC_ECHO__PRT) 
/* Connect to Analog Globals */                                                  
#define HC_ECHO_AG                     (* (reg8 *) HC_ECHO__AG)                       
/* Analog MUX bux enable */
#define HC_ECHO_AMUX                   (* (reg8 *) HC_ECHO__AMUX) 
/* Bidirectional Enable */                                                        
#define HC_ECHO_BIE                    (* (reg8 *) HC_ECHO__BIE)
/* Bit-mask for Aliased Register Access */
#define HC_ECHO_BIT_MASK               (* (reg8 *) HC_ECHO__BIT_MASK)
/* Bypass Enable */
#define HC_ECHO_BYP                    (* (reg8 *) HC_ECHO__BYP)
/* Port wide control signals */                                                   
#define HC_ECHO_CTL                    (* (reg8 *) HC_ECHO__CTL)
/* Drive Modes */
#define HC_ECHO_DM0                    (* (reg8 *) HC_ECHO__DM0) 
#define HC_ECHO_DM1                    (* (reg8 *) HC_ECHO__DM1)
#define HC_ECHO_DM2                    (* (reg8 *) HC_ECHO__DM2) 
/* Input Buffer Disable Override */
#define HC_ECHO_INP_DIS                (* (reg8 *) HC_ECHO__INP_DIS)
/* LCD Common or Segment Drive */
#define HC_ECHO_LCD_COM_SEG            (* (reg8 *) HC_ECHO__LCD_COM_SEG)
/* Enable Segment LCD */
#define HC_ECHO_LCD_EN                 (* (reg8 *) HC_ECHO__LCD_EN)
/* Slew Rate Control */
#define HC_ECHO_SLW                    (* (reg8 *) HC_ECHO__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define HC_ECHO_PRTDSI__CAPS_SEL       (* (reg8 *) HC_ECHO__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define HC_ECHO_PRTDSI__DBL_SYNC_IN    (* (reg8 *) HC_ECHO__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define HC_ECHO_PRTDSI__OE_SEL0        (* (reg8 *) HC_ECHO__PRTDSI__OE_SEL0) 
#define HC_ECHO_PRTDSI__OE_SEL1        (* (reg8 *) HC_ECHO__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define HC_ECHO_PRTDSI__OUT_SEL0       (* (reg8 *) HC_ECHO__PRTDSI__OUT_SEL0) 
#define HC_ECHO_PRTDSI__OUT_SEL1       (* (reg8 *) HC_ECHO__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define HC_ECHO_PRTDSI__SYNC_OUT       (* (reg8 *) HC_ECHO__PRTDSI__SYNC_OUT) 


#if defined(HC_ECHO__INTSTAT)  /* Interrupt Registers */

    #define HC_ECHO_INTSTAT                (* (reg8 *) HC_ECHO__INTSTAT)
    #define HC_ECHO_SNAP                   (* (reg8 *) HC_ECHO__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_HC_ECHO_H */


/* [] END OF FILE */
