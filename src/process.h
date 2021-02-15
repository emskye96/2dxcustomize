#pragma once

#include <windows.h>

namespace app
{
	class process
	{
		public:
			process() = default;

			bool find();
			bool open();

			bool read(PBYTE address, void* buffer, SIZE_T size);
			bool write(PBYTE address, void* buffer, SIZE_T size);

			PBYTE find_pattern(const char* pattern);
		private:
			DWORD _pid = 0;
			PBYTE _startRegion = nullptr;
			PBYTE _endRegion = nullptr;
			HANDLE _process = nullptr;
	};
}