#include "process.h"
#include "util/find_pattern.h"
#include <tlhelp32.h>

namespace app
{
	/**
	 * Search for the game using its window title and get the process ID.
	 *
	 * @return Whether the process ID was found.
	 */
	bool process::find()
	{
		auto hwnd = FindWindowA("beatmania IIDX INFINITAS", "beatmania IIDX INFINITAS");

		GetWindowThreadProcessId(hwnd, &_pid);

		if (_pid == 0)
			return false;

		return true;
	}

	/**
	 * Attempt to open a handle to the process with the necessary permissions.
	 *
	 * Also find the start and end addresses of the 'bm2dx.exe' module, allowing
	 * us to only scan over relevant areas of memory later.
	 *
	 * @return Whether the process handle was opened and module information set.
	 */
	bool process::open()
	{
		if (_pid == 0 && !find())
			return false;

		_process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, true, _pid);

		if (_process == nullptr)
			return false;

		auto module = MODULEENTRY32 { .dwSize = sizeof(MODULEENTRY32) };
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _pid);

		if (snapshot == INVALID_HANDLE_VALUE)
			return EXIT_FAILURE;

		if (!Module32First(snapshot, &module))
			return false;

		_startRegion = module.modBaseAddr;
		_endRegion = module.modBaseAddr + module.modBaseSize;

		CloseHandle(snapshot);

		return true;
	}

	/**
	 * Performs a pattern scan on the remote process.
	 * If no results are found, this method will return nullptr.
	 *
	 * @param pattern 	Pattern to search for in "IDA" format. (e.g. XX XX ? ? XX XX)
	 * @return Pointer to where the pattern was found in the remote process.
	 */
	PBYTE process::find_pattern(const char* pattern)
	{
		auto pos = _startRegion;
		auto result = PBYTE {};
		auto info = MEMORY_BASIC_INFORMATION {};

		while (VirtualQueryEx(_process, pos, &info, sizeof(info)))
		{
			const auto start = pos;
			pos += info.RegionSize;

			if (info.State != MEM_COMMIT || info.Protect == PAGE_NOACCESS)
				continue;

			auto buffer = new BYTE[info.RegionSize];
			auto read = SIZE_T {0};

			if (ReadProcessMemory(_process, start, buffer, info.RegionSize, &read) && read > 0)
			{
				auto offset = FindPattern(buffer, buffer + info.RegionSize, pattern);

				if (offset != nullptr)
					result = (offset - buffer + start);
			}

			delete[] buffer;

			if (result != nullptr || pos > _endRegion)
				break;
		}

		return result;
	}

	/**
	 * Read memory from the remote process
	 *
	 * @param address	Pointer to memory in the remote process to read from.
	 * @param buffer 	Pointer to memory in the local process to write to.
	 * @param size 		How many bytes should be read from the remote process.
	 * @return Whether the read operation succeeded.
	 */
	bool process::read(PBYTE address, void* buffer, SIZE_T size)
	{
		auto in = SIZE_T {0};

		if (!ReadProcessMemory(_process, address, reinterpret_cast<PBYTE>(buffer), size, &in) || in != size)
			return false;

		return true;
	}

	/**
	 * Write memory to the remote process
	 *
	 * @param address	Pointer to memory in the remote process to write from.
	 * @param buffer 	Pointer to memory in the local process to read from.
	 * @param size 		How many bytes should be written to the remote process.
	 * @return Whether the write operation succeeded.
	 */
	bool process::write(PBYTE address, void* buffer, SIZE_T size)
	{
		auto out = SIZE_T {0};

		if (!WriteProcessMemory(_process, address, reinterpret_cast<PBYTE>(buffer), size, &out) || out != size)
			return false;

		return true;
	}
}