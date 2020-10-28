#pragma once
#include <windows.h>
#include <string>

#ifndef IMEM_H
#define IMEM_H

typedef unsigned short ushort;
typedef unsigned int uint;
typedef uintptr_t QWORD;

#ifdef _WIN64
#define PTRMAXVAL ((PVOID)0x000F000000000000)
#else
#define PTRMAXVAL ((PVOID)0xFFF00000)
#endif

class Mem
{
public:

	template <class vType>
	static vType Read(QWORD Add)
	{
		__try
		{
			if (IsValid((vType*)(Add)))
				return *(vType*)(Add);
			return vType();
		}
		__except (InternalErrorHandler(GetExceptionCode(), GetExceptionInformation()))
		{
			return vType();
		}
	}

	static std::string ReadString(QWORD Add, size_t strLength)
	{
		char szString[512] = { 0 };
		for (size_t i = 0; i < strLength; i++)
		{
			char tChar;
			tChar = *(char*)(Add + (i * 0x1));
			szString[i] = tChar;
		}
		return szString;
	}

	template <typename vType>
	static vType ReadPtr(std::initializer_list<QWORD> _Offsets, bool ReadFirstOffset)
	{
		QWORD LastPtr = NULL;
		int OffsetsSize = NULL;
		std::vector<QWORD> Offsets = { NULL };
		Offsets = _Offsets;
		OffsetsSize = Offsets.size();
		LastPtr = Read<QWORD>((ReadFirstOffset ? Read<QWORD>(Offsets[0]) : Offsets[0]) + Offsets[1]);
		for (size_t i = 2; i < OffsetsSize - 1; i++)
			if (!(LastPtr = Read<QWORD>(LastPtr + Offsets[i])))
				return vType();
		return Read<vType>(LastPtr + Offsets[OffsetsSize - 1]);
	}

	template <typename vType>
	static BOOLEAN WritePtr(std::initializer_list<QWORD> _Offsets, vType _value, bool ReadFirstOffset)
	{
		QWORD LastPtr = NULL;
		int OffsetsSize = NULL;
		std::vector<QWORD> Offsets = { NULL };
		Offsets = _Offsets;
		OffsetsSize = Offsets.size();
		LastPtr = Read<QWORD>((ReadFirstOffset ? Read<QWORD>(Offsets[0]) : Offsets[0]) + Offsets[1]);
		for (size_t i = 2; i < OffsetsSize - 1; i++)
			if (!(LastPtr = Read<QWORD>(LastPtr + Offsets[i])))
				return FALSE;
		return Write<vType>(LastPtr + Offsets[OffsetsSize - 1], _value);
	}

	template <class vType>
	static BOOLEAN Write(QWORD address, vType value)
	{
		__try
		{
			if (IsValid((vType*)(address)))
			{
				*(vType*)(address) = value;
				return TRUE;
			}
			return FALSE;
		}
		__except (InternalErrorHandler(GetExceptionCode(), GetExceptionInformation()))
		{
			return FALSE;
		}
	}

	static int InternalErrorHandler(unsigned int Code, struct _EXCEPTION_POINTERS* EP)
	{
		return (Code == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
	}

	static __forceinline BOOLEAN IsValid(PVOID ptr)
	{ 
		return (ptr >= (PVOID)0x10000) && (ptr < PTRMAXVAL) && ptr != nullptr && !IsBadReadPtr(ptr, sizeof(ptr));
	}

	static DWORD64 GetBase()
	{
		return (DWORD64)GetModuleHandle(NULL);
	}

};


#endif