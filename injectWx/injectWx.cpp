// injectWx.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <minwindef.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <memoryapi.h>
#include <libloaderapi.h>
#include <windows.h>
#include <lmcons.h>  
#include <lmat.h> 
#include <string>
#include <locale>
#include <codecvt>
#pragma comment(lib,"NETAPI32.LIB")  
using namespace std;
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"mainCRTStartup\"")
DWORD GetPIDForProcess(TCHAR* process)
{
	BOOL                    working;
	PROCESSENTRY32          lppe = { 0 };
	DWORD                   targetPid = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hSnapshot)
	{
		return 0;
	}
	lppe.dwSize = sizeof(lppe);
	working = Process32First(hSnapshot, &lppe);
	while (working)
	{
		if (_tcscmp((TCHAR*)lppe.szExeFile, process) == 0)
		{
			targetPid = lppe.th32ProcessID;
			break;
		}working = Process32Next(hSnapshot, &lppe);
	}
	CloseHandle(hSnapshot);
	return targetPid;
}
void shutdown(const TCHAR* wc)
{
	DWORD dwPID = 0;
	dwPID = GetPIDForProcess(const_cast<TCHAR*>(wc));

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!hProcess)
	{
		return;
	}
	// 调用远程dll
	HMODULE hK32 = GetModuleHandle(_T("Kernel32.dll"));
	LPVOID lpLoadLibAddr = GetProcAddress(hK32, "sjkdghpouiawrklj");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLibAddr, NULL, 0, 0);
	if (!hThread)
	{
		return;
	}
}
void shutdownAll()
{
	BOOL                    working;
	PROCESSENTRY32          lppe = { 0 };
	DWORD                   targetPid = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hSnapshot)
	{
		return ;
	}
	lppe.dwSize = sizeof(lppe);
	working = Process32First(hSnapshot, &lppe);
	while (working)
	{
		if (_tcscmp((TCHAR*)lppe.szExeFile, "injectWx.exe") == 0)
		{
			working = Process32Next(hSnapshot, &lppe);
			continue;
			//targetPid = lppe.th32ProcessID;
			//break;
		}

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lppe.th32ProcessID);
		if (!hProcess)
		{
			working = Process32Next(hSnapshot, &lppe);
			continue;
		}
		// 调用远程dll
		HMODULE hK32 = GetModuleHandle(_T("Kernel32.dll"));
		LPVOID lpLoadLibAddr = GetProcAddress(hK32, "sjkdghpouiawrklj");
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLibAddr, NULL, 0, 0);
		if (!hThread)
		{
			working = Process32Next(hSnapshot, &lppe);
			continue;;
		}

		working = Process32Next(hSnapshot, &lppe);
	}
	CloseHandle(hSnapshot);
}

std::string GetExePath()
{
	char exeFullPath[MAX_PATH] = { 0 };
	std::string strPath = "";
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = (std::string)exeFullPath;
	return strPath;
}

inline std::wstring to_wide_string(const std::string& input)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(input);
}

void Jobadd() {
	DWORD JobId, ret;
	AT_INFO ai;
	//char* filepath;
	//long Len;
	//char RootPath[MAX_PATH];
	//WCHAR szFilePath[256];
	//GetSystemDirectory(RootPath, MAX_PATH);
	//filepath = new char[strlen(RootPath) + 11];
	//strcpy(filepath, RootPath);
	//strcat(filepath, "\\notepad.exe"); //完整路径  
	//memset(&ai, 0, sizeof(ai));
	//Len = MultiByteToWideChar(CP_ACP, 0, filepath, strlen(filepath), szFilePath, sizeof(szFilePath));
	//szFilePath[Len] = '\0';
	string exePath = GetExePath();
	wstring wsExePath = to_wide_string(exePath);
	ai.Command = const_cast<wchar_t*>(wsExePath.c_str());
	ai.DaysOfMonth = 0;
	ai.DaysOfWeek = 0x7F; //7F等于二进制的7个1，就是每周的7天全部运行  
	ai.Flags = JOB_RUN_PERIODICALLY;
	ai.JobTime = 22 * 60 * 60 * 1000 + 21 * 60 * 1000; //22点21分，这里是以毫秒为单位的，所以需要乘这些值  
	ret = NetScheduleJobAdd(NULL, LPBYTE(&ai), &JobId);
	if (ret == ERROR_SUCCESS)
		std::cout << " SUCCESS!!" << std::endl;
	else
		std::cout << "Error" << std::endl;

}

int main()
{
	//Jobadd();
	shutdown("WXWork.exe");
	shutdown("WeChat.exe");
	//shutdownAll();

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
