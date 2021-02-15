#include <QApplication>
#include "context.h"
#include "main_window.h"
#include "loading_dialog.h"

int main(int argc, char* argv[])
{
	auto context = app::context();
	auto app = QApplication(argc, argv);

	// show the loading dialog
	auto loading = loading_dialog(&context);

	if (!loading.try_init())
	{
		loading.start_init_timer();

		if (loading.exec() != 1)
			return EXIT_FAILURE;
	}

	// show the main window
	auto window = main_window(&context);
	window.show();

	return QApplication::exec();
}
