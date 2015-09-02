/*******************************************************************************
* File Name: OV9650_1_I2C_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "OV9650_1_I2C_PVT.h"


/*******************************************************************************
*  Place your includes, defines and code here.
********************************************************************************/
/* `#START OV9650_1_I2C_ISR_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: OV9650_1_I2C_ISR
********************************************************************************
*
* Summary:
*  The handler for the I2C interrupt. The slave and master operations are
*  handled here.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
CY_ISR(OV9650_1_I2C_ISR)
{
#if (OV9650_1_I2C_MODE_SLAVE_ENABLED)
   uint8  tmp8;
#endif  /* (OV9650_1_I2C_MODE_SLAVE_ENABLED) */

    uint8  tmpCsr;

#if(OV9650_1_I2C_TIMEOUT_FF_ENABLED)
    if(0u != OV9650_1_I2C_TimeoutGetStatus())
    {
        OV9650_1_I2C_TimeoutReset();
        OV9650_1_I2C_state = OV9650_1_I2C_SM_EXIT_IDLE;
        /* OV9650_1_I2C_CSR_REG should be cleared after reset */
    }
#endif /* (OV9650_1_I2C_TIMEOUT_FF_ENABLED) */


    tmpCsr = OV9650_1_I2C_CSR_REG;      /* Make copy as interrupts clear */

#if(OV9650_1_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED)
    if(OV9650_1_I2C_CHECK_START_GEN(OV9650_1_I2C_MCSR_REG))
    {
        OV9650_1_I2C_CLEAR_START_GEN;

        /* Set transfer complete and error flags */
        OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_ERR_XFER |
                                        OV9650_1_I2C_GET_MSTAT_CMPLT);

        /* Slave was addressed */
        OV9650_1_I2C_state = OV9650_1_I2C_SM_SLAVE;
    }
#endif /* (OV9650_1_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED) */


#if(OV9650_1_I2C_MODE_MULTI_MASTER_ENABLED)
    if(OV9650_1_I2C_CHECK_LOST_ARB(tmpCsr))
    {
        /* Set errors */
        OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_ERR_XFER     |
                                        OV9650_1_I2C_MSTAT_ERR_ARB_LOST |
                                        OV9650_1_I2C_GET_MSTAT_CMPLT);

        OV9650_1_I2C_DISABLE_INT_ON_STOP; /* Interrupt on Stop is enabled by write */

        #if(OV9650_1_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(OV9650_1_I2C_CHECK_ADDRESS_STS(tmpCsr))
            {
                /* Slave was addressed */
                OV9650_1_I2C_state = OV9650_1_I2C_SM_SLAVE;
            }
            else
            {
                OV9650_1_I2C_BUS_RELEASE;

                OV9650_1_I2C_state = OV9650_1_I2C_SM_EXIT_IDLE;
            }
        #else
            OV9650_1_I2C_BUS_RELEASE;

            OV9650_1_I2C_state = OV9650_1_I2C_SM_EXIT_IDLE;

        #endif /* (OV9650_1_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED) */
    }
#endif /* (OV9650_1_I2C_MODE_MULTI_MASTER_ENABLED) */

    /* Check for master operation mode */
    if(OV9650_1_I2C_CHECK_SM_MASTER)
    {
    #if(OV9650_1_I2C_MODE_MASTER_ENABLED)
        if(OV9650_1_I2C_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            switch (OV9650_1_I2C_state)
            {
            case OV9650_1_I2C_SM_MSTR_WR_ADDR:  /* After address is sent, write data */
            case OV9650_1_I2C_SM_MSTR_RD_ADDR:  /* After address is sent, read data */

                tmpCsr &= ((uint8) ~OV9650_1_I2C_CSR_STOP_STATUS); /* Clear Stop bit history on address phase */

                if(OV9650_1_I2C_CHECK_ADDR_ACK(tmpCsr))
                {
                    /* Setup for transmit or receive of data */
                    if(OV9650_1_I2C_state == OV9650_1_I2C_SM_MSTR_WR_ADDR)   /* TRANSMIT data */
                    {
                        /* Check if at least one byte to transfer */
                        if(OV9650_1_I2C_mstrWrBufSize > 0u)
                        {
                            /* Load the 1st data byte */
                            OV9650_1_I2C_DATA_REG = OV9650_1_I2C_mstrWrBufPtr[0u];
                            OV9650_1_I2C_TRANSMIT_DATA;
                            OV9650_1_I2C_mstrWrBufIndex = 1u;   /* Set index to 2nd element */

                            /* Set transmit state until done */
                            OV9650_1_I2C_state = OV9650_1_I2C_SM_MSTR_WR_DATA;
                        }
                        /* End of buffer: complete writing */
                        else if(OV9650_1_I2C_CHECK_NO_STOP(OV9650_1_I2C_mstrControl))
                        {
                            /* Set write complete and master halted */
                            OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_XFER_HALT |
                                                            OV9650_1_I2C_MSTAT_WR_CMPLT);

                            OV9650_1_I2C_state = OV9650_1_I2C_SM_MSTR_HALT; /* Expect ReStart */
                            OV9650_1_I2C_DisableInt();
                        }
                        else
                        {
                            OV9650_1_I2C_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                            OV9650_1_I2C_GENERATE_STOP;
                        }
                    }
                    else  /* Master receive data */
                    {
                        OV9650_1_I2C_READY_TO_READ; /* Release bus to read data */

                        OV9650_1_I2C_state  = OV9650_1_I2C_SM_MSTR_RD_DATA;
                    }
                }
                /* Address is NACKed */
                else if(OV9650_1_I2C_CHECK_ADDR_NAK(tmpCsr))
                {
                    /* Set Address NAK error */
                    OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_ERR_XFER |
                                                    OV9650_1_I2C_MSTAT_ERR_ADDR_NAK);

                    if(OV9650_1_I2C_CHECK_NO_STOP(OV9650_1_I2C_mstrControl))
                    {
                        OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_XFER_HALT |
                                                        OV9650_1_I2C_GET_MSTAT_CMPLT);

                        OV9650_1_I2C_state = OV9650_1_I2C_SM_MSTR_HALT; /* Expect RESTART */
                        OV9650_1_I2C_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        OV9650_1_I2C_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                        OV9650_1_I2C_GENERATE_STOP;
                    }
                }
                else
                {
                    /* Address phase is not set for some reason: error */
                    #if(OV9650_1_I2C_TIMEOUT_ENABLED)
                        /* Exit interrupt to take chance for timeout timer to handle this case */
                        OV9650_1_I2C_DisableInt();
                        OV9650_1_I2C_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (OV9650_1_I2C_TIMEOUT_ENABLED) */
                }
                break;

            case OV9650_1_I2C_SM_MSTR_WR_DATA:

                if(OV9650_1_I2C_CHECK_DATA_ACK(tmpCsr))
                {
                    /* Check if end of buffer */
                    if(OV9650_1_I2C_mstrWrBufIndex  < OV9650_1_I2C_mstrWrBufSize)
                    {
                        OV9650_1_I2C_DATA_REG =
                                                 OV9650_1_I2C_mstrWrBufPtr[OV9650_1_I2C_mstrWrBufIndex];
                        OV9650_1_I2C_TRANSMIT_DATA;
                        OV9650_1_I2C_mstrWrBufIndex++;
                    }
                    /* End of buffer: complete writing */
                    else if(OV9650_1_I2C_CHECK_NO_STOP(OV9650_1_I2C_mstrControl))
                    {
                        /* Set write complete and master halted */
                        OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_XFER_HALT |
                                                        OV9650_1_I2C_MSTAT_WR_CMPLT);

                        OV9650_1_I2C_state = OV9650_1_I2C_SM_MSTR_HALT;    /* Expect restart */
                        OV9650_1_I2C_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        OV9650_1_I2C_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                        OV9650_1_I2C_GENERATE_STOP;
                    }
                }
                /* Last byte NAKed: end writing */
                else if(OV9650_1_I2C_CHECK_NO_STOP(OV9650_1_I2C_mstrControl))
                {
                    /* Set write complete, short transfer and master halted */
                    OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_ERR_XFER       |
                                                    OV9650_1_I2C_MSTAT_ERR_SHORT_XFER |
                                                    OV9650_1_I2C_MSTAT_XFER_HALT      |
                                                    OV9650_1_I2C_MSTAT_WR_CMPLT);

                    OV9650_1_I2C_state = OV9650_1_I2C_SM_MSTR_HALT;    /* Expect ReStart */
                    OV9650_1_I2C_DisableInt();
                }
                else  /* Do normal Stop */
                {
                    OV9650_1_I2C_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                    OV9650_1_I2C_GENERATE_STOP;

                    /* Set short transfer and error flag */
                    OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_ERR_SHORT_XFER |
                                                    OV9650_1_I2C_MSTAT_ERR_XFER);
                }

                break;

            case OV9650_1_I2C_SM_MSTR_RD_DATA:

                OV9650_1_I2C_mstrRdBufPtr[OV9650_1_I2C_mstrRdBufIndex] = OV9650_1_I2C_DATA_REG;
                OV9650_1_I2C_mstrRdBufIndex++;

                /* Check if end of buffer */
                if(OV9650_1_I2C_mstrRdBufIndex < OV9650_1_I2C_mstrRdBufSize)
                {
                    OV9650_1_I2C_ACK_AND_RECEIVE;       /* ACK and receive byte */
                }
                /* End of buffer: complete reading */
                else if(OV9650_1_I2C_CHECK_NO_STOP(OV9650_1_I2C_mstrControl))
                {
                    /* Set read complete and master halted */
                    OV9650_1_I2C_mstrStatus |= (OV9650_1_I2C_MSTAT_XFER_HALT |
                                                    OV9650_1_I2C_MSTAT_RD_CMPLT);

                    OV9650_1_I2C_state = OV9650_1_I2C_SM_MSTR_HALT;    /* Expect ReStart */
                    OV9650_1_I2C_DisableInt();
                }
                else
                {
                    OV9650_1_I2C_ENABLE_INT_ON_STOP;
                    OV9650_1_I2C_NAK_AND_RECEIVE;       /* NACK and TRY to generate Stop */
                }
                break;

            default: /* This is an invalid state and should not occur */

            #if(OV9650_1_I2C_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handles this case */
                OV9650_1_I2C_DisableInt();
                OV9650_1_I2C_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (OV9650_1_I2C_TIMEOUT_ENABLED) */

                break;
            }
        }

        /* Catches Stop: end of transaction */
        if(OV9650_1_I2C_CHECK_STOP_STS(tmpCsr))
        {
            OV9650_1_I2C_mstrStatus |= OV9650_1_I2C_GET_MSTAT_CMPLT;

            OV9650_1_I2C_DISABLE_INT_ON_STOP;
            OV9650_1_I2C_state = OV9650_1_I2C_SM_IDLE;
        }
    #endif /* (OV9650_1_I2C_MODE_MASTER_ENABLED) */
    }
    else if(OV9650_1_I2C_CHECK_SM_SLAVE)
    {
    #if(OV9650_1_I2C_MODE_SLAVE_ENABLED)

        if((OV9650_1_I2C_CHECK_STOP_STS(tmpCsr)) || /* Stop || Restart */
           (OV9650_1_I2C_CHECK_BYTE_COMPLETE(tmpCsr) && OV9650_1_I2C_CHECK_ADDRESS_STS(tmpCsr)))
        {
            /* Catch end of master write transaction: use interrupt on Stop */
            /* The Stop bit history on address phase does not have correct state */
            if(OV9650_1_I2C_SM_SL_WR_DATA == OV9650_1_I2C_state)
            {
                OV9650_1_I2C_DISABLE_INT_ON_STOP;

                OV9650_1_I2C_slStatus &= ((uint8) ~OV9650_1_I2C_SSTAT_WR_BUSY);
                OV9650_1_I2C_slStatus |= ((uint8)  OV9650_1_I2C_SSTAT_WR_CMPLT);

                OV9650_1_I2C_state = OV9650_1_I2C_SM_IDLE;
            }
        }

        if(OV9650_1_I2C_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            /* The address only issued after Start or ReStart: so check the address
               to catch these events:
                FF : sets an address phase with a byte_complete interrupt trigger.
                UDB: sets an address phase immediately after Start or ReStart. */
            if(OV9650_1_I2C_CHECK_ADDRESS_STS(tmpCsr))
            {
            /* Check for software address detection */
            #if(OV9650_1_I2C_SW_ADRR_DECODE)
                tmp8 = OV9650_1_I2C_GET_SLAVE_ADDR(OV9650_1_I2C_DATA_REG);

                if(tmp8 == OV9650_1_I2C_slAddress)   /* Check for address match */
                {
                    if(0u != (OV9650_1_I2C_DATA_REG & OV9650_1_I2C_READ_FLAG))
                    {
                        /* Place code to prepare read buffer here                  */
                        /* `#START OV9650_1_I2C_SW_PREPARE_READ_BUF_interrupt` */

                        /* `#END` */

                        /* Prepare next operation to read, get data and place in data register */
                        if(OV9650_1_I2C_slRdBufIndex < OV9650_1_I2C_slRdBufSize)
                        {
                            /* Load first data byte from array */
                            OV9650_1_I2C_DATA_REG = OV9650_1_I2C_slRdBufPtr[OV9650_1_I2C_slRdBufIndex];
                            OV9650_1_I2C_ACK_AND_TRANSMIT;
                            OV9650_1_I2C_slRdBufIndex++;

                            OV9650_1_I2C_slStatus |= OV9650_1_I2C_SSTAT_RD_BUSY;
                        }
                        else    /* Overflow: provide 0xFF on bus */
                        {
                            OV9650_1_I2C_DATA_REG = OV9650_1_I2C_OVERFLOW_RETURN;
                            OV9650_1_I2C_ACK_AND_TRANSMIT;

                            OV9650_1_I2C_slStatus  |= (OV9650_1_I2C_SSTAT_RD_BUSY |
                                                           OV9650_1_I2C_SSTAT_RD_ERR_OVFL);
                        }

                        OV9650_1_I2C_state = OV9650_1_I2C_SM_SL_RD_DATA;
                    }
                    else  /* Write transaction: receive 1st byte */
                    {
                        OV9650_1_I2C_ACK_AND_RECEIVE;
                        OV9650_1_I2C_state = OV9650_1_I2C_SM_SL_WR_DATA;

                        OV9650_1_I2C_slStatus |= OV9650_1_I2C_SSTAT_WR_BUSY;
                        OV9650_1_I2C_ENABLE_INT_ON_STOP;
                    }
                }
                else
                {
                    /*     Place code to compare for additional address here    */
                    /* `#START OV9650_1_I2C_SW_ADDR_COMPARE_interruptStart` */

                    /* `#END` */

                    OV9650_1_I2C_NAK_AND_RECEIVE;   /* NACK address */

                    /* Place code to end of condition for NACK generation here */
                    /* `#START OV9650_1_I2C_SW_ADDR_COMPARE_interruptEnd`  */

                    /* `#END` */
                }

            #else /* (OV9650_1_I2C_HW_ADRR_DECODE) */

                if(0u != (OV9650_1_I2C_DATA_REG & OV9650_1_I2C_READ_FLAG))
                {
                    /* Place code to prepare read buffer here                  */
                    /* `#START OV9650_1_I2C_HW_PREPARE_READ_BUF_interrupt` */

                    /* `#END` */

                    /* Prepare next operation to read, get data and place in data register */
                    if(OV9650_1_I2C_slRdBufIndex < OV9650_1_I2C_slRdBufSize)
                    {
                        /* Load first data byte from array */
                        OV9650_1_I2C_DATA_REG = OV9650_1_I2C_slRdBufPtr[OV9650_1_I2C_slRdBufIndex];
                        OV9650_1_I2C_ACK_AND_TRANSMIT;
                        OV9650_1_I2C_slRdBufIndex++;

                        OV9650_1_I2C_slStatus |= OV9650_1_I2C_SSTAT_RD_BUSY;
                    }
                    else    /* Overflow: provide 0xFF on bus */
                    {
                        OV9650_1_I2C_DATA_REG = OV9650_1_I2C_OVERFLOW_RETURN;
                        OV9650_1_I2C_ACK_AND_TRANSMIT;

                        OV9650_1_I2C_slStatus  |= (OV9650_1_I2C_SSTAT_RD_BUSY |
                                                       OV9650_1_I2C_SSTAT_RD_ERR_OVFL);
                    }

                    OV9650_1_I2C_state = OV9650_1_I2C_SM_SL_RD_DATA;
                }
                else  /* Write transaction: receive 1st byte */
                {
                    OV9650_1_I2C_ACK_AND_RECEIVE;
                    OV9650_1_I2C_state = OV9650_1_I2C_SM_SL_WR_DATA;

                    OV9650_1_I2C_slStatus |= OV9650_1_I2C_SSTAT_WR_BUSY;
                    OV9650_1_I2C_ENABLE_INT_ON_STOP;
                }

            #endif /* (OV9650_1_I2C_SW_ADRR_DECODE) */
            }
            /* Data states */
            /* Data master writes into slave */
            else if(OV9650_1_I2C_state == OV9650_1_I2C_SM_SL_WR_DATA)
            {
                if(OV9650_1_I2C_slWrBufIndex < OV9650_1_I2C_slWrBufSize)
                {
                    tmp8 = OV9650_1_I2C_DATA_REG;
                    OV9650_1_I2C_ACK_AND_RECEIVE;
                    OV9650_1_I2C_slWrBufPtr[OV9650_1_I2C_slWrBufIndex] = tmp8;
                    OV9650_1_I2C_slWrBufIndex++;
                }
                else  /* of array: complete write, send NACK */
                {
                    OV9650_1_I2C_NAK_AND_RECEIVE;

                    OV9650_1_I2C_slStatus |= OV9650_1_I2C_SSTAT_WR_ERR_OVFL;
                }
            }
            /* Data master reads from slave */
            else if(OV9650_1_I2C_state == OV9650_1_I2C_SM_SL_RD_DATA)
            {
                if(OV9650_1_I2C_CHECK_DATA_ACK(tmpCsr))
                {
                    if(OV9650_1_I2C_slRdBufIndex < OV9650_1_I2C_slRdBufSize)
                    {
                         /* Get data from array */
                        OV9650_1_I2C_DATA_REG = OV9650_1_I2C_slRdBufPtr[OV9650_1_I2C_slRdBufIndex];
                        OV9650_1_I2C_TRANSMIT_DATA;
                        OV9650_1_I2C_slRdBufIndex++;
                    }
                    else   /* Overflow: provide 0xFF on bus */
                    {
                        OV9650_1_I2C_DATA_REG = OV9650_1_I2C_OVERFLOW_RETURN;
                        OV9650_1_I2C_TRANSMIT_DATA;

                        OV9650_1_I2C_slStatus |= OV9650_1_I2C_SSTAT_RD_ERR_OVFL;
                    }
                }
                else  /* Last byte was NACKed: read complete */
                {
                    /* Only NACK appears on bus */
                    OV9650_1_I2C_DATA_REG = OV9650_1_I2C_OVERFLOW_RETURN;
                    OV9650_1_I2C_NAK_AND_TRANSMIT;

                    OV9650_1_I2C_slStatus &= ((uint8) ~OV9650_1_I2C_SSTAT_RD_BUSY);
                    OV9650_1_I2C_slStatus |= ((uint8)  OV9650_1_I2C_SSTAT_RD_CMPLT);

                    OV9650_1_I2C_state = OV9650_1_I2C_SM_IDLE;
                }
            }
            else
            {
            #if(OV9650_1_I2C_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handle this case */
                OV9650_1_I2C_DisableInt();
                OV9650_1_I2C_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (OV9650_1_I2C_TIMEOUT_ENABLED) */
            }
        }
    #endif /* (OV9650_1_I2C_MODE_SLAVE_ENABLED) */
    }
    else
    {
        /* The FSM skips master and slave processing: return to IDLE */
        OV9650_1_I2C_state = OV9650_1_I2C_SM_IDLE;
    }
}


#if ((OV9650_1_I2C_FF_IMPLEMENTED) && (OV9650_1_I2C_WAKEUP_ENABLED))
    /*******************************************************************************
    * Function Name: OV9650_1_I2C_WAKEUP_ISR
    ********************************************************************************
    *
    * Summary:
    *  The interrupt handler to trigger after a wakeup.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(OV9650_1_I2C_WAKEUP_ISR)
    {
         /* Set flag to notify that matched address is received */
        OV9650_1_I2C_wakeupSource = 1u;

        /* SCL is stretched until the I2C_Wake() is called */
    }
#endif /* ((OV9650_1_I2C_FF_IMPLEMENTED) && (OV9650_1_I2C_WAKEUP_ENABLED)) */


/* [] END OF FILE */