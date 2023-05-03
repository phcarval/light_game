#ifndef MAIN_LEVELS_H
#define MAIN_LEVELS_H

#include <string>

#define NB_LEVELS 2

typedef int LevelNb;
enum {SINGLE_LEVEL = -1};
// -1 = not a main level

class MainLevels
{
public:
	static std::string getLevel(LevelNb lvlNb);

	static void unlockNewLevel(LevelNb levelSolved);

	static int getUnlockedLevels();
	static int getNumberOfLevels() {return _nbLevels;}

	static void resetUnlockedLevels();

private:
	static const int _nbLevels = NB_LEVELS;
	static int _levelsUnlocked;

	static void loadUnlockedLevels();
	static void saveUnlockedLevels();
};

#endif //MAIN_LEVELS_H