#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <windows.h> 

// File Helper
// 파일 관련 읽고, 파싱하고 등등의 기능을 담당
// static 하게 짜자.

#define MAX_STRING_INPUT_SIZE 100

struct FFileHelper
{
	template<typename T>
	static void LoadFileToArrayWithDelimeter(const wchar_t* baseDir, const wchar_t* fileName, const wchar_t delimeter, int size, int* _rowSize, int* _colSize, T*** array)
	{
		wchar_t* wcsbuf = (wchar_t*)malloc(sizeof(wchar_t) * size);
		if (!wcsbuf)
			return;

		int num = swprintf(wcsbuf, size, L"../Resource/%s/%s", baseDir, fileName);

		// 파일 열기
		FILE* fp = nullptr;
		_wfopen_s(&fp, wcsbuf, L"rt");
		if (!fp) {
			fwprintf(stderr, L"파일 열기 실패: %s\n", wcsbuf);
			free(wcsbuf);
			return;
		}

		wchar_t wchSize[MAX_STRING_INPUT_SIZE];
		fgetws(wchSize, MAX_STRING_INPUT_SIZE, fp);
		int totalSize = _wtoi(wchSize);
		*_rowSize = totalSize;

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
				//if (!wcscmp(wchSize[i], delimeter))
				{
					colSize++;
				}
			}
			if(*_colSize == -1)
				*_colSize = colSize;
			else if (*_colSize != colSize)
			{
				printf("Please Check .txt file structure");
			}
			(*array)[rowIdx] = (T*)malloc(sizeof(T) * colSize);

			size_t g_cszBuff_size = wcslen(wchSize);
			wchar_t* context = NULL;
			wchar_t* ptr = wcstok_s(wchSize, &delimeter, &context);

			while (ptr != NULL)
			{
				int value = _wtoi(ptr);
				(*array)[rowIdx][colIdx] = value;
				
				colIdx++;
				ptr = wcstok_s(NULL, &delimeter, &context);
			}
			rowIdx++;
		}

		fclose(fp);
		free(wcsbuf);
	}

	template<typename T>
	static bool LoadFileToVectorWithDelimiter(const std::wstring& baseDir, const std::wstring& fileName, wchar_t delimiter, std::vector<std::vector<T>>& outArray)
	{
		// 경로 구성
		std::wstring fullPath = L"../Resource/" + baseDir + L"/" + fileName;

		// 파일 열기
		std::wifstream file(fullPath);
		if (!file.is_open()) {
			//std::wcerr << L"파일 열기 실패: " << fullPath << std::endl;
			return false;
		}

		outArray.clear();
		std::wstring line;

		// 첫 줄: 총 row 개수 (정보용, 실제로는 무시 가능)
		if (!std::getline(file, line))
			return false;

		int expectedRowCount = std::stoi(line);

		int expectedColSize = -1;
		while (std::getline(file, line)) {
			std::vector<T> row;
			std::wstringstream ss(line);
			std::wstring token;

			while (std::getline(ss, token, delimiter)) {
				if constexpr (std::is_same_v<T, int>) {
					row.push_back(std::stoi(token));
				}
				else if constexpr (std::is_same_v<T, float>) {
					row.push_back(std::stof(token));
				}
				else {
					row.push_back(static_cast<T>(token)); // 문자열 등
				}
			}

			if (expectedColSize == -1) {
				expectedColSize = static_cast<int>(row.size());
			}
			else if (expectedColSize != row.size()) {
				//std::wcerr << L"열 개수가 일치하지 않습니다. txt 파일 구조 확인 필요" << std::endl;
				return false;
			}

			outArray.push_back(std::move(row));
		}

		// 행 개수 검증
		if (outArray.size() != expectedRowCount) {
			//std::wcerr << L"행 개수가 예상과 다릅니다: " << outArray.size() << L" != " << expectedRowCount << std::endl;
		}

		return true;
	}

	struct ScoreEntry
	{
		int score;
		std::wstring time;

		bool operator<(const ScoreEntry& other) const {
			return score < other.score; // 점수 높은 순 정렬
		}
	};

	// 읽기 함수
	static void LoadFileString(const std::wstring& dir, std::vector<ScoreEntry>& outEntries)
	{
		std::wstring baseDir = L"../Resource";
		std::wstring targetDir = baseDir + dir;

		std::wifstream file(targetDir);
		if (!file.is_open()) return;

		outEntries.clear();
		std::wstring line;
		while (std::getline(file, line)) {
			std::wstringstream ss(line);
			ScoreEntry entry;
			ss >> entry.score >> entry.time;
			if (ss) outEntries.push_back(entry);
		}

		file.close();
	}

	// 쓰기 함수 - 상위 10개만 저장
	static void SaveScoreAndKeepTop10(const std::wstring& dir, int newScore)
	{
		std::wstring baseDir = L"../Resource";
		std::wstring targetDir = baseDir + dir;

		// 1. 기존 데이터 읽기
		std::vector<ScoreEntry> entries;
		LoadFileString(dir, entries);

		// 2. 현재 시간 얻기
		SYSTEMTIME st;
		GetLocalTime(&st);
		wchar_t timeBuffer[100];
		swprintf_s(timeBuffer, 100, L"%04d-%02d-%02d_%02d:%02d:%02d",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		// 3. 새 점수 추가
		entries.push_back({ newScore, timeBuffer });

		// 4. 정렬 후 상위 10개만 저장
		std::sort(entries.begin(), entries.end());

		std::wofstream file(targetDir, std::ios::trunc);
		if (!file.is_open()) return;

		int count = 0;
		for (const auto& entry : entries) {
			file << entry.score << L" " << entry.time << L"\n";
			if (++count >= 10) break;
		}

		file.close();
	}

};