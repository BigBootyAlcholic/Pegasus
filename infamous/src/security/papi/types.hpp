#pragma once
#include <psapi.h>

namespace Security
{
	// Data types
	struct AnsiString
	{
		u16 Length;
		u16 MaximumLength;
		char* Buffer;
	};

	struct UnicodeString
	{
		u16 Length;
		u16 MaximumLength;
		wchar_t* Buffer;
	};

	struct ListEntry
	{
		u64 Flink;
		u64 Blink;
	};

	struct LdrDataTableEntry
	{
		ListEntry InLoadOrderLinks;
		ListEntry InMemoryOrderLinks;
		ListEntry InInitializationOrderLinks;
		u64 BaseAddress;
		u64 EntryPoint;
		u64 SizeOfImage;
		UnicodeString FullDllName;
		UnicodeString BaseDllName;
	};

	struct PebLdrData
	{
		u32 Length;
		u8 Initialized;
		u64 SsHandle;
		ListEntry InLoadOrderModuleList;
		ListEntry InMemoryOrderModuleList;
		ListEntry InInitializationOrderModuleList;
		u64 EntryInProgress;
		u8 ShutdownInProgress;
		u64 ShutdownThreadId;
	};

	typedef struct _STRING
	{
		USHORT Length;
		USHORT MaximumLength;
		PCHAR Buffer;
	}STRING;

	typedef STRING* PSTRING;
	typedef STRING ANSI_STRING;
	typedef PSTRING PANSI_STRING;

	typedef struct _UNICODE_STRING
	{
		USHORT Length;
		USHORT MaximumLength;
		PWSTR  Buffer;
	} UNICODE_STRING, * PUNICODE_STRING;

	typedef struct _LDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		LIST_ENTRY InInitializationOrderLinks;
		PVOID BaseAddress;
		PVOID EntryPoint;
		DWORD64 SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		WORD LoadCount;
		WORD TlsIndex;
		union
		{
			LIST_ENTRY HashLinks;
			struct
			{
				PVOID SectionPointer;
				ULONG CheckSum;
			} structHashLinks;
		};
		union
		{
			ULONG TimeDateStamp;
			PVOID LoadedImports;
		};
		PVOID EntryPointActivationContext;
		PVOID Lock;
		/*
		+0x000 InLoadOrderLinks : _LIST_ENTRY
		+0x010 InMemoryOrderLinks : _LIST_ENTRY
		+0x020 InInitializationOrderLinks : _LIST_ENTRY
		+0x030 DllBase          : Ptr64 Void
		+0x038 EntryPoint       : Ptr64 Void
		+0x040 SizeOfImage      : Uint4B
		+0x048 FullDllName      : _UNICODE_STRING
		+0x058 BaseDllName      : _UNICODE_STRING
		+0x068 Flags            : Uint4B
		+0x06c LoadCount        : Uint2B
		+0x06e TlsIndex         : Uint2B
		+0x070 HashLinks        : _LIST_ENTRY
		+0x070 SectionPointer   : Ptr64 Void
		+0x078 CheckSum         : Uint4B
		+0x080 TimeDateStamp    : Uint4B
		+0x080 LoadedImports    : Ptr64 Void
		+0x088 EntryPointActivationContext : Ptr64 _ACTIVATION_CONTEXT
		+0x090 PatchInformation : Ptr64 Void
		+0x098 ForwarderLinks   : _LIST_ENTRY
		+0x0a8 ServiceTagLinks  : _LIST_ENTRY
		+0x0b8 StaticLinks      : _LIST_ENTRY
		+0x0c8 ContextInformation : Ptr64 Void
		+0x0d0 OriginalBase     : Uint8B
		*/
	}LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

	typedef struct _PEB_LDR_DATA
	{
		DWORD Length;
		DWORD Initialized;
		DWORD64 SsHandle;
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		LIST_ENTRY InInitializationOrderLinks;
		DWORD64 EntryInProgress;
		DWORD64 ShutdownInProgress;
		DWORD64 ShutdownThreadId;
		/*
		+0x000 Length           : Uint4B
		+0x004 Initialized      : UChar
		+0x008 SsHandle         : Ptr64 Void
		+0x010 InLoadOrderModuleList : _LIST_ENTRY
		+0x020 InMemoryOrderModuleList : _LIST_ENTRY
		+0x030 InInitializationOrderModuleList : _LIST_ENTRY
		+0x040 EntryInProgress  : Ptr64 Void
		+0x048 ShutdownInProgress : UChar
		+0x050 ShutdownThreadId : Ptr64 Void
		*/
	}PEB_LDR_DATA, * PPEB_LDR_DATA;

	typedef struct _PEB
	{
		BYTE                          Reserved1[2];
		BYTE                          BeingDebugged;
		BYTE                          Reserved2[1];
		PVOID                         Reserved3[2];
		PPEB_LDR_DATA                 Ldr;
		PVOID                         ProcessParameters;
		BYTE                          Reserved4[104];
		PVOID                         Reserved5[52];
		PVOID                         PostProcessInitRoutine;
		BYTE                          Reserved6[128];
		PVOID                         Reserved7[1];
		ULONG                         SessionId;
	} PEB, * PPEB;

	typedef struct _OBJECT_ATTRIBUTES
	{
		ULONG           Length;
		HANDLE          RootDirectory;
		PUNICODE_STRING ObjectName;
		ULONG           Attributes;
		PVOID           SecurityDescriptor;
		PVOID           SecurityQualityOfService;
	}  OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

	typedef struct _IO_STATUS_BLOCK
	{
		union
		{
			NTSTATUS Status;
			PVOID Pointer;
		} DUMMYUNIONNAME;
		ULONG_PTR Information;
	} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

	typedef struct _SP_DEVINFO_DATA {
		DWORD cbSize;
		GUID  ClassGuid;
		DWORD DevInst;    // DEVINST handle
		ULONG_PTR Reserved;
	} SP_DEVINFO_DATA, * PSP_DEVINFO_DATA;

	typedef GUID  DEVPROPGUID, * PDEVPROPGUID;
	typedef ULONG DEVPROPID, * PDEVPROPID;

	typedef PVOID HDEVINFO;

	typedef enum _MEMORY_INFORMATION_CLASS {
		MemoryBasicInformation,
		MemoryWorkingSetList,
		MemorySectionName,
		MemoryBasicVlmInformation,
		Last_MEMORY_INFORMATION_CLASS = 0xffffffffffffffff	// Need to force this enum to be 64bit
	} MEMORY_INFORMATION_CLASS;

	typedef enum _PROCESS_INFORMATION_CLASS {
		ProcessBasicInformation = 0,
		ProcessDebugPort = 7,
		ProcessWow64Information = 26,
		ProcessImageFileName = 27,
		ProcessBreakOnTermination = 28,
		Last_PROCESS_INFORMATION_CLASS = 0xffffffffffffffff	// Need to force this enum to be 64bit
	} PROCESS_INFORMATION_CLASS;

	typedef enum _SYSTEM_INFORMATION_CLASS {
		SystemBasicInformation = 0,
		SystemProcessInformation = 5,
		SystemKernelDebuggerInformation = 35,
		SystemExtendedHandleInformation = 64,
		Last_SYSTEM_INFORMATION_CLASS = 0xffffffffffffffff	// Need to force this enum to be 64bit
	} SYSTEM_INFORMATION_CLASS;

	typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
		bool DebuggerEnabled;
		bool DebuggerNotPresent;
	} SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

	typedef struct _SYSTEM_PROCESS_INFORMATION // Size=200
	{
		ULONG NextEntryOffset; // Size=4 Offset=0
		ULONG NumberOfThreads; // Size=4 Offset=4
		LARGE_INTEGER WorkingSetPrivateSize; // Size=8 Offset=8
		ULONG HardFaultCount; // Size=4 Offset=16
		ULONG NumberOfThreadsHighWatermark; // Size=4 Offset=20
		ULONGLONG CycleTime; // Size=8 Offset=24
		LARGE_INTEGER CreateTime; // Size=8 Offset=32
		LARGE_INTEGER UserTime; // Size=8 Offset=40
		LARGE_INTEGER KernelTime; // Size=8 Offset=48
		UNICODE_STRING ImageName; // Size=8 Offset=56
		LONG BasePriority; // Size=4 Offset=64
		SIZE_T UniqueProcessId; // Size=4 Offset=68
		SIZE_T InheritedFromUniqueProcessId; // Size=4 Offset=76
		ULONG HandleCount; // Size=4 Offset=84
		ULONG SessionId; // Size=4 Offset=88
		ULONG UniqueProcessKey; // Size=4 Offset=92
		SIZE_T PeakVirtualSize; // Size=4 Offset=96
		SIZE_T VirtualSize; // Size=4 Offset=104
		ULONG PageFaultCount; // Size=4 Offset=112
		ULONG PeakWorkingSetSize; // Size=4 Offset=116
		ULONG WorkingSetSize; // Size=4 Offset=120
		ULONG QuotaPeakPagedPoolUsage; // Size=4 Offset=124
		ULONG QuotaPagedPoolUsage; // Size=4 Offset=128
		ULONG QuotaPeakNonPagedPoolUsage; // Size=4 Offset=132
		ULONG QuotaNonPagedPoolUsage; // Size=4 Offset=136
		ULONG PagefileUsage; // Size=4 Offset=140
		ULONG PeakPagefileUsage; // Size=4 Offset=144
		ULONG PrivatePageCount; // Size=4 Offset=148
		LARGE_INTEGER ReadOperationCount; // Size=8 Offset=152
		LARGE_INTEGER WriteOperationCount; // Size=8 Offset=160
		LARGE_INTEGER OtherOperationCount; // Size=8 Offset=168
		LARGE_INTEGER ReadTransferCount; // Size=8 Offset=176
		LARGE_INTEGER WriteTransferCount; // Size=8 Offset=184
		LARGE_INTEGER OtherTransferCount; // Size=8 Offset=192
	} SYSTEM_PROCESS_INFORMATION;

	typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX
	{
		PVOID Object;
		ULONG_PTR UniqueProcessId;
		ULONG_PTR HandleValue;
		ULONG GrantedAccess;
		USHORT CreatorBackTraceIndex;
		USHORT ObjectTypeIndex;
		ULONG HandleAttributes;
		ULONG Reserved;
	} SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO_EX;

	typedef struct _SYSTEM_HANDLE_INFORMATION_EX
	{
		ULONG_PTR NumberOfHandles;
		ULONG_PTR Reserved;
		SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];
	} SYSTEM_HANDLE_INFORMATION_EX, * PSYSTEM_HANDLE_INFORMATION_EX;

	// NtDll function pointer types.
	typedef NTSTATUS(WINAPI* NtQueryVirtualMemoryFnPtr)(
		_In_	HANDLE						ProcessHandle,
		_In_	PVOID						BaseAddress,
		_In_	MEMORY_INFORMATION_CLASS	MemoryInformationClass,
		_Out_	PVOID						Buffer,
		_In_	size_t						Length,
		_Out_	size_t* ResultLength
		);

	typedef NTSTATUS(WINAPI* NtQuerySystemInformationFnPtr)(
		_In_       SYSTEM_INFORMATION_CLASS SystemInformationClass,
		_Inout_    PVOID					stemInformation,
		_In_       size_t					SystemInformationLength,
		_Out_opt_  size_t* ReturnLength
		);

	typedef NTSTATUS(WINAPI* NtQueryInformationProcessFnPtr)(
		_In_       HANDLE	ProcessHandle,
		_In_       INT		ProcessInformationClass,
		_Out_      PVOID	ProcessInformation,
		_In_       size_t	ProcessInformationLength,
		_Out_opt_  size_t* ReturnLength
		);

	typedef NTSTATUS(*NtOpenProcessFnPtr)(
		_Out_     PHANDLE				ProcessHandle,
		_In_      ACCESS_MASK			DesiredAccess,
		_In_      POBJECT_ATTRIBUTES	ObjectAttributes,
		_In_opt_  PVOID					ClientId
		);

	typedef NTSTATUS(WINAPI* RtlGetVersionFn)(PRTL_OSVERSIONINFOW);
	typedef VOID(WINAPI* RtlInitUnicodeStringFn)(PUNICODE_STRING DestinationString, PCWSTR SourceString);

	// Kernel32 types.
	typedef BOOL(WINAPI* CloseHandleFn)(HANDLE);
	typedef HANDLE(WINAPI* CreateFileAFn)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

	typedef BOOL(WINAPI* DeviceIoControlFn)(HANDLE, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);

	typedef BOOL(WINAPI* GetComputerNameExWFn)(COMPUTER_NAME_FORMAT, LPWSTR, LPDWORD);
	typedef HANDLE(WINAPI* GetCurrentProcessFn)();
	typedef DWORD(WINAPI* GetLogicalDrivesFn)(void);
	typedef BOOL(WINAPI* GetModuleInformationFn)(HANDLE, HMODULE, LPMODULEINFO, DWORD);
	typedef VOID(WINAPI* GetNativeSystemInfoFn)(LPSYSTEM_INFO);
	typedef BOOL(WINAPI* GetPhysicallyInstalledSystemMemoryFn)(PULONGLONG);
	typedef HANDLE(WINAPI* GetProcessHeapFn)(VOID);
	typedef UINT(WINAPI* GetSystemFirmwareTableFn)(DWORD, DWORD, PVOID, DWORD);
	typedef void(WINAPI* GetSystemTimeAsFileTimeFn)(LPFILETIME);
	typedef BOOL(WINAPI* GetVolumeInformationAFn)(LPCSTR, LPSTR, DWORD, LPDWORD, LPDWORD, LPDWORD, LPSTR, DWORD);
	typedef BOOL(WINAPI* GlobalMemoryStatusExFn)(LPMEMORYSTATUSEX);

	typedef LPVOID(WINAPI* HeapAllocFn)(HANDLE, DWORD, SIZE_T);
	typedef BOOL(WINAPI* HeapFreeFn)(HANDLE, DWORD, LPVOID);

	typedef HMODULE(WINAPI* LoadLibraryAFn)(LPCSTR);
	typedef HMODULE(WINAPI* LoadLibraryWFn)(LPCWSTR);

	typedef long (WINAPI* RegCloseKeyFn)(HKEY);
	typedef long (WINAPI* RegEnumValueAFn)(HKEY, DWORD, LPSTR, LPDWORD, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
	typedef long (WINAPI* RegOpenKeyExAFn)(HKEY, LPCSTR, DWORD, REGSAM, PHKEY);
	typedef long (WINAPI* RegQueryValueExAFn)(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD);

	typedef BOOL(WINAPI* GetModuleInformationFnPtr)(
		__in	HANDLE	hProcess,
		__in	HMODULE	hModule,
		__out	LPMODULEINFO lpmodinfo,
		__in	DWORD cb
		);

	typedef DWORD(WINAPI* GetModuleFileNameAFnPtr)(
		__in_opt HMODULE hModule,
		__out_ecount_part(nSize, return +1) LPSTR lpFilename,
		__in DWORD nSize
		);

	typedef DWORD(WINAPI* GetModuleFileNameWFnPtr)(
		__in_opt HMODULE hModule,
		__out_ecount_part(nSize, return +1) LPWSTR lpFilename,
		__in DWORD nSize
		);

	typedef DWORD(WINAPI* GetModuleFileNameExAFnPtr)(
		__in HANDLE hProcess,
		__in_opt HMODULE hModule,
		__out_ecount(nSize) LPSTR lpFilename,
		__in DWORD nSize
		);

	typedef DWORD(WINAPI* GetModuleFileNameExWFnPtr)(
		__in HANDLE hProcess,
		__in_opt HMODULE hModule,
		__out_ecount(nSize) LPWSTR lpFilename,
		__in DWORD nSize
		);

	typedef HANDLE(WINAPI* OpenProcessFnPtr)(
		__in DWORD dwDesiredAccess,
		__in BOOL bInheritHandle,
		__in DWORD dwProcessId
		);

	typedef BOOL(WINAPI* EnumProcessesFn)(DWORD*, DWORD, LPDWORD);

	typedef BOOL(WINAPI* EnumProcessModulesFnPtr)(
		__in  HANDLE hProcess,
		__out_bcount(cb) HMODULE* lphModule,
		__in  DWORD cb,
		__out LPDWORD lpcbNeeded
		);

	typedef BOOL(WINAPI* ReadProcessMemoryFnPtr)(
		__in      HANDLE hProcess,
		__in      LPCVOID lpBaseAddress,
		__out_bcount_part(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
		__in      SIZE_T nSize,
		__out_opt SIZE_T* lpNumberOfBytesRead
		);

	typedef BOOL(WINAPI* CloseHandleFnPtr)(
		__in HANDLE hObject
		);

	typedef HANDLE(WINAPI* GetCurrentProcessFnPtr)(
		VOID
		);

	typedef HLOCAL(WINAPI* LocalAllocFnPtr)(
		__in UINT uFlags,
		__in SIZE_T uBytes
		);

	typedef HLOCAL(WINAPI* LocalFreeFnPtr)(
		__deref HLOCAL hMem
		);

	// Ole32 types.
	typedef HRESULT(STDAPICALLTYPE* CoCreateInstanceFn)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID FAR*);
	typedef HRESULT(STDAPICALLTYPE* CoInitializeExFn)(LPVOID, DWORD);
	typedef HRESULT(STDAPICALLTYPE* CoSetProxyBlanketFn)(IUnknown*, DWORD, DWORD, OLECHAR*, DWORD, DWORD, RPC_AUTH_IDENTITY_HANDLE, DWORD);
	typedef void(STDAPICALLTYPE* CoTaskMemFreeFn)(LPVOID);
	typedef void(STDAPICALLTYPE* CoUninitializeFn)(void);

	typedef HRESULT(STDAPICALLTYPE* PropVariantClearFn)(PROPVARIANT*);

	// Combase types.
	typedef BOOL(CALLBACK* LPDSENUMCALLBACKW)(LPGUID, LPCWSTR, LPCWSTR, LPVOID);
	typedef HRESULT(WINAPI* DirectSoundEnumerateFn)(LPDSENUMCALLBACKW, LPVOID);
	typedef int(WINAPI* StringFromGUID2Fn)(REFGUID, LPOLESTR, int);

	typedef HDEVINFO(WINAPI* SetupDiGetClassDevsAFn)(CONST GUID* ClassGuid, PCSTR, HWND, DWORD);
	typedef HDEVINFO(WINAPI* SetupDiGetClassDevsWFn)(CONST GUID* ClassGuid, PCWSTR, HWND, DWORD);
	typedef BOOL(WINAPI* SetupDiEnumDeviceInfoFn)(HDEVINFO, DWORD, PSP_DEVINFO_DATA);

	// oleauth32 types.
	typedef BSTR(STDAPICALLTYPE* SysAllocStringFn)(const OLECHAR*);
	typedef BSTR(STDAPICALLTYPE* SysFreeStringFn)(BSTR);

	typedef ULONG			EXPORT_ENTRY;
	typedef EXPORT_ENTRY* PEXPORT_ENTRY;

	// User32 types.
	typedef BOOL(WINAPI* EnumDisplayDevicesAFn)(LPCSTR, DWORD, PDISPLAY_DEVICEA, DWORD);
	typedef BOOL(WINAPI* EnumDisplayDevicesWFn)(LPCWSTR, DWORD, PDISPLAY_DEVICEW, DWORD);

	// DXGI types
	typedef HRESULT(WINAPI* CreateDXGIFactoryFn)(REFIID riid, _Out_ void** ppFactory);

	// cfgmgr32 types
	typedef DWORD(WINAPI* CMGetDeviceIDWFn)(DWORD dnDevInst, PWSTR Buffer, ULONG BufferLen, ULONG ulFlags);

	// shell32 types
	typedef HRESULT(WINAPI* SHGetFolderPathAFn)(HWND, int, HANDLE, DWORD, LPSTR);

	// Advapi32 types
	typedef LSTATUS(WINAPI* RegQueryValueExAFn)(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
	typedef LSTATUS(WINAPI* RegOpenKeyExAFn)(HKEY, LPCSTR, DWORD, REGSAM, PHKEY);
}
