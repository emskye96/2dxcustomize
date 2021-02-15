#pragma once

#include <cstdint>

/**
 * Partial representation of the CCustomizeGameData game class.
 *
 * Since only one player can be active at a time in INFINITAS, we always read the P1
 * value regardless of which side is active, then write to both when making changes.
 */
struct CCustomizeGameData
{
	void* vftable; //0x0000
	std::int32_t p1_note_skin; //0x0008
	std::int32_t p1_burst; //0x000C
	std::int32_t p1_turntable; //0x0010
	std::int32_t p1_lane_cover; //0x0014
	std::int32_t p1_full_combo_effect; //0x0018
	std::int32_t p1_lane_beam; //0x001C
	std::uint8_t pad_0020[4]; //0x0020
	std::int32_t p1_judge_font; //0x0024
	std::int32_t p2_note_skin; //0x0028
	std::int32_t p2_burst; //0x002C
	std::int32_t p2_turntable; //0x0030
	std::int32_t p2_lane_cover; //0x0034
	std::int32_t p2_full_combo_effect; //0x0038
	std::int32_t p2_lane_beam; //0x003C
	std::uint8_t pad_0040[4]; //0x0040
	std::int32_t p2_judge_font; //0x0044

	/**
	 * @return The current burst item index.
	 */
	std::uint32_t get_burst() const
	{
		return p1_burst;
	}

	/**
	 * @return The current turntable item index.
	 */
	std::uint32_t get_turntable() const
	{
		return p1_turntable;
	}

	/**
	 * @return The current lane cover item index.
	 */
	std::uint32_t get_lane_cover() const
	{
		return p1_lane_cover;
	}

	/**
	 * @return The current judge font item index.
	 */
	std::uint32_t get_judge_font() const
	{
		return p1_judge_font;
	}

	/**
	 * @return The current note skin index.
	 */
	std::uint32_t get_note_skin() const
	{
		return p1_note_skin;
	}

	/**
	 * @return The current full combo effect item index.
	 */
	std::uint32_t get_full_combo_effect() const
	{
		return p1_full_combo_effect;
	}

	/**
	 * @return The current lane beam item index.
	 */
	std::uint32_t get_lane_beam() const
	{
		return p1_lane_beam;
	}

	/**
	 * @return The new burst item index.
	 */
	void set_burst(std::uint32_t value)
	{
		p1_burst = value;
		p2_burst = value;
	}

	/**
	 * @return The new turntable item index.
	 */
	void set_turntable(std::uint32_t value)
	{
		p1_turntable = value;
		p2_turntable = value;
	}

	/**
	 * @return The new lane cover item index.
	 */
	void set_lane_cover(std::uint32_t value)
	{
		p1_lane_cover = value;
		p2_lane_cover = value;
	}

	/**
	 * @return The new judge font item index.
	 */
	void set_judge_font(std::uint32_t value)
	{
		p1_judge_font = value;
		p2_judge_font = value;
	}

	/**
	 * @return The new note skin index.
	 */
	void set_note_skin(std::uint32_t value)
	{
		p1_note_skin = value;
		p2_note_skin = value;
	}

	/**
	 * @return The new full combo effect item index.
	 */
	void set_full_combo_effect(std::uint32_t value)
	{
		p1_full_combo_effect = value;
		p2_full_combo_effect = value;
	}

	/**
	 * @return The new lane beam item index.
	 */
	void set_lane_beam(std::uint32_t value)
	{
		p1_lane_beam = value;
		p2_lane_beam = value;
	}
};

static_assert(sizeof(CCustomizeGameData) == 0x48);