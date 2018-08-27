# Eventlogedit-evt--General
Remove individual lines from Windows Event Viewer Log (EVT) files

Support: Windows XP and Windows 2003

My posts about the details:

[1. Windows Event Viewer Log (EVT)单条日志清除（一）——删除思路与实例](https://3gstudent.github.io/3gstudent.github.io/Windows-Event-Viewer-Log-(EVT)%E5%8D%95%E6%9D%A1%E6%97%A5%E5%BF%97%E6%B8%85%E9%99%A4-%E4%B8%80-%E5%88%A0%E9%99%A4%E6%80%9D%E8%B7%AF%E4%B8%8E%E5%AE%9E%E4%BE%8B/)

[2. Windows Event Viewer Log (EVT)单条日志清除（二）——程序实现删除evt文件指定时间段的日志记录](https://3gstudent.github.io/3gstudent.github.io/Windows-Event-Viewer-Log-(EVT)%E5%8D%95%E6%9D%A1%E6%97%A5%E5%BF%97%E6%B8%85%E9%99%A4-%E4%BA%8C-%E7%A8%8B%E5%BA%8F%E5%AE%9E%E7%8E%B0%E5%88%A0%E9%99%A4evt%E6%96%87%E4%BB%B6%E6%8C%87%E5%AE%9A%E6%97%B6%E9%97%B4%E6%AE%B5%E7%9A%84%E6%97%A5%E5%BF%97%E8%AE%B0%E5%BD%95/)

3. Windows Event Viewer Log (EVT)单条日志清除（三）——删除当前系统指定指定时间段evt日志记录

Later I'll translate them into English.

Note:

- WinXP and Win7,ObjectTypeNumber = 0x1c

---

### evtDeleteRecordofFile.cpp

Read an evt file(c:\test\sys1.evt),then delete some eventlog records(from StartTime = "2018-7-16 17:46:17" to EndTime = "2018-7-16 17:46:40").

The new evt files are saved as `c:\test\sys2.evt` and `c:\test\sys3.evt`(different in file size).

### sys1.evt

Number of events:9

### sys2.evt

Number of events:8

You can use evtDeleteRecordofFile.cpp to delete an eventlog record of sys1.evt.

---

### evtDeleteRecordbyGetHandle.cpp

Get specified .evt file's handle and delete some eventlog records.

Test fails,just an example.

### evtQueryRecordbyGetHandle.cpp

Get specified .evt file's handle and query the eventlog record's information.

### evtModifyRecordbyGetHandle.cpp

Get specified .evt file's handle and modify the eventlog record's information.


