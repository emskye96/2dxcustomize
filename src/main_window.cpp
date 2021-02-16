#include <tuple>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QTextStream>
#include "main_window.h"
#include "ui_main_window.h"

main_window::main_window(app::context* context):
	_context(context), _ui(new Ui::main_window), _scene(new QGraphicsScene),
	_banner(new QGraphicsPixmapItem(QPixmap(":/graphics/banner.png")))
{
	_ui->setupUi(this);

	// Load the somewhat pointless banner image.
	_ui->banner->setScene(_scene);
	_scene->addItem(_banner);

	// Populate combobox contents.
	auto customize = _context->get_customize();
	auto customize_data = customize->get_game_data();

	for (auto [object, filename, count]: { std::tuple
		{_ui->burst_items, ":/items/Burst.txt", customize->get_burst_count()},
		{_ui->turntable_items, ":/items/Turntable.txt", customize->get_turntable_count()},
		{_ui->lane_cover_items, ":/items/LaneCover.txt", customize->get_lane_cover_count()},
		{_ui->judge_font_items, ":/items/JudgeFont.txt", customize->get_judge_font_count()},
		{_ui->note_skin_items, ":/items/NoteSkin.txt", customize->get_note_skin_count()},
		{_ui->full_combo_effect_items, ":/items/FullComboEffect.txt", customize->get_full_combo_effect_count()},
		{_ui->lane_beam_items, ":/items/LaneBeam.txt", customize->get_lane_beam_count()},
	})
	{
		auto file = QFile(filename);
		auto names = QMap<std::uint32_t, QString> {};

		// Try to read item names from the resource text file.
		if (file.open(QIODevice::ReadOnly))
		{
			auto in = QTextStream(&file);

			while (!in.atEnd())
			{
				auto line = in.readLine();

				if (line.isEmpty())
					continue;

				auto delimiter = line.indexOf(":");

				if (delimiter == -1)
					continue;

				auto id = line.left(delimiter).toInt();
				auto name = line.mid(delimiter + 1, -1);

				names[id] = name;
			}
		}

		for (auto i = 0; i < count; ++i)
		{
			if (!names.contains(i))
				object->addItem(QString("Unknown (ID: %1)").arg(i), i);
			else
				object->addItem(names[i], i);
		}
	}

	// Reload settings from the game on 'Load' button click.
	connect(_ui->refresh_button, &QPushButton::clicked, [=] ()
	{
		if (!customize->load_from_game())
		{
			QMessageBox::warning(this, "Error", "Failed to load data. Is the game still running?");
			return;
		}

		_ui->burst_items->setCurrentIndex(customize_data->get_burst());
		_ui->turntable_items->setCurrentIndex(customize_data->get_turntable());
		_ui->lane_cover_items->setCurrentIndex(customize_data->get_lane_cover());
		_ui->judge_font_items->setCurrentIndex(customize_data->get_judge_font());
		_ui->note_skin_items->setCurrentIndex(customize_data->get_note_skin());
		_ui->full_combo_effect_items->setCurrentIndex(customize_data->get_full_combo_effect());
		_ui->lane_beam_items->setCurrentIndex(customize_data->get_lane_beam());
	});

	// Write settings to the game on 'Save' button click.
	connect(_ui->save_button, &QPushButton::clicked, [=] ()
	{
		customize_data->set_burst( _ui->burst_items->itemData(_ui->burst_items->currentIndex()).toUInt());
		customize_data->set_turntable( _ui->turntable_items->itemData(_ui->turntable_items->currentIndex()).toUInt());
		customize_data->set_lane_cover( _ui->lane_cover_items->itemData(_ui->lane_cover_items->currentIndex()).toUInt());
		customize_data->set_judge_font( _ui->judge_font_items->itemData(_ui->judge_font_items->currentIndex()).toUInt());
		customize_data->set_note_skin( _ui->note_skin_items->itemData(_ui->note_skin_items->currentIndex()).toUInt());
		customize_data->set_full_combo_effect( _ui->full_combo_effect_items->itemData(_ui->full_combo_effect_items->currentIndex()).toUInt());
		customize_data->set_lane_beam( _ui->lane_beam_items->itemData(_ui->lane_beam_items->currentIndex()).toUInt());

		if (!customize->save_to_game())
			QMessageBox::warning(this, "Error", "Failed to save changes. Is the game still running?");
	});

	// Load settings from the game before the window is shown.
	_ui->refresh_button->click();
}

main_window::~main_window()
{
	delete _ui;
}
