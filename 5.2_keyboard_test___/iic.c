/*********************************************************************************************
* File£º	iic.c
* Author:	embest
* Desc£º	iic source code
* History:	
*********************************************************************************************/
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "iic.h"

/*------------------------------------------------------------------------------------------*/
/*	 								global variables					 				    */
/*------------------------------------------------------------------------------------------*/
int f_nGetACK;

/*------------------------------------------------------------------------------------------*/
/*	 								function declare					 				    */
/*------------------------------------------------------------------------------------------*/
void iic_int(void);
void iic_init(void);
void iic_write(UINT32T unSlaveAddr, UINT32T unAddr, UINT8T ucData);
void iic_read(UINT32T unSlaveAddr, UINT32T unAddr, UINT8T *pData);

/*********************************************************************************************
* name:		iic_init
* func:		initialize iic
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void iic_init(void)
{
	f_nGetACK = 0;

    // Enable interrupt
	rINTMOD = 0x0;
	rINTCON = 0x1;
	rINTMSK = rINTMSK & (~(BIT_GLOBAL|BIT_IIC));
    pISR_IIC= (unsigned)iic_int;

	// Initialize iic
	rIICADD = 0x10;						// S3C44B0X slave address 
    rIICCON = 0xe5;						// Enable ACK, interrupt, IICCLK=MCLK/512, Enable ACK//64Mhz/512/(15+1) = 8Khz
    rIICSTAT= 0x10;						// Enable TX/RX 
}

/*********************************************************************************************
* name:		iic_write
* func:		write data to iic
* para:		unSlaveAddr --- input, chip slave address
*			unAddr		--- input, data address
*			ucData    	--- input, data value
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void iic_write(UINT32T unSlaveAddr,UINT32T unAddr,UINT8T ucData)
{
	f_nGetACK = 0;
    
    // Send control byte
    rIICDS = unSlaveAddr;			// 0xa0
    rIICSTAT = 0xf0; 				// Master Tx,Start
    while(f_nGetACK == 0);			// Wait ACK
    f_nGetACK = 0;
    
    // Send address
    rIICDS = unAddr;
    rIICCON = 0xe5;   				// Resumes IIC operation.
	while(f_nGetACK == 0);			// Wait ACK
    f_nGetACK = 0;
    
    // Send data
    rIICDS = ucData;
    rIICCON = 0xe5;   				// Resumes IIC operation.
    while(f_nGetACK == 0);			// Wait ACK
    f_nGetACK = 0;
    
    // End send
    rIICSTAT = 0xd0;				// Stop Master Tx condition
    rIICCON = 0xe5;					// Resumes IIC operation.
	while(rIICSTAT & 0x20 == 1);	// Wait until stop condtion is in effect.
}
	
/*********************************************************************************************
* name:		iic_read
* func:		read data from iic
* para:		unSlaveAddr --- input, chip slave address
*			unAddr		--- input, data address
*			pData    	--- output, data pointer
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void iic_read(UINT32T unSlaveAddr,UINT32T unAddr,UINT8T *pData)
{
	char cRecvByte;
	
	f_nGetACK = 0;

    // Send control byte 
    rIICDS = unSlaveAddr;			// Write slave address to IICDS
    rIICSTAT = 0xf0; 				// Master Tx,Start
    while(f_nGetACK == 0);			// Wait ACK
    f_nGetACK = 0;

    // Send address 
    rIICDS = unAddr;
    rIICCON = 0xe5;   				// Resumes IIC operation.
    while(f_nGetACK == 0);			// Wait ACK
    f_nGetACK = 0;

    // Send control byte
    rIICDS = unSlaveAddr;			// 0xa0
    rIICSTAT = 0xb0; 				// Master Rx,Start
	rIICCON = 0xe5;  				// Resumes IIC operation.   
    while(f_nGetACK == 0);			// Wait ACK
    f_nGetACK = 0;
    
    // Get data
    rIICCON = 0x65;
    while(f_nGetACK == 0);			// Wait ACK
    f_nGetACK = 0;
    cRecvByte = rIICDS;
    
    /* End receive */
    rIICSTAT = 0x90;  				// Stop Master Rx condition 
	rIICCON = 0xe5;   				// Resumes IIC operation.
	while(rIICSTAT & 0x20 == 1);	// Wait until stop condtion is in effect.
	
    *pData = cRecvByte;
}

/*********************************************************************************************
* name:		iic_int
* func:		IIC interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void iic_int(void)
{
    rI_ISPC=BIT_IIC;
	f_nGetACK = 1;
}
