#include <QMessageBox>
#include "loading_dialog.h"
#include "ui_loading_dialog.h"

loading_dialog::loading_dialog(app::context* context):
	_context(context), _timer(new QTimer(this)), _ui(new Ui::loading_dialog)
{
	_ui->setupUi(this);

	// Exit when the close button is clicked.
	connect(_ui->close_button, &QPushButton::clicked, [] () { exit(EXIT_FAILURE); });
}

loading_dialog::~loading_dialog()
{
	delete _ui;
}

/**
 * Tries to initialise the process and customize classes.
 *
 * @return Whether the application is ready to be used.
 */
bool loading_dialog::try_init()
{
	auto process = _context->get_process();
	auto customize = _context->get_customize();

	if (!customize->ready())
	{
		_ui->state_text->setText("Waiting for game process..");

		if (!process->open())
			return false;

		_ui->state_text->setText("Searching for code patterns..");

		if (!customize->find_pointers())
		{
			QMessageBox::critical(this, "Error", "A required code pattern was not be found.");
			exit(EXIT_FAILURE);
		}
	}

	_ui->state_text->setText("Waiting for class initialization..");

	if (!customize->find_categorize_data())
		return false;
	
	customize->find_items();

	_ready = true;
	return true;
}

/**
 * Schedules a 'try_init' attempt in 1 second.
 */
void loading_dialog::start_init_timer()
{
	connect(_timer, &QTimer::timeout, this, [=] ()
	{
		if (!_ready && !try_init())
			return _timer->start();

		_timer->deleteLater();

		return this->accept();
	});

	_timer->setSingleShot(true);
	_timer->setInterval(1000);
	_timer->start();
}