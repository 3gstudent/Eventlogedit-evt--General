#include <windows.h>
#include <time.h>

#pragma pack(1)

typedef struct _EVENTLOGHEADER {
	ULONG HeaderSize;
	ULONG Signature;
	ULONG MajorVersion;
	ULONG MinorVersion;
	ULONG StartOffset;
	ULONG EndOffset;
	ULONG CurrentRecordNumber;
	ULONG OldestRecordNumber;
	ULONG MaxSize;
	ULONG Flags;
	ULONG Retention;
	ULONG EndHeaderSize;
} EVENTLOGHEADER, *PEVENTLOGHEADER;

typedef struct _EVTLOGRECORD {
	DWORD Length;
	DWORD Reserved;
	DWORD RecordNumber;
	DWORD TimeGenerated;
	DWORD TimeWritten;
	DWORD EventID;
	WORD  EventType;
	WORD  NumStrings;
	WORD  EventCategory;
	WORD  ReservedFlags;
	DWORD ClosingRecordNumber;
	DWORD StringOffset;
	DWORD UserSidLength;
	DWORD UserSidOffset;
	DWORD DataLength;
	DWORD DataOffset;
} EVTLOGRECORD, *PEVTLOGRECORD;

typedef struct _EVENTLOGEOF {
	ULONG RecordSizeBeginning;
	ULONG One;
	ULONG Two;
	ULONG Three;
	ULONG Four;
	ULONG BeginRecord;
	ULONG EndRecord;
	ULONG CurrentRecordNumber;
	ULONG OldestRecordNumber;
	ULONG RecordSizeEnd;
} EVENTLOGEOF, *PEVENTLOGEOF;

#pragma pack()

int Newlen = 0;

time_t StringToDatetime(char *str)
{
	tm tm_;
	int year, month, day, hour, minute, second;
	sscanf_s(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour - 1;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t t_ = mktime(&tm_);
	return t_;
}

unsigned char *DeleteRecord(PVOID mapAddress, int len, char *StartTime, char *EndTime)
{
	unsigned char *NewBuf = new unsigned char[len];
	memset(NewBuf, 0, len);

	DWORD TempLen = 0x30;
	int flag = 0;

	time_t StartTimeNum = StringToDatetime(StartTime);
	time_t EndTimeNum = StringToDatetime(EndTime);

	PEVENTLOGHEADER evtFilePtr = (PEVENTLOGHEADER)mapAddress;
	PEVTLOGRECORD currentRecordPtr = (PEVTLOGRECORD)((PBYTE)evtFilePtr + 0x30);
	PEVTLOGRECORD nextRecordPtr = (PEVTLOGRECORD)((PBYTE)currentRecordPtr + currentRecordPtr->Length);

	//copy file header
	memcpy(NewBuf, mapAddress, 0x30);

	while (currentRecordPtr->TimeGenerated != 0x33333333)
	{
		if (currentRecordPtr->TimeGenerated<StartTimeNum || currentRecordPtr->TimeGenerated>EndTimeNum)
		{		
			//after delete,change the RecordNumber
			currentRecordPtr->RecordNumber = currentRecordPtr->RecordNumber - flag;
			//not selected evt record,copy it
			memcpy(NewBuf + TempLen, currentRecordPtr, currentRecordPtr->Length);
			TempLen = TempLen + currentRecordPtr->Length;
		}
		else
		{
			printf("DeleteRecordNum:%d\n", currentRecordPtr->RecordNumber);
			flag++;
		}
		currentRecordPtr = nextRecordPtr;
		nextRecordPtr = (PEVTLOGRECORD)((PBYTE)nextRecordPtr + nextRecordPtr->Length);
	}

	PEVENTLOGEOF evtEOFPtr = (PEVENTLOGEOF)((PBYTE)currentRecordPtr);	
	//copy eof
	memcpy(NewBuf + TempLen, evtEOFPtr, 0x28);

	//reset file header
	evtFilePtr->EndOffset = TempLen;
	evtFilePtr->CurrentRecordNumber = evtFilePtr->CurrentRecordNumber - flag;
	memcpy(NewBuf, mapAddress, 0x30);

	//reset eof
	evtEOFPtr->EndRecord = TempLen;
	evtEOFPtr->CurrentRecordNumber = evtFilePtr->CurrentRecordNumber;
	memcpy(NewBuf + TempLen, evtEOFPtr, 0x28);

	Newlen = TempLen + 0x28;
	return NewBuf;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char *StartTime = "2018-7-16 17:46:17";
	char *EndTime = "2018-7-16 17:46:40";

	FILE* fp;
	int err = fopen_s(&fp, "c:\\test\\sys1.evt", "a+");
	if (err != 0)
	{
		printf("openfile error!");
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	unsigned char *buf = new unsigned char[len];
	fseek(fp, 0, SEEK_SET);
	fread(buf, len, 1, fp);
	fclose(fp);

	unsigned char *NewBuf = new unsigned char[len];
	memset(NewBuf, 0, len);
	NewBuf = DeleteRecord(buf,len,StartTime,EndTime);
	
	FILE* fp2;
	err = fopen_s(&fp2, "c:\\test\\sys2.evt", "wb+");
	if (err != 0)
	{
		printf("createfile error!");
		return 0;
	}
	fwrite(NewBuf, len, 1, fp2);
	fclose(fp2);

	FILE* fp3;
	err = fopen_s(&fp3, "c:\\test\\sys3.evt", "wb+");
	if (err != 0)
	{
		printf("createfile error!");
		return 0;
	}
	fwrite(NewBuf, Newlen, 1, fp3);
	fclose(fp3);

	return 0;
}