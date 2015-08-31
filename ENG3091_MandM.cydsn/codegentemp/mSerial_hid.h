/*******************************************************************************
* File Name: mSerial_hid.h
* Version 2.80
*
* Description:
*  Header File for the USBFS component. Contains prototypes and constant values.
*
* Related Document:
*  Device Class Definition for Human Interface Devices (HID) Version 1.11
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_USBFS_mSerial_hid_H)
#define CY_USBFS_mSerial_hid_H

#include "cytypes.h"


/***************************************
* Prototypes of the mSerial_hid API.
***************************************/

uint8 mSerial_UpdateHIDTimer(uint8 interface) ;
uint8 mSerial_GetProtocol(uint8 interface) ;


/***************************************
*Renamed Functions for backward compatible
***************************************/

#define mSerial_bGetProtocol               mSerial_GetProtocol


/***************************************
*  Constants for mSerial_hid API.
***************************************/

#define mSerial_PROTOCOL_BOOT              (0x00u)
#define mSerial_PROTOCOL_REPORT            (0x01u)

/* Request Types (HID Chapter 7.2) */
#define mSerial_HID_GET_REPORT             (0x01u)
#define mSerial_HID_GET_IDLE               (0x02u)
#define mSerial_HID_GET_PROTOCOL           (0x03u)
#define mSerial_HID_SET_REPORT             (0x09u)
#define mSerial_HID_SET_IDLE               (0x0Au)
#define mSerial_HID_SET_PROTOCOL           (0x0Bu)

/* Descriptor Types (HID Chapter 7.1) */
#define mSerial_DESCR_HID_CLASS            (0x21u)
#define mSerial_DESCR_HID_REPORT           (0x22u)
#define mSerial_DESCR_HID_PHYSICAL         (0x23u)

/* Report Request Types (HID Chapter 7.2.1) */
#define mSerial_HID_GET_REPORT_INPUT       (0x01u)
#define mSerial_HID_GET_REPORT_OUTPUT      (0x02u)
#define mSerial_HID_GET_REPORT_FEATURE     (0x03u)

#endif /*  CY_USBFS_mSerial_hid_H */


/* [] END OF FILE */
