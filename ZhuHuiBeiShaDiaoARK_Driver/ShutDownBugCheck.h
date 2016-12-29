#pragma once
#include <ntddk.h>

/*
���ߣ�Tesla.Angela


�ػ��ص��������ص������ڹػ�ǰ������ǰִ�еĻص������ܱ�ĳЩ����ľ�����ڹػ�ǰ������ǰ��д������ARK����Ҳ�ṩö�ٺ�ɾ����Щ�ص��Ĺ��ܡ�
�ػ��ص�ֻ��1�֣�ShutdownNotification�������������ص���2�֣�BugCheckCallback��BugCheckReasonCallback��
��2�֣�����˵3�֣��ص������Ƶĵط�����������һ���ṹ��������ҵ����Ե�����ͷ������ö�ٳ����еĹػ��������ص���
�����ػ��ص���Ϊ���⣺���Ļص���ַ���Ǽ�¼��ϵͳ�Ľṹ����������Ǽ�¼�ڸ��������ķַ����̣�IRP_MJ_SHUTDOWN���
�����ö��CreateProcess��CreateThread��LoadImage�Ȼص���ͬС�졣*/


ULONG64 FindIopNotifyShutdownQueueHead()
{
	ULONG64			i = 0, pCheckArea = 0;
	UNICODE_STRING	unstrFunc;
	RtlInitUnicodeString(&unstrFunc, L"IoRegisterShutdownNotification");
	pCheckArea = (ULONG64)MmGetSystemRoutineAddress(&unstrFunc);
	//DbgPrint("IoRegisterShutdownNotification: %llx\n", pCheckArea);
	for (i = pCheckArea; i<pCheckArea + 0xff; i++)
	{
		if (*(PUCHAR)i == 0x48 && *(PUCHAR)(i + 1) == 0x8d && *(PUCHAR)(i + 2) == 0x0d)	//lea rcx,xxxx
		{
			LONG OffsetAddr = 0;
			memcpy(&OffsetAddr, (PUCHAR)(i + 3), 4);
			return OffsetAddr + 7 + i;
		}
	}
	return 0;
}

PNOTIFY_INFO EnumShutdownCallback()
{
	PDEVICE_OBJECT DevObj = NULL;
	PDRIVER_OBJECT DrvObj = NULL;
	PLIST_ENTRY g_IopNotifyShutdownQueueHead = NULL;
	PLIST_ENTRY CurrEntry = NULL;
	ULONG64 Dispatch = 0;
	ULONG	i = 0;
	PNOTIFY_INFO pNotify = NULL;
	SYSTEM_MODULE Sysmodule = { 0 };
	ULONG64 IopNotifyShutdownQueueHead = FindIopNotifyShutdownQueueHead();

	pNotify = ExAllocatePool(NonPagedPool, sizeof(NOTIFY_INFO) * 100);

	if (pNotify == NULL)
		return NULL;


	//DbgPrint("IopNotifyShutdownQueueHead: %llx\n", IopNotifyShutdownQueueHead);
	g_IopNotifyShutdownQueueHead = (PLIST_ENTRY)IopNotifyShutdownQueueHead;
	//DbgPrint("Callback\t\tDrvName\t\tDevObj\n");
	for (CurrEntry = g_IopNotifyShutdownQueueHead->Flink;
		CurrEntry != g_IopNotifyShutdownQueueHead;
		CurrEntry = CurrEntry->Flink)
	{
		DevObj = (PDEVICE_OBJECT)(*(PULONG64)((ULONG64)CurrEntry + sizeof(LIST_ENTRY)));
		DrvObj = DevObj->DriverObject;
		Dispatch = (ULONG64)(DrvObj->MajorFunction[IRP_MJ_SHUTDOWN]);
		//DbgPrint("[shutdown]0x%X\t\t%wZ\t\t0x%X\n",Dispatch,&DrvObj->DriverName,DevObj);
		//DbgPrint("[Shutdown]%llx\n", Dispatch);

		pNotify[i].CallbacksAddr = Dispatch;
		pNotify[i].CallbackType = 4; // shutdown

		if (NT_SUCCESS(getSystemImageInfoByAddress(pNotify[i].CallbacksAddr, &Sysmodule)) &&
			strlen(Sysmodule.ImageName) < MAX_PATH)
		{
			RtlCopyMemory(pNotify[i].ImgPath, Sysmodule.ImageName, MAX_PATH);
		}
		i++;

	}

	pNotify[0].Count = i;

	return pNotify;
}

ULONG64 GetKeBugCheckReasonCallbackListHead() //ȡ�����ã�KeDeregisterBugCheckReasonCallback
{
	ULONG64			i = 0, pCheckArea = 0;
	UNICODE_STRING	unstrFunc;
	RtlInitUnicodeString(&unstrFunc, L"KeRegisterBugCheckReasonCallback");
	pCheckArea = (ULONG64)MmGetSystemRoutineAddress(&unstrFunc);
	//DbgPrint("KeRegisterBugCheckReasonCallback: %llx\n", pCheckArea);
	for (i = pCheckArea; i<pCheckArea + 0xfff; i++)
	{
		if (*(PUCHAR)i == 0x48 && *(PUCHAR)(i + 1) == 0x8b && *(PUCHAR)(i + 2) == 0x05)	//mov rax,xxxx
		{
			LONG OffsetAddr = 0;
			memcpy(&OffsetAddr, (PUCHAR)(i + 3), 4);
			return OffsetAddr + 7 + i;
		}
	}
	return 0;
}

ULONG64 GetKeBugCheckCallbackListHead() //ȡ�����ã�KeDeregisterBugCheckCallback
{
	ULONG64			i = 0, pCheckArea = 0;
	UNICODE_STRING	unstrFunc;
	RtlInitUnicodeString(&unstrFunc, L"KeRegisterBugCheckCallback");
	pCheckArea = (ULONG64)MmGetSystemRoutineAddress(&unstrFunc);
	//DbgPrint("KeRegisterBugCheckCallback: %llx\n", pCheckArea);
	for (i = pCheckArea; i<pCheckArea + 0xfff; i++)
	{
		if (*(PUCHAR)i == 0x48 && *(PUCHAR)(i + 1) == 0x8d && *(PUCHAR)(i + 2) == 0x0d &&
			*(PUCHAR)(i - 1) == 0xc1 && *(PUCHAR)(i - 2) == 0x03 && *(PUCHAR)(i - 3) == 0x48)	//lea rcx,xxxx
		{
			LONG OffsetAddr = 0;
			memcpy(&OffsetAddr, (PUCHAR)(i + 3), 4);
			return OffsetAddr + 7 + i;
		}
	}
	return 0;
}

PNOTIFY_INFO EnumBugcheckCallback()
{
	ULONG64 Dispatch = 0;
	ULONG	i = 0;
	PNOTIFY_INFO pNotify = NULL;
	SYSTEM_MODULE Sysmodule = { 0 };
	PLIST_ENTRY CurrEntry = NULL, g_KeBugCheckCallbackListHead = NULL, g_KeBugCheckReasonCallbackListHead = NULL;
	ULONG64 KeBugCheckReasonCallbackListHead, KeBugCheckCallbackListHead;
	KeBugCheckReasonCallbackListHead = GetKeBugCheckReasonCallbackListHead();
	KeBugCheckCallbackListHead = GetKeBugCheckCallbackListHead();
	//DbgPrint("KeBugCheckReasonCallbackListHead: %llx\n", KeBugCheckReasonCallbackListHead);
	//DbgPrint("KeBugCheckCallbackListHead: %llx\n", KeBugCheckCallbackListHead);
	g_KeBugCheckReasonCallbackListHead = (PLIST_ENTRY)KeBugCheckReasonCallbackListHead;
	g_KeBugCheckCallbackListHead = (PLIST_ENTRY)KeBugCheckCallbackListHead;

	pNotify = ExAllocatePool(NonPagedPool, sizeof(NOTIFY_INFO) * 100);

	if (pNotify == NULL)
		return NULL;

	//enum bugcheck reason callback
	CurrEntry = g_KeBugCheckReasonCallbackListHead->Flink;
	do
	{
		Dispatch = *(ULONG64 *)((ULONG64)CurrEntry + sizeof(LIST_ENTRY));
		//DbgPrint("Entry:%llx\n", CurrEntry);
		//DbgPrint("[BugCheckReason]%llx\n", Dispatch);
		CurrEntry = CurrEntry->Flink;

		pNotify[i].CallbacksAddr = Dispatch;
		pNotify[i].CallbackType = 6; // BugCheckReason
		memset(&Sysmodule,0,sizeof(SYSTEM_MODULE));

		if (NT_SUCCESS(getSystemImageInfoByAddress(pNotify[i].CallbacksAddr, &Sysmodule)) &&
			strlen(Sysmodule.ImageName) < MAX_PATH)
		{
			RtlCopyMemory(pNotify[i].ImgPath, Sysmodule.ImageName, MAX_PATH);
		}
		i++;

	} while (CurrEntry != g_KeBugCheckReasonCallbackListHead);
	//enum bugcheck callback
	CurrEntry = g_KeBugCheckCallbackListHead->Flink;
	do
	{
		Dispatch = *(ULONG64 *)((ULONG64)CurrEntry + sizeof(LIST_ENTRY));
		//DbgPrint("Entry:%llx\n", CurrEntry);
		//DbgPrint("[BugCheck]%llx\n", Dispatch);
		CurrEntry = CurrEntry->Flink;

		pNotify[i].CallbacksAddr = Dispatch;
		pNotify[i].CallbackType = 5; // BugCheck
		memset(&Sysmodule, 0, sizeof(SYSTEM_MODULE));

		if (NT_SUCCESS(getSystemImageInfoByAddress(pNotify[i].CallbacksAddr, &Sysmodule)) &&
			strlen(Sysmodule.ImageName) < MAX_PATH)
		{
			RtlCopyMemory(pNotify[i].ImgPath, Sysmodule.ImageName, MAX_PATH);
		}
		i++;

	} while (CurrEntry != g_KeBugCheckCallbackListHead);

	pNotify[0].Count = i;
	return pNotify;
}