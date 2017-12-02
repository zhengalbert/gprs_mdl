/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_BufferManage.c
 Description    : ������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#include  "MC55_BufferManage.h"
#include  "MC55_module.h"

#define DEBUG_COM	COM0             		//uart0��Ϊ��ؿ�   
U8 debug_out_buf[CON_SCI_DEBUG_BUFFER_DEPTH];
                

/********************************************************************
  ��������: SCIBufferInit
  ����  : ����ͨ�Ż�������ʼ��
  ��ڲ���:
  ���ڲ���:
*********************************************************************/

void SCIBufferInit(void)
{
  //GR47���ͻ�������ʼ��
    SCIBufferClearAll(&stAT_TX_BUFFER);
    stAT_TX_BUFFER.usMaxDepth = CON_GR47_TX_BUFFER_DEPTH;
    stAT_TX_BUFFER.pBuf = NULL;//azh ������ͻ��岻��ucGr47_TX_Buffer;

  //GR47���ջ�������ʼ��
  SCIBufferClearAll(&stAT_RX_BUFFER);
  stAT_RX_BUFFER.usMaxDepth = CON_GR47_RX_BUFFER_DEPTH;
  stAT_RX_BUFFER.pBuf = ucGr47_RX_Buffer;

  SCIBufferClearAll(&stDebOut_TX_Handle.stSendBuf);
  stDebOut_TX_Handle.dly = AT_DLY_500MS;
  stDebOut_TX_Handle.stSendBuf.usMaxDepth = CON_SCI_DEBUG_BUFFER_DEPTH;
  stDebOut_TX_Handle.stSendBuf.pBuf = debug_out_buf;

  gucAtCmd_WatchOut_Flag = 0;//CON_ATCMD_OUT_ENABLE;//0;
}

/********************************************************************
  ��������: GetBuferLength
  ����  : ��ȡ����������
  ��ڲ���: pSciBuffer  ������ָ��
  ���ڲ���: ���������ݳ���
*********************************************************************/

unsigned short SCIBufferGetDataLength(PST_SCI_BUFFER pSciBuffer)
{
  unsigned short  usLength[5];
  unsigned short  usLen   =  0;
  unsigned char i,j;//cfh 10-04-14
  for(j=0;j<5;j++)
  {
    for(i=0;i<2;i++)
    {
      if(pSciBuffer->usTailPtr >= pSciBuffer->usHeadPtr)
        usLength[i] = pSciBuffer->usTailPtr - pSciBuffer->usHeadPtr;
      else
        usLength[i] = pSciBuffer->usTailPtr + pSciBuffer->usMaxDepth -
          pSciBuffer->usHeadPtr;
    }
    if(usLength[0] == usLength[1])
    {
      usLen = usLength[0];
      return usLen;
    }
  }
  return usLen;
}

unsigned short SCIBufferGetSpareLength(PST_SCI_BUFFER pSciBuffer)
{
  unsigned short  usLength = 0;

  if(pSciBuffer->usTailPtr >= pSciBuffer->usHeadPtr)
    usLength = pSciBuffer->usMaxDepth -1 - (pSciBuffer->usTailPtr - pSciBuffer->usHeadPtr);
  else
    usLength = pSciBuffer->usHeadPtr - pSciBuffer->usTailPtr -1 ;
  return usLength;
}

/********************************************************************
  ��������: ReadSCIBuferData
  ����  : ��ȡ���������ݣ���ͷָ�벻�ƶ�
  ��ڲ���: pSciBuffer  ������ָ��
        pDstBuf   ��ȡ���ݴ�Ż�����ָ��
        usLen   ��ȡ����
  ���ڲ���: ʵ�ʶ�ȡ�����ݳ���
*********************************************************************/

unsigned short SCIBufferRead(PST_SCI_BUFFER pSciBuffer ,  unsigned short ucOffset , unsigned char* pDstBuf , unsigned short usLen)
{
  unsigned short  usDataLength = 0;
  unsigned short  usCount = 0;
  unsigned short  i = 0;
  unsigned short  j = 0;

  usDataLength = SCIBufferGetDataLength(pSciBuffer);
  if(usDataLength<usLen+ucOffset)
    return 0;

  if(usDataLength<usLen)
  {
    usCount = usDataLength;
  }
  else
    usCount = usLen;

  if (pSciBuffer->usHeadPtr+ucOffset>pSciBuffer->usMaxDepth)
    i=pSciBuffer->usHeadPtr+ucOffset-pSciBuffer->usMaxDepth;
  else
    i = pSciBuffer->usHeadPtr+ucOffset;

  for( j = 0 ; j<usCount ; i++ , j++)
  {
    if(i >= pSciBuffer->usMaxDepth)
      i = 0;

    pDstBuf[j] = pSciBuffer->pBuf[i];
  }

  return usCount;
}
/********************************************************************
  ��������: SCIBufferGetByte
  ����  : �ӻ��������ݶ�ȡһ���ֽڣ�ͷָ���ƶ�
  ��ڲ���: pSciBuffer  ������ָ��
        pch     ���ݴ��ָ��
  ���ڲ���: ʵ�ʶ�ȡ�����ݳ���
*********************************************************************/
//
//unsigned char SCIBufferGetByte(PST_SCI_BUFFER pSciBuffer, unsigned char* pch)
//{
//  unsigned short  usDataLength = 0;
//
//  usDataLength = SCIBufferGetDataLength(pSciBuffer);
//  if(usDataLength == 0 )
//    return 0;
//
//  *pch = pSciBuffer->pBuf[pSciBuffer->usHeadPtr];
//
//  pSciBuffer->usHeadPtr++;
//
//  if(pSciBuffer->usHeadPtr >= pSciBuffer->usMaxDepth)
//    pSciBuffer->usHeadPtr = 0;
//
//  return 1;
//}

/********************************************************************
  ��������: SCIBufferReadByte
  ����  : �ӻ��������ݶ�ȡһ���ֽڣ�ͷָ�벻�ƶ��ƶ�
  ��ڲ���: pSciBuffer  ������ָ��
        ucOffset  ��ȡ��ƫ����
        pch     ���ݴ��ָ��
  ���ڲ���: ʵ�ʶ�ȡ�����ݳ���
*********************************************************************/
//
//unsigned char SCIBufferReadByte(PST_SCI_BUFFER pSciBuffer, unsigned short ucOffset , unsigned char* pch)
//{
//  unsigned short  usDataLength = 0;
//
//  usDataLength = SCIBufferGetDataLength(pSciBuffer);
//  if((usDataLength == 0) || (ucOffset > usDataLength) )
//    return 0;
//
//  ucOffset += pSciBuffer->usHeadPtr;
//  if(ucOffset >= pSciBuffer->usMaxDepth)
//    ucOffset -= pSciBuffer->usMaxDepth;
//
//  *pch = pSciBuffer->pBuf[pSciBuffer->usHeadPtr];
//  return 1;
//}

/********************************************************************
  ��������: ShiftSCIBuferHeadPtr
  ����  : �ƶ�������ͷָ�롣���ݶ�ȡ��ȷ����Ҫȡ����ͷָ�����

  ��ڲ���: pSciBuffer  ������ָ��
        usLen   �ƶ�����

  ���ڲ���: 1     uslen�ڻ��������ȷ�Χ��
        0     uslen��������������

  ����  : unsigned short���ͱ����ķ�Χ��0��65535��ͷָ������ƶ�
        ���Ⱥ��ܳ���65535,�������,�����ִ�����ֵ
*********************************************************************/
unsigned char SCIBufferMoveHeadPtr(PST_SCI_BUFFER pSciBuffer , unsigned short usLen)
{
  if( usLen > SCIBufferGetDataLength(pSciBuffer) )
    return 0;

  pSciBuffer->usHeadPtr += usLen;
  if(pSciBuffer->usHeadPtr >= pSciBuffer->usMaxDepth)
    pSciBuffer->usHeadPtr -= pSciBuffer->usMaxDepth;

  return 1;
}
/********************************************************************
  ��������: WriteSCIBuffer
  ����  : ���ڻ�����д�롣ʣ��ռ�С��д�볤�ȣ��򲻽���д�������
  ��ڲ���: pSciBuffer    ������ָ��
  ���ڲ���: д�볤�Ȼ򻺳���ʣ��ռ�
*********************************************************************/
unsigned long ulErrTest = 0;
unsigned short SCIBufferWrite(PST_SCI_BUFFER pSciBuffer , unsigned char* pSrcBuf , unsigned short usLen)
{
  //unsigned short  usDataLength = 0;
  unsigned short  usCount = 0;
  unsigned short  j = 0;

  usCount = SCIBufferGetSpareLength(pSciBuffer);//������ʣ��ռ�

  if(usCount >= usLen)
  {
    usCount = usLen;

    for(j = 0 ; j<usCount ; j++)
    {
      pSciBuffer->pBuf[pSciBuffer->usTailPtr] = pSrcBuf[j];
      pSciBuffer->usTailPtr++;
      if(pSciBuffer->usTailPtr >= pSciBuffer->usMaxDepth)
        pSciBuffer->usTailPtr = 0;
    }
  }
  else
  {
    ulErrTest ++;
//    DebugPrint((unsigned char*)STR_DEBUG_RECEIVE_BUFFER_Full, sizeof(STR_DEBUG_RECEIVE_BUFFER_Full));
    usCount = 0;

  }
    return usCount;
}

void SCIBufferClearAll(PST_SCI_BUFFER pSciBuffer)
{
  pSciBuffer->usHeadPtr = pSciBuffer->usTailPtr = 0;

}

/********************************************************************
  ��������: Gprs_send_frame
  ����  : ��Gprs���ڷ��� AT�����������
  ��ڲ���: pSciBuffer    ������ָ��
  ���ڲ���: д�볤�Ȼ򻺳���ʣ��ռ�
*********************************************************************/
void Gprs_send_frame(unsigned char* pstr, unsigned short usLen)
{
  unsigned char mem_type;
  unsigned char *send_buf;

  if(usLen < 230)
  {
    send_buf = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR31);
    if(send_buf != NULL)
    {
      memcpy(&send_buf[DATA_OFFSET], pstr, usLen);
      send_protocol_frame(send_buf, GPRS_CHL, usLen, MEM_256_TYPE);
    }
    else
    {
//          memcpy(gusCurMemErrFunName, "Gprs_se1", 8);
          setmemgetnullid(GET_MEM_ERR31, gusCurMemErrFunName);
    }
  }
  else
  {
    send_buf = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR1);
    if(send_buf != NULL)
    {
      memcpy(&send_buf[DATA_OFFSET], pstr, usLen);
      send_protocol_frame(send_buf, GPRS_CHL, usLen, MEM_1536_TYPE);
    }
    else
    {
//          memcpy(gusCurMemErrFunName, "Gprs_se2", 8);
          setmemgetnullid(GET_MEM_ERR1, gusCurMemErrFunName);
    }
  }

}

/********************************************************************
  ��������: debug_come_send_frame
  ����  : ��Gprs���ڷ��� AT�����������
  ��ڲ���: pSciBuffer    ������ָ��
  ���ڲ���: д�볤�Ȼ򻺳���ʣ��ռ�
*********************************************************************/
/*
U8 debug_uart_err;

void debug_come_send_frame(unsigned char* pstr, unsigned short usLen)
{
  unsigned char *send_buf;

  send_buf = NULL;

  if(usLen < 240)
  {
    send_buf = _alloc_box(Mem_256_Byte);
    if(send_buf != NULL)
    {
      debug_uart_err = 0;

      memcpy(&send_buf[DATA_OFFSET], pstr, usLen);
      send_protocol_frame(send_buf, COM0, usLen, MEM_256_TYPE);
    }
    else
    {
      debug_uart_err = 1;
    }
  }
  else
  {
    send_buf = _alloc_box(Mem_1536_Byte);
    if(send_buf != NULL)
    {
      memcpy(&send_buf[DATA_OFFSET], pstr, usLen);
      send_protocol_frame(send_buf, COM0, usLen, MEM_1536_TYPE);
    }
  }

}
*/
void debug_come_send_frame(unsigned char* pstr, unsigned short usLen)
{
  unsigned char mem_type;
  unsigned char *send_buf;
  unsigned short len;

  send_buf = NULL;

  if(usLen > (CON_SCI_DEBUG_BUFFER_DEPTH - DATA_OFFSET))
  {
    return;
  }
  len = SCIBufferGetDataLength(&stDebOut_TX_Handle.stSendBuf);
  if((len + usLen + DATA_OFFSET) > CON_SCI_DEBUG_BUFFER_DEPTH)
  {
    send_buf = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR2);
    if(send_buf == NULL)
    {
//          memcpy(gusCurMemErrFunName, "debug_co", 8);
          setmemgetnullid(GET_MEM_ERR2, gusCurMemErrFunName);
      return;
    }
    SCIBufferRead(&stDebOut_TX_Handle.stSendBuf ,0, send_buf+DATA_OFFSET ,len);
    SCIBufferMoveHeadPtr(&stDebOut_TX_Handle.stSendBuf , len);
    send_protocol_frame(send_buf, COM0, len, MEM_256_TYPE);//azh 131024 COM0
  }
  SCIBufferWrite(&stDebOut_TX_Handle.stSendBuf ,pstr, usLen);
  stDebOut_TX_Handle.dly = AT_DLY_500MS;//500ms

}
/********************************************************************
  ��������: debug_handle_interval
  ����  : ��Gprs���ڷ��� AT�����������
  ��ڲ���: pSciBuffer    ������ָ��
  ���ڲ���: д�볤�Ȼ򻺳���ʣ��ռ�
*********************************************************************/
void debug_handle_interval(void)
{
  unsigned char mem_type;
  unsigned char *send_buf;
  unsigned short len;

    if(gucAtCmd_WatchOut_Flag != CON_ATCMD_OUT_ENABLE)//AT�����عر�
    {
        return;
    }
  send_buf = NULL;

  if(stDebOut_TX_Handle.dly > AT_DLY_500MS)
  {
    stDebOut_TX_Handle.dly = AT_DLY_500MS;
  }
  if(stDebOut_TX_Handle.dly > AT_DLY_500MS)
  {
    stDebOut_TX_Handle.dly--;
  }
  else
  {
    len = SCIBufferGetDataLength(&stDebOut_TX_Handle.stSendBuf);
    if(len == 0)
    {
      return;
    }
    if( (len + DATA_OFFSET) > CON_SCI_DEBUG_BUFFER_DEPTH )
    {
      return;
    }
    send_buf = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR3);
    if(send_buf == NULL)
    {
//          memcpy(gusCurMemErrFunName, "debug_ha", 8);
          setmemgetnullid(GET_MEM_ERR3, gusCurMemErrFunName);
      return;
    }
    SCIBufferRead(&stDebOut_TX_Handle.stSendBuf ,0, send_buf+DATA_OFFSET ,len);
    SCIBufferMoveHeadPtr(&stDebOut_TX_Handle.stSendBuf , len);
    send_protocol_frame(send_buf, COM0, len, MEM_256_TYPE);
  }
}
