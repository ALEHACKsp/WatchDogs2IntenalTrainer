// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include<Windows.h>
#include <vector>
#include<iostream>


struct values
{
	uintptr_t money;
	uintptr_t moduleBase;
}vals;
struct offsets
{
	DWORD moneyOffsetBase = 0x3C82200;
}offs;

uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}


DWORD WINAPI dll_mains(HMODULE hModule)
{
	AllocConsole();
	freopen("CONOUT$","w",stdout);


	vals.moduleBase = (uintptr_t)GetModuleHandle("Disrupt_64.dll");
	std::cout << vals.moduleBase;
	uintptr_t pMoney = FindDMAAddy(vals.moduleBase + offs.moneyOffsetBase, { 0x38,0x50,0x68,0x18,0xE0,0x0,0x840 }); //CRASH OCCURS HERE
	
	while (true)
	{
		int money = *(int*)(pMoney);
		if (GetAsyncKeyState(VK_INSERT))
		{
			*(int*)pMoney = 999;
			std::cout<<std::hex<<pMoney<<std::dec << " > " <<money<<"\n";
		}
		if (GetAsyncKeyState(VK_END))
		{
			std::cout << std::hex <<pMoney << " > "<<std::dec<<money<<"\n";
		}
		
	}
	
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)dll_mains, 0, 0, 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}