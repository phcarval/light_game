#ifndef COMMON_H
#define COMMON_H

#include <utility>
#include <unordered_map>
#include <string>

/* Mouse buttons and scrolling enums */

typedef int M_BUTTON;

enum {M_DOWN = 0, M_UP = 1};
enum {M_RIGHT = 0, M_LEFT = 1};

#define N_MOUSE_BUTTONS 2
#define OTHER_BUTTON(but) 1-but

/* Position struct for positions and helper function */

struct Position
{
	int x;
	int y;

	Position(int nX=0, int nY=0) : x(nX), y(nY) {}
	Position(std::pair<int,int> pair) : x(pair.first), y(pair.second) {}

	Position operator+(const Position& nPos) const {return Position(x+nPos.x,y+nPos.y);}
	Position operator-(const Position& nPos) const {return Position(x-nPos.x,y-nPos.y);}
	Position& operator=(const Position& nPos) {x=nPos.x; y=nPos.y; return *this;}
	
	bool operator==(const Position& nPos) const {return (x == nPos.x && y == nPos.y);}
	bool operator<(const Position& nPos) const {return (x < nPos.x && y < nPos.y);}
	bool operator>(const Position& nPos) const {return (x > nPos.x && y > nPos.y);}

	struct TileComp // used for sets of Position
	{
		bool operator()(const Position& first, const Position& second)
		{return first.x < second.x ? true : (first.x > second.x ? false : (first.y < second.y ? true : false));}
	};

	std::string to_string() const {return std::to_string(x) + "," + std::to_string(y);}
};

namespace Utils
{
	double distance(const Position& a, const Position& b);
}

typedef std::array<bool,N_MOUSE_BUTTONS> MouseBool;
typedef std::array<Position,N_MOUSE_BUTTONS> ClickBool;

#endif //COMMON_H