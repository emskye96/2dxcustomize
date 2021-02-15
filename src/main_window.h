#pragma once

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include "context.h"

namespace Ui
{
	class main_window;
}

class main_window: public QMainWindow
{
	Q_OBJECT

	public:
		explicit main_window(app::context* context = nullptr);
		~main_window() override;
	private:
		app::context* _context = nullptr;
		QGraphicsScene* _scene = nullptr;
		QGraphicsPixmapItem* _banner = nullptr;
		Ui::main_window* _ui = nullptr;
};