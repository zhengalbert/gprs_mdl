STATUS dwT_ESAMSend(const UINT8 ucCLA, const UINT8 ucINS, const UINT8 ucP1, const UINT8 ucP2, const UINT16 uwLen, const UINT8 *pucData)
{
	#if 0//chenchao
	UINT8 ucBuf[8], ucLRC1;
	UINT32 i;

	if ((pucData == NULL) && (uwLen != 0))
		{
		return ERROR;
		}
/*
	//调整SPI接口，主要是CPOL与计量芯片不同
	taskLock();
	*PADAT = CLEAR_WORD_BIT(*PADAT, BIT7);
	vNOP(40);
	taskUnlock();
	dwT_ESAM_Read(ucBuf, 8);
	taskLock();
	vNOP(40);
	*PADAT = SET_WORD_BIT(*PADAT, BIT7);
	taskUnlock();

	taskDelay(1);
*/
	//组帧
	ucBuf[0] = 0x55;	//55为发送命令结构的命令头
	ucBuf[1] = ucCLA;	//命令类别
	ucBuf[2] = ucINS;	//命令类别中的指令代码
	ucBuf[3] = ucP1;	//完成指令代码的参考符号
	ucBuf[4] = ucP2;	//完成指令代码的参考符号
	ucBuf[5] = GET_HI_BYTE(uwLen);	//后续DATA的长度
	ucBuf[6] = GET_LO_BYTE(uwLen);
	
	/*
	//计算LRC1 
	ucLRC1 = ucBuf[1];	//从CLA开始
	for (i=2; i<7; i++)
		{
		ucLRC1 = ucLRC1 ^ ucBuf[i];
		}
	if (pucData != NULL)
		{
		for (i=0; i<uwLen; i++)
			{
			ucLRC1 = ~(ucLRC1 ^ pucData[i]);
			}
		}
	ucLRC1 = ~ucLRC1;
	*/

	//计算LRC1 
	ucLRC1 = ucBuf[1];	//从CLA开始
	for (i=2; i<7; i++)
		{
		ucLRC1 = ucLRC1 ^ ucBuf[i];
		}
	if (pucData != NULL)
		{
		for (i=0; i<uwLen; i++)
			{
			ucLRC1 = ucLRC1 ^ pucData[i];
			}
		}
	ucLRC1 = ~ucLRC1;
	
	taskLock();
	*PADAT = CLEAR_WORD_BIT(*PADAT, BIT7);
	vNOP(40);
	taskUnlock();

	//发送帧头，0x55到Len2，共7字节
	if (dwT_ESAM_Write(ucBuf, 7) != 7)
		{
		taskLock();
		*PADAT = SET_WORD_BIT(*PADAT, BIT7);
		taskUnlock();
		return ERROR;
		}
	//发送完Len2后，需要等待100us，再发送DATA
	taskLock();
	vNOP(80);
	taskUnlock();
	if ((pucData != NULL) && (uwLen != 0))
		{
		if (dwT_ESAM_Write(pucData, uwLen) != uwLen)
			{
			taskLock();
			*PADAT = SET_WORD_BIT(*PADAT, BIT7);
			taskUnlock();
			return ERROR;
			}
		}
	//发送LRC1
	if (dwT_ESAM_Write(&ucLRC1, sizeof(ucLRC1)) != sizeof(ucLRC1))
		{
		taskLock();
		*PADAT = SET_WORD_BIT(*PADAT, BIT7);
		taskUnlock();
		return ERROR;
		}

	taskLock();
	vNOP(40);
	*PADAT = SET_WORD_BIT(*PADAT, BIT7);
	taskUnlock();

	return OK;
	#endif //chenchao 
	
	unsigned int i;
	unsigned char ucLRC1;
	unsigned char ucBuf[8];
	STATUS dwStatus;

	if( ( pucData == NULL ) && ( uwLen != 0 ) )
	{
		return ERROR;
	}
	
	ucBuf[0] = 0x55;	//55为发送命令结构的命令头
	ucBuf[1] = ucCLA;	//命令类别
	ucBuf[2] = ucINS;	//命令类别中的指令代码
	ucBuf[3] = ucP1;	//完成指令代码的参考符号
	ucBuf[4] = ucP2;	//完成指令代码的参考符号
	ucBuf[5] = GET_HI_BYTE(uwLen );	//后续DATA的长度
	ucBuf[6] = GET_LO_BYTE(uwLen );

	ucLRC1 = ucBuf[1];	//从CLA开始
	for (i=2; i<7; i++)
	{
		ucLRC1 = ucLRC1 ^ ucBuf[i];
	}

	if (pucData != NULL)
	{
		for (i=0; i<uwLen; i++)
		{
			ucLRC1 = ucLRC1 ^ pucData[i];
		}
	}
	ucLRC1 = ~ucLRC1;

	ioctl(dwESAMFD,CMD_ESAM_CS,0);
	usleep(60);
	dwStatus = write( dwESAMFD, ucBuf, 7 );
	if(dwStatus == -1)
	{
		printf("write ERROR1\n");
		ioctl(dwESAMFD,CMD_ESAM_CS,1);
		usleep(20);
		return ERROR;
	}
	usleep(50);//手册注明
	if( ( uwLen != 0 ) && ( pucData != NULL ) )
	{
		   dwStatus = write( dwESAMFD, pucData , uwLen );
		   if(dwStatus == -1)
		  {
		 	 printf("write ERROR2\n");
			ioctl(dwESAMFD,CMD_ESAM_CS,1);
			usleep(20);
			return ERROR;
		  }
	}
	//usleep(1);
	dwStatus = write( dwESAMFD, &ucLRC1, 1 );
	if(dwStatus == -1)
	 {
	 	printf("write ERROR3\n");
		ioctl(dwESAMFD,CMD_ESAM_CS,1);
		usleep(20);
		return ERROR;
	 }
	
	//usleep(50);
	ioctl(dwESAMFD,CMD_ESAM_CS,1);
	usleep(20);

   	 return OK;
}
