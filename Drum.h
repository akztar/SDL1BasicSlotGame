#pragma once

#include <list>
#include <vector>
#include <iostream>
#include "include/GameEntity.hpp"
#include "include/SDLApp.hpp"

extern __declspec(selectany) std::vector<std::string> Tiles;
extern __declspec(selectany) SDL_Renderer* Renderer;

class Drum
{
public:
    Drum(int x = 0, int y = 0, int vs = 0, int tx = 160, int ty = 160);
	~Drum();
    void SetStats(int x = 0, int y = 0, int vs = 0, int tx = 160, int ty = 160);
	void StartSpin(int _top, int _mid, int _bot);
	void ForceStop();
	void SlowStop();
	void Step();
    bool Started();
    void Render();
private:
    bool spining;
    int baseShiftX;
    int baseShiftY;
    int tileX;
    int tileY;
    int vSpacer;
    int vvSpacer;
	bool ramp;
	bool fullStop;
	int drumShift;
    int stepsBeforeSlow;
	void tpb();
    std::auto_ptr<GameEntity> top;
    std::auto_ptr<GameEntity> mid;
    std::auto_ptr<GameEntity> bot;
    std::auto_ptr < std::list< std::auto_ptr<GameEntity>>> LOH;
	int spinSpeed; // reversed 
};



class DrumStak
{
public:
    DrumStak(int x = 0, int y = 0, int hs = 0, int vs = 0, int tx = 160, int ty = 160);
    ~DrumStak();
    void Step();
    void LeverPull();
private:
    void schStart(std::list<Drum>::iterator iter, int steps_waiting);
    std::list<Drum>::iterator waitingDrum;
    bool startedWaiting;
    int waitingSteps;
    int tileX;
    int tileY;
    int baseShiftX;
    int baseShiftY;
    int hSpacer;
    int vSpacer;
    std::list<Drum>* LOD;

};
