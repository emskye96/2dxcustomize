#include <QString>
#include "process.h"
#include "customize.h"

namespace app
{
	customize::customize(app::process* process):
		_process(process) {}

	/**
	 * Searches for code patterns inside the 'bm2dx.exe' module.
	 *
	 * @return Whether all patterns were found successfully.
	 */
	bool customize::find_pointers()
	{
		auto game_data_ptr = _process->find_pattern("48 8B 05 ? ? ? ? 41 B8 02");
		auto first_item_ptr = _process->find_pattern("49 31 31 30 30 30 30 30"); // "I1100000"
		
		if (game_data_ptr == nullptr || first_item_ptr == nullptr)
			return false;

		// Calculate the relative displacement to get the "absolute" address.
		auto displ = std::uint32_t {0};
		
		if (!_process->read(game_data_ptr + 3, &displ, sizeof(std::uint32_t)))
			return false;
		
		_game_data_ptr = (game_data_ptr + displ + 7);
		_first_item_ptr = first_item_ptr;
		
		return true;
	}

	/**
	 * Determine how many of each item exists by reading until we hit invalid data.
	 */
	void customize::find_items()
	{
		char buffer[16] = {0};
		auto item_ptr = _first_item_ptr;

		while (true)
		{
			// Read string from the game process.
			if (!_process->read(item_ptr, buffer, sizeof(buffer)))
				break;

			// Check if this is a "valid" item. (begins with 'I')
			// If it doesn't, we're already past the end and can stop.
			if (buffer[0] != 'I')
				break;

			auto item_id = QString { buffer };

			if (item_id.startsWith("I11"))
				_burst_count++;
			else if (item_id.startsWith("I12"))
				_turntable_count++;
			else if (item_id.startsWith("I13"))
				_lane_cover_count++;
			else if (item_id.startsWith("I14"))
				_judge_font_count++;
			else if (item_id.startsWith("I15"))
				_note_skin_count++;
			else if (item_id.startsWith("I16"))
				_full_combo_effect_count++;
			else if (item_id.startsWith("I17"))
				_lane_beam_count++;
			else
				break;

			// Prepare to read the next string.
			item_ptr += sizeof(buffer);
		}
	}

	/**
	 * De-references the CCustomizeGameData** pointer, leaving us with the actual address
	 * of the class that we can read and write to in the future.
	 *
	 * This pointer is NULL until some time during the 'NOW LOADING...' screen, so this should
	 * be called multiple times until it returns true.
	 *
	 * @return Whether the pointer was updated.
	 */
	bool customize::find_categorize_data()
	{
		if (_resolved)
			return true;

		auto address = PBYTE { nullptr };

		if (!_process->read(_game_data_ptr, &address, sizeof(PBYTE)) || address == nullptr)
			return false;

		_game_data_ptr = address;
		_resolved = true;

		return true;
	}

	/**
	 * @return Whether the class is ready for use.
	 */
	bool customize::ready() const
	{
		return _resolved;
	}

	/**
	 * Reads the CCustomizeGameData structure from the game process.
	 *
	 * @return Whether the read operation succeeded.
	 */
	bool customize::load_from_game()
	{
		return _process->read(_game_data_ptr, &_customize_game_data, sizeof(CCustomizeGameData));
	}

	/**
	 * Writes the local CCustomizeGameData structure to the game process.
	 *
	 * @return Whether the write operation succeeded.
	 */
	bool customize::save_to_game()
	{
		return _process->write(_game_data_ptr, &_customize_game_data, sizeof(CCustomizeGameData));
	}

	/**
	 * Retrieves a pointer to the local CCustomizeGameData.
	 *
	 * @return A pointer to the local CCustomizeGameData structure.
	 */
	CCustomizeGameData* customize::get_game_data()
	{
		return &_customize_game_data;
	};

	/**
	 * @return How many burst items were discovered.
	 */
	std::uint32_t customize::get_burst_count() const
	{
		return _burst_count;
	}

	/**
	 * @return How many turntable items were discovered.
	 */
	std::uint32_t customize::get_turntable_count() const
	{
		return _turntable_count;
	}

	/**
	 * @return How many lane cover items were discovered.
	 */
	std::uint32_t customize::get_lane_cover_count() const
	{
		return _lane_cover_count;
	}

	/**
	 * @return How many judge font items were discovered.
	 */
	std::uint32_t customize::get_judge_font_count() const
	{
		return _judge_font_count;
	}

	/**
	 * @return How many note skin items were discovered.
	 */
	std::uint32_t customize::get_note_skin_count() const
	{
		return _note_skin_count;
	}

	/**
	 * @return How many full combo effect items were discovered.
	 */
	std::uint32_t customize::get_full_combo_effect_count() const
	{
		return _full_combo_effect_count;
	}

	/**
	 * @return How many lane beam items were discovered.
	 */
	std::uint32_t customize::get_lane_beam_count() const
	{
		return _lane_beam_count;
	}
}
