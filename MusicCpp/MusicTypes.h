#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <algorithm>
#include <ranges>
#include <assert.h>
#include <iomanip>


class Note
{
public:
	Note() {}

	Note(std::string_view str)
	{
		if (!FromStr(str))
			throw std::exception{ "invalid note" };
	}

	Note(char letter, int accCnt)
		: m_letter{ letter }
		, m_accCnt{ accCnt }
	{
	}

	char GetLetter() const { return m_letter; }
	int GetAccCnt()  const { return m_accCnt; }

	bool Equal(Note const& rhs, bool bExact) const
	{
		if (bExact) return EqualExact(rhs);
		return EqualByIndex(rhs);
	}

	bool EqualByIndex(Note const& rhs) const
	{
		return ToIndex() == rhs.ToIndex();
	}

	bool EqualExact(Note const& rhs) const
	{
		return m_letter == rhs.m_letter && m_accCnt == rhs.m_accCnt;
	}

	// Init from a string like C# Db F## A  etc... 
	bool FromStr(std::string_view str)
	{
		if (str.size() == 0) return false;

		char letter = str[0];
		if (letter < 'A' || letter > 'G') return false;

		if (str.size() == 1)
		{
			m_letter = letter;
			m_accCnt = 0;
			return true;
		}

		if (str[1] == '#')  // sharp
		{
			for (size_t i = 2; i < str.size(); ++i) {
				if (str[i] != '#') return false;  // you cannot mix sharp with non-sharp
			}
			m_letter = letter;
			assert(str.size() > 1);
			m_accCnt = static_cast<int>(str.size()) - 1;
			return true;
		}

		if (str[1] == 'b')  // flat
		{
			for (size_t i = 2; i < str.size(); ++i) {
				if (str[i] != 'b') return false;   // you cannot mix flat with non-flat
			}
			m_letter = letter;
			assert(str.size() > 1);
			m_accCnt = 1 - static_cast<int>(str.size());
			return true;
		}

		return false;
	}

	// Convert to string like C# Db F## A  etc... 
	std::string ToStr() const
	{
		std::string str{ m_letter };
		if (m_accCnt > 0)  // sharp
		{
			for (int i = 0; i <  m_accCnt; ++i)   str += "#";
		}
		else if (m_accCnt < 0) // flat
		{
			for (int i = 0; i < -m_accCnt; ++i)  str += "b";
		}
		return str;
	}

	// Convert to string like C# Db F## A  etc... with additional debug info
	std::string ToStrDebug() const
	{
		std::string str = ToStr();
		str += " : " + std::to_string(ToIndex());
		return str;
	}

	// Convert to index 0..11 in the octave of C .. C
	int ToIndex() const
	{
		assert('A' <= m_letter && m_letter <= 'G');
		int idx = 0;
		switch (m_letter)
		{
		case 'C': idx = 0; break;
		case 'D': idx = 2; break;
		case 'E': idx = 4; break;
		case 'F': idx = 5; break;
		case 'G': idx = 7; break;
		case 'A': idx = 9; break;
		case 'B': idx = 11; break;
		}
		idx = (12 + idx + m_accCnt) % 12;
		return idx;
	}

	void RaiseSemitone()
	{
		m_accCnt += 1;
	}

	void LowerSemitone()
	{
		m_accCnt -= 1;
	}

private:
	char	m_letter{ 'C' };   // C D E F G A B
	int		m_accCnt{ 0 }; 	   // accidentals: number of semitones to adjust (0 for natural, <0 for flat, >0 for sharp)
};


class CircleOfFifth
{
public:
	struct Entry
	{
		Note			  m_root;			   // the root of the scale
		Note			  m_relMinorRoot;	   // the root of the relative minor scale
		bool			  m_isSharps{ false }; // e.g. true for D major scale
		std::vector<char> m_sharpsOrFlats{};   // e.g. {'F', 'C'} for D major scale
	};

	Entry const& GetEntry(Note const& note, bool bOnlyExact)
	{
		// first try to match exact:
		for (Entry const& entry : m_entriess)
		{
			if (note.Equal(entry.m_root, true))
				return entry;
		}
		if (!bOnlyExact)
		{
			// then try to match in-exact:
			for (Entry const& entry : m_entriess)
			{
				if (note.Equal(entry.m_root, false))
					return entry;
			}
		}
		throw std::exception(std::format("CircleOfFifth::GetEntry: couln't find {} (only exact: {})", note.ToStr(), bOnlyExact).c_str());
	}

	Entry const& GetEntryOfRelMajor(Note const& note, bool bOnlyExact)
	{
		// first try to match exact:
		for (Entry const& entry : m_entriess)
		{
			if (note.Equal(entry.m_relMinorRoot, true))
				return entry;
		}
		if (!bOnlyExact)
		{
			// then try to match in-exact:
			for (Entry const& entry : m_entriess)
			{
				if (note.Equal(entry.m_relMinorRoot, false))
					return entry;
			}
		}
		throw std::exception(std::format("CircleOfFifth::GetEntryOfRelMajor: couln't find {} (only exact: {})", note.ToStr(), bOnlyExact).c_str());
	}

	static CircleOfFifth& Instance()
	{
		static CircleOfFifth circleOfFifth;
		return circleOfFifth;
	}

private:
	CircleOfFifth()
		: m_entriess{
//          | root      | rel minor | is-sharp | sharps/flats 		   
			{ Note{"C"},  Note{"A"},  false,	 {}},
			{ Note{"G"},  Note{"E"},  true,		 {'F'}},
			{ Note{"D"},  Note{"B"},  true,		 {'F', 'C'}},
			{ Note{"A"},  Note{"F#"}, true,		 {'F', 'C', 'G'}},
			{ Note{"E"},  Note{"C#"}, true,		 {'F', 'C', 'G', 'D'}},
			{ Note{"B"},  Note{"G#"}, true,		 {'F', 'C', 'G', 'D', 'A'}},
			{ Note{"F#"}, Note{"D#"}, true,		 {'F', 'C', 'G', 'D', 'A', 'E'}},
			{ Note{"C#"}, Note{"Bb"}, true,		 {'F', 'C', 'G', 'D', 'A', 'E', 'B'}},
			{ Note{"Cb"}, Note{"G#"}, false,	 {'B', 'E', 'A', 'D', 'G', 'C', 'F'}},
			{ Note{"Gb"}, Note{"Eb"}, false,	 {'B', 'E', 'A', 'D', 'G', 'C'}},
			{ Note{"Db"}, Note{"Bb"}, false,	 {'B', 'E', 'A', 'D', 'G'}},
			{ Note{"Ab"}, Note{"F"},  false,	 {'B', 'E', 'A', 'D'}},
			{ Note{"Eb"}, Note{"C"},  false,	 {'B', 'E', 'A'}},
			{ Note{"Bb"}, Note{"G"},  false,	 {'B', 'E'}},
			{ Note{"F"},  Note{"D"},  false,	 {'B'}},
		}
	{}

	std::vector<Entry> m_entriess;
};


class Scale
{
public:
	enum class ScaleType : int  // values must be 0..n-1
	{
		Major = 0,
		NaturalMinor,
		HarmonicMinor,
		MelodicMinor,

		ScaleType_COUNT
	};

	static std::string ScaleTypeToStr(ScaleType type)
	{
		switch (type)
		{
		case ScaleType::Major: return "Major";
		case ScaleType::NaturalMinor: return "natural minor";
		case ScaleType::HarmonicMinor: return "harmonic minor";
		case ScaleType::MelodicMinor: return "melodic minor";
		default: assert(false); throw std::exception("ScaleTypeToStr FAILED: invalid type");
		}
	}

	static std::string GetName(ScaleType type, Note const& root)
	{
		std::string scaleName = root.ToStr() + " " + ScaleTypeToStr(type);
		return scaleName;
	}

	Scale(ScaleType type, Note const & root)
	{
		switch (type)
		{
		case ScaleType::Major: SetMajor(root); break;
		case ScaleType::NaturalMinor: SetNaturalMinor(root); break;
		case ScaleType::HarmonicMinor: SetHarmonicMinor(root); break;
		case ScaleType::MelodicMinor: SetMelodicMinor(root); break;
		default: assert(false); throw std::exception("Scale ctor FAILED: invalid type");
		}
	}

	std::string ToStr()
	{
		std::string str;
		for (auto const& note : m_notes)
		{
			str += note.ToStr() + " ";
		}
		return str;
	}

	std::array<Note, 7> const& GetNotes() const { return m_notes; }

private:

	void SetMajor(Note const& root)
	{
		auto const& COF_entry = CircleOfFifth::Instance().GetEntry(root, true /*bOnlyExact*/);
		char curLetter = root.GetLetter();
		for (int i = 0; i < 7; ++i)
		{
			int curAccCnt = 0;
			Note curNote;
			if (std::ranges::contains(COF_entry.m_sharpsOrFlats, curLetter))
			{
				curAccCnt = COF_entry.m_isSharps ? 1 : (-1);
			}
			m_notes[i] = Note{ curLetter, curAccCnt };
			curLetter++;
			if (curLetter > 'G')
			{
				curLetter = 'A';
			}
		}
	}

	void SetNaturalMinor(Note const& root)
	{
		auto const& COF_entryRelMaj = CircleOfFifth::Instance().GetEntryOfRelMajor(root, true /*bOnlyExact*/);
		Scale relativeMaj{ ScaleType::Major, COF_entryRelMaj.m_root };
		auto const& relativeMajNotes = relativeMaj.GetNotes();
		for (int i = 0; i < 7; ++i)
		{
			m_notes[i] = relativeMajNotes[(i - 2 + 7) % 7];
		}
	}

	void SetHarmonicMinor(Note const& root)
	{
		SetNaturalMinor(root);

		// Raise the 7th (using zero based index 6):
		m_notes[6].RaiseSemitone();
	}

	void SetMelodicMinor(Note const& root)
	{
		SetNaturalMinor(root);

		// Raise the 6th and 7th (using zero based index 5 and 6):
		m_notes[5].RaiseSemitone();
		m_notes[6].RaiseSemitone();
	}


	std::array<Note, 7> m_notes;
};

