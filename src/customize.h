#pragma once

#include <windows.h>
#include "customize_game_data.h"

namespace app
{
	class process;

	class customize
	{
		public:
			explicit customize(process* process);

			bool find_pointers();
			void find_items();
			bool find_categorize_data();
			bool ready() const;

			bool load_from_game();
			bool save_to_game();

			CCustomizeGameData* get_game_data();

			std::uint32_t get_burst_count() const;
			std::uint32_t get_turntable_count() const;
			std::uint32_t get_lane_cover_count() const;
			std::uint32_t get_judge_font_count() const;
			std::uint32_t get_note_skin_count() const;
			std::uint32_t get_full_combo_effect_count() const;
			std::uint32_t get_lane_beam_count() const;
		private:
			bool _resolved = false;
			process* _process = nullptr;
			PBYTE _game_data_ptr = nullptr;
			PBYTE _first_item_ptr = nullptr;
			std::uint32_t _burst_count = 0;
			std::uint32_t _turntable_count = 0;
			std::uint32_t _lane_cover_count = 0;
			std::uint32_t _judge_font_count = 0;
			std::uint32_t _note_skin_count = 0;
			std::uint32_t _full_combo_effect_count = 0;
			std::uint32_t _lane_beam_count = 0;
			CCustomizeGameData _customize_game_data = {};
	};
}