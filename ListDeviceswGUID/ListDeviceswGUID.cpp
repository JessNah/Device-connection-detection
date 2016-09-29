// ListDeviceswGUID.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <initguid.h>
#include <iostream>
#include <string>
#include <vector>
#include <devguid.h>
#include <stdlib.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <regstr.h>
#include <time.h>
#include <math.h>



using namespace std;
using namespace System;
using namespace System::Configuration;



#define MAX_DATA	35 
DEFINE_GUID(guid, 0x745a17a0, 0x74d3, 0x11d0, 0xb6, 0xfe, 0x00, 0xa0, 0xc9, 0x0f, 0x57, 0xda);

int foundMouse = 0;
bool found = false;


class timer {
private:
	unsigned long begTime;
public:
	void start() {
		begTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long)clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}

	unsigned long getTime() {
		return ((unsigned long)clock()) / CLOCKS_PER_SEC;
	}
};



int DevicesMatchingGuid() {

	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;

	int countFound = 0;

	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs((GUID*)&guid,
		0,
		0,
		DIGCF_PRESENT); // | DIGCF_ALLCLASSES  <--Add this for all installed devices (not currently present too)

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		printf("error");
		return 0;
	}

	// Enumerate all devices in Set.

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i,
		&DeviceInfoData); i++)
	{
		DWORD DataT;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;

		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			if (GetLastError() ==
				ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer) LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
			}
			else
			{
				break;
			}
		}


		LPTSTR mouse = L"Lenovo Optical Mouse (HID)";

		if (0 == lstrcmpi(buffer, mouse)) 	foundMouse = 1;


		printf("Device: ", buffer);

		for (int i = 0; i < MAX_DATA; i++) {
			printf("%s", buffer + i);
		}
		if (foundMouse == 1) {
			printf("       <--- Mouse found !!! ");
			foundMouse = 0;
			found = true;
		}

		printf(" \n", buffer);
		countFound++;



		if (buffer) LocalFree(buffer);
	}


	if (GetLastError() != NO_ERROR &&
		GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		printf("error");
		return 0;
	}

	//Clearance
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return countFound;

}

bool checkConnection() {

	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;

	foundMouse = 0;

	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs((GUID*)&guid,
		0,
		0,
		DIGCF_PRESENT); // | DIGCF_ALLCLASSES  <--Add this for all installed devices (not currently present too)

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		printf("error");
		return false;
	}

	// Enumerate all devices in Set.

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i,
		&DeviceInfoData); i++)
	{
		DWORD DataT;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;

		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			if (GetLastError() ==
				ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer) LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
			}
			else
			{
				break;
			}
		}
		
		LPTSTR mouse = L"Lenovo Optical Mouse (HID)";

		if (0 == lstrcmpi(buffer, mouse)) 	foundMouse = 1;

		if (foundMouse == 1) {
			foundMouse = 0;
			SetupDiDestroyDeviceInfoList(hDevInfo);
			return true;
		}
		if (buffer) LocalFree(buffer);
	}

	//Clearance
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return false;
}


int main()
{
	unsigned long seconds = 5;
	timer t;
	bool printed = false;
	int prevTimeRound = 0;

	int test = 0;

	test = DevicesMatchingGuid();
	if (test == 0) {
		printf("no matching USB devices were found \n");
	}
	else {
		printf("%d matching SetupClass GUID type USB devices were found\n", test);
	}

	if (found == true) { //if a mouse was found, continue to check its connection status
		t.start();
		printf("timer started . . .");
		while (true) {
			double timeT = (double)(t.getTime()); //start timer
			int timeRound =trunc(timeT);
			if (timeRound != prevTimeRound){
				prevTimeRound = timeRound;
				printed = false;
			}
			if ((timeRound % 5) == 0 && printed == false) { //every 5 seconds check status
				if (checkConnection()) {
					printf("\n checkConnection() ==> Device found \n");
					printed = true;
				}
				else{
					printf("\n checkConnection() ==> Device not found \n");
					printed = true;
				}
			}
		}
	}

	getchar();
}

