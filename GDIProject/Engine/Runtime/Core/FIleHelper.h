#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// File Helper
// 파일 관련 읽고, 파싱하고 등등의 기능을 담당
// static 하게 짜자.

#define MAX_STRING_INPUT_SIZE 100

struct FFileHelper
{
	template<typename T>
	static void LoadFileToArrayWithDelimeter(const wchar_t* baseDir, const wchar_t* fileName, const wchar_t delimeter, int size, T*** array)
	{
		wchar_t* wcsbuf = (wchar_t*)malloc(sizeof(wchar_t) * size);
		if (!wcsbuf)
			return;

		int num = swprintf(wcsbuf, size, L"../Resource/%s/%s", baseDir, fileName);

		// 파일 열기
		FILE* fp = _wfopen(wcsbuf, L"rt");
		if (!fp) {
			fwprintf(stderr, L"파일 열기 실패: %s\n", wcsbuf);
			free(wcsbuf);
			return;
		}

		wchar_t wchSize[MAX_STRING_INPUT_SIZE];
		fgetws(wchSize, MAX_STRING_INPUT_SIZE, fp);
		int totalSize = _wtoi(wchSize);

		//int** pArray = dynamic_cast<int**>(array);
		//if (pArray)
		//{
		//	pArray = (int*)malloc(sizeof(int) * totalSize);
		//}

		*array = (T**)malloc(sizeof(T*) * totalSize);
		int rowIdx = 0;
		// array 파싱 로직
		while (true)
		{
			memset(wchSize, L'\0', MAX_STRING_INPUT_SIZE);
			fgetws(wchSize, MAX_STRING_INPUT_SIZE, fp);
			if (feof(fp)) break;


			int colIdx = 0;
			int colSize = 1;
			for (int i = 0; i < MAX_STRING_INPUT_SIZE; i++)
			{
				if (wchSize[i] == '\0') break;
				if (wchSize[i] == delimeter)
				{
					colSize++;
				}
			}
			(*array)[colIdx] = (T*)malloc(sizeof(T) * colSize);

			size_t g_cszBuff_size = wcslen(wchSize);
			wchar_t* context = NULL;
			wchar_t* ptr = wcstok_s(g_cszBuff, ",", &context);

			while (ptr != NULL)
			{
				int value = _wtoi(ptr);
				(*array)[rowIdx][colIdx] = value;
				
				colIdx++;
				ptr = wcstok_s(NULL, ",", &context);
			}
			rowIdx++;
		}

		fclose(fp);
		free(wcsbuf);
	}

};