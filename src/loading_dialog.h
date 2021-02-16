#pragma once

#include <QTimer>
#include <QDialog>
#include "context.h"

namespace Ui
{
	class loading_dialog;
}

class loading_dialog: public QDialog
{
	Q_OBJECT

	public:
		explicit loading_dialog(app::context* context = nullptr);
		~loading_dialog() override;

		bool try_init();
		void start_init_timer();
	private:
		bool _ready = false;
		app::context* _context = nullptr;
		QTimer* _timer = nullptr;
		Ui::loading_dialog* _ui = nullptr;
};
