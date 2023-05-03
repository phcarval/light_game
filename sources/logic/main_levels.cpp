#include <fstream>
#include <iostream>

#include "logic/main_levels.h"

using namespace std;

int MainLevels::_levelsUnlocked = -1;

void MainLevels::loadUnlockedLevels()
{
	string saveFile("/home/philippe/Documents/jeu de la mort/bin/savefile.sv");
	ifstream str;

	str.open(saveFile, ifstream::in);
	if(!str.is_open())
	{
		cout << "Error while opening" << endl;
		_levelsUnlocked = 1;
	}
	else
	{
		str.read((char*)&_levelsUnlocked,sizeof(_levelsUnlocked));
		str.close();
	}
}

void MainLevels::unlockNewLevel(LevelNb levelSolved)
{
	if(levelSolved == _levelsUnlocked && _levelsUnlocked < _nbLevels)
	{
		_levelsUnlocked++;
		saveUnlockedLevels();
	}
}

void MainLevels::saveUnlockedLevels()
{
	string saveFile("/home/philippe/Documents/jeu de la mort/bin/savefile.sv");
	ofstream str;

	str.open(saveFile, ifstream::out);
	if(!str.is_open())
	{
		cout << "Error while opening" << endl;
	}

	str.write((char*)&_levelsUnlocked,sizeof(_levelsUnlocked));

	str.close();
}

void MainLevels::resetUnlockedLevels()
{
	_levelsUnlocked = 1;
	saveUnlockedLevels();
}

string MainLevels::getLevel(LevelNb lvlNb)
{
	return "/home/philippe/Documents/jeu de la mort/bin/levels/level_" + to_string(lvlNb) + ".lvl";
}

int MainLevels::getUnlockedLevels()
{
	if(_levelsUnlocked == -1)
		loadUnlockedLevels();
	
	return _levelsUnlocked;
}
