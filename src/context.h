#pragma once

#include <memory>
#include "customize.h"
#include "process.h"

namespace app
{
	class process;
	class customize;

	class context
	{
		public:
			context();

			process* get_process();
			customize* get_customize();
		private:
			std::unique_ptr<process> _process;
			std::unique_ptr<customize> _customize;
	};
}