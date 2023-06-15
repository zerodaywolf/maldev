#include <windows.h>
#include <stdio.h>

const char* k = "[+]";
const char* e = "[-]";
const char* i = "[*]";

int main(int argc, char* argv[]) {

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	if (CreateProcessW(L"C:\\Windows\\system32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == 0) {
		printf("%s failed to create process, error: %ld\n", e, GetLastError());
		return EXIT_FAILURE;
	}

	DWORD TID = pi.dwThreadId;
	DWORD PID = pi.dwProcessId;
	HANDLE hThread = pi.hThread;
	HANDLE hProcess = pi.hProcess;

	printf("%s got handle to process\n%s process started!\n\t%s pid: %ld, handle: 0x%x\n\t%s tid: %ld, handle: 0x%x\n", k, k, k, PID, hProcess, k, TID, hThread);

	/* cleanup */
	WaitForSingleObject(hProcess, INFINITE);

	if (CloseHandle(hProcess) == 0) {
		printf("%s failed to close handle to process 0x%x, error: %ld\n", e, hProcess, GetLastError);
		return EXIT_FAILURE;
	}
	
	printf("%s finished! exiting..\n", k);

	return EXIT_SUCCESS;
}