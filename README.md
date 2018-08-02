# Eventlogedit-evt--General
Remove individual lines from Windows Event Viewer Log (EVT) files

Support: Windows XP and Windows 2003

My posts about the details:

1. Windows Event Viewer Log (EVT)单条日志清除（一）——删除思路与实例
2. Windows Event Viewer Log (EVT)单条日志清除（二）——程序实现删除evt文件指定时间段的日志记录
3. Windows Event Viewer Log (EVT)单条日志清除（三）——删除当前系统指定指定时间段evt日志记录

Later I'll translate them into English.

Note:

- WinXP and Win7,ObjectTypeNumber = 0x1c

---

### evtDeleteRecordofFile.cpp

Read an evt file(c:\test\sys1.evt),then delete some eventlog records(from StartTime = "2018-7-16 17:46:17" to EndTime = "2018-7-16 17:46:40").

The new evt files are saved as `c:\test\sys2.evt` and `c:\test\sys3.evt`(different in file size).

### sys1.evt


### sys2.evt


---

### evtDeleteRecordbyGetHandle.cpp

Get specified .evt file's handle and delete some eventlog records.

Test fails,just an example.

### evtQueryRecordbyGetHandle.cpp

Get specified .evt file's handle and query the eventlog record's information.

### evtModifyRecordbyGetHandle.cpp

Get specified .evt file's handle and modify the eventlog record's information.


