// Chess analysis
//
// Copyright (C) 2021 Garyl Hester. All rights reserved.
// github.com/codefool/chess
//
// Released under the GNU General Public Licence Version 3, 29 June 2007
//
#pragma once
#include <iostream>
#include <memory>
#include <vector>

enum Side {
	SIDE_WHITE = 0,
	SIDE_BLACK = 1
};

enum EndGameReason {
	EGR_NONE               = 0x00,
	//
	EGR_CHECKMATE          = 0x01,
	EGR_14A_STALEMATE      = 0x02,
	EGR_14C_TRIPLE_OF_POS  = 0x03,
	EGR_14D_NO_MATERIAL    = 0x04,
	EGR_14D1_KVK           = 0x05,
	EGR_14D2_KVKWBN        = 0x06,
	EGR_14D3_KWBVKWB       = 0x07,
	EGR_14D4_NO_LGL_MV_CM  = 0x08,
	EGR_14F_50_MOVE_RULE   = 0x09
};

enum Rank {
	R1 = 0x00,
	R2 = 0x01,
	R3 = 0x02,
	R4 = 0x03,
	R5 = 0x04,
	R6 = 0x05,
	R7 = 0x06,
	R8 = 0x07
};

enum File {
	Fa = 0x00,
	Fb = 0x01,
	Fc = 0x02,
	Fd = 0x03,
	Fe = 0x04,
	Ff = 0x05,
	Fg = 0x06,
	Fh = 0x07
};

enum PieceType {
	PT_EMPTY     = 0x00,
	PT_KING      = 0x01,
	PT_QUEEN     = 0x02,
	PT_BISHOP    = 0x03,
	PT_KNIGHT    = 0x04,
	PT_ROOK      = 0x05,
	PT_PAWN      = 0x06, // on its own file
	PT_PAWN_OFF  = 0x07,  // off its own file
	SIDE_MASK    = 0x08,
	BLACK_MASK   = 0x08,
	PIECE_MASK   = 0x07
};

enum Dir {
	UP = 0,
	DN,
	LFT,
	RGT,
	UPR,
	UPL,
	DNR,
	DNL
};

struct Offset {
	short df;	// delta file
	short dr;	// delta rank
};

struct Vector {
	short  			 c;	// max number of moves
	std::vector<Dir> d;
};

union Nibbles {
	uint8_t b;
	struct {
		uint8_t lo : 4;
		uint8_t hi : 4;
	} f;
};

enum MoveAction {
	MV_MOVE = 0,
	MV_CAPTURE = 1,
	MV_CASTLE_KINGSIDE = 2,
	MV_CASTLE_QUEENSIDE = 3,
	MV_CHECK = 4,
	MV_CHECKMATE = 5,
	MV_EN_PASSANT = 6,
	// MV_UNUSED = 7,
	MV_PROMOTION_QUEEN = 8,
	MV_PROMOTION_BISHOP = 9,
	MV_PROMOTION_KNIGHT = 10,
	MV_PROMOTION_ROOK = 11
};


class Pos {
private:
	short _r;
	short _f;
public:
	Pos() : _r{0}, _f{0} {}

	Pos(Rank ra, File fi)
	{
		set(ra,fi);
	}

	Pos(short ra, short fi)
	: _r(ra), _f(fi)
	{}

	Pos(uint8_t b)
	{
		fromByte(b);
	}

	void set(short ra, short fi) {
		_r = ra;
		_f = fi;
	}

	void set(Rank ra, File fi) {
		set(static_cast<short>(ra), static_cast<short>(fi));
	}

	void set(uint8_t b) {
		fromByte(b);
	}

	inline Pos operator+(const Offset& o) {
		return Pos(_r + o.dr, _f + o.df);
	}

	inline Pos operator+=(const Offset& o) {
		_r += o.dr;
		_f += o.df;
		return *this;
	}

	Pos offset(Offset& o) {
		return Pos(_r + o.dr, _f + o.df);
	}

	uint8_t toByte() {
		return (uint8_t)(_r << 3 | _f);
	}

	void fromByte(uint8_t b) {
		_f = static_cast<short>(b & 0x07);
		_r = static_cast<short>((b >> 3) & 0x07);
	}

	const short r() const { return _r; }
	const short f() const { return _f; }

	inline Rank rank() { return static_cast<Rank>(_r); }
	inline File file() { return static_cast<File>(_f); }

	friend std::ostream& operator<<(std::ostream& os, const Pos& p);
};

class Piece;
typedef std::shared_ptr<Piece> PiecePtr;

union Move {
	uint8_t b;
	struct {
		uint8_t action    : 4;
		uint8_t source    : 6;
		uint8_t target    : 6;
	} f;

	Move() : b{0} {}
	Move(MoveAction a, Pos from, Pos to)
	{
		setAction(a);
		f.source = from.toByte();
		f.target = to.toByte();
	}
	void setAction(MoveAction a) { f.action = static_cast<uint8_t>(a); }
	MoveAction getAction() { return static_cast<MoveAction>(f.action); }

	friend std::ostream& operator<<(std::ostream& os, const Move& p);
};

typedef std::vector<Move>   MoveList;
typedef MoveList::iterator  MoveListItr;