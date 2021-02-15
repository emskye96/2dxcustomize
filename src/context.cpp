#include "context.h"

namespace app
{
	context::context()
	{
		_process = std::make_unique<process>();
		_customize = std::make_unique<customize>(get_process());
	}

	/**
	 * @return Pointer to the process class.
	 */
	process* context::get_process()
	{
		return _process.get();
	}

	/**
	 * @return Pointer to the customize class.
	 */
	customize* context::get_customize()
	{
		return _customize.get();
	}
}