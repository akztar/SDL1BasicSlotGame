#include "Drum.h"


extern std::vector<std::string> Tiles;
extern SDL_Renderer* Renderer;

// TODO: solve memory leak

Drum::Drum(int x, int y, int vs, int tx, int ty):baseShiftX(x),baseShiftY(y), vSpacer(vs),
tileX(tx), tileY(ty), vvSpacer(vs + ty), top(nullptr), mid(nullptr), bot(nullptr),
drumShift(0), fullStop(true), spining(false), ramp(false), spinSpeed(4), stepsBeforeSlow(256)
{
    LOH = std::auto_ptr<std::list<std::auto_ptr<GameEntity>>>(new std::list<std::auto_ptr<GameEntity>>());
    LOH->push_front(std::auto_ptr <GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
    LOH->push_front(std::auto_ptr <GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
    LOH->push_front(std::auto_ptr <GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
    LOH->push_front(std::auto_ptr <GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
}

Drum::~Drum()
{
}

void Drum::SetStats(int x, int y, int vs, int tx, int ty)
{
    baseShiftX = x;
    baseShiftY = y;
    vSpacer = vs;
    tileX = tx;
    tileY = ty;
    vvSpacer = vs + ty;
}

void Drum::StartSpin(int _top, int _mid, int _bot)
{
	top = std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[_top]));
	mid = std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[_mid]));
	bot = std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[_bot]));
    if(LOH.get() == nullptr)
        LOH = std::auto_ptr<std::list<std::auto_ptr<GameEntity>>>(new std::list<std::auto_ptr<GameEntity>>());
    else
    {
        LOH->pop_back();
        LOH->pop_back();
        LOH->pop_back();
        LOH->pop_back();
    }
    LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
    LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
    LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
    LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
	spinSpeed = 4;
	ramp = true;
	fullStop = false;
    spining = true;
    stepsBeforeSlow = 256 + rand() % 256;
}

void Drum::ForceStop()
{
    std::cout << "-----------FS----------------" << std::endl << std::endl << std::endl << std::endl;
	LOH->pop_front();
	LOH->pop_front();
	LOH->pop_front();
	LOH->push_back(std::auto_ptr<GameEntity>(new GameEntity(*top)));
	LOH->push_back(std::auto_ptr<GameEntity>(new GameEntity(*mid)));
	LOH->push_back(std::auto_ptr<GameEntity>(new GameEntity(*bot)));
	drumShift = 0;
	fullStop = true;
    spining = false;
}

void Drum::SlowStop()
{
	ramp = false;
}

void Drum::Step()
{
    static int slowdown = 16;
	if (spining != false || fullStop == false)
    {
        stepsBeforeSlow--;
        if (stepsBeforeSlow == 0)
            SlowStop();
        drumShift += (64 == spinSpeed? 1: (64 - spinSpeed));
        if (drumShift >= 60)
        {
            drumShift = 0;
            if (slowdown > 0)
                slowdown--;
            else
            {
                ramp ? (spinSpeed < 64 ? spinSpeed >> 1 : ramp) : (spinSpeed > 8 ? spinSpeed << 1 : ramp);
                tpb(); // replace from the top 
                slowdown = 8;
            }
        }
    }
    
    int c = 0;
    for (std::list< std::auto_ptr<GameEntity>>::iterator i = LOH.get()->begin(); i != LOH.get()->end(); i++, c++)
    {
        if (c == 0 && spining == false)
            continue;
        int shiftY = ((stepsBeforeSlow% spinSpeed) * vvSpacer) >> 6;
        if (spining == false)
            shiftY = 0;
        std::cout << drumShift << "___" << vvSpacer << "   " << shiftY << std::endl;
        shiftY += c * ((vSpacer >> 1) + tileY);
        std::cout << c << " shift("<< (double)(drumShift * vvSpacer) / 60 << ") prerender coordinats:" << baseShiftX << "," << shiftY << " spin spd: " << spinSpeed << " steps: " << stepsBeforeSlow << std::endl;
        (*i)->GetTexturedRectangle().SetPosition(baseShiftX, shiftY);
        (*i)->GetTexturedRectangle().SetDimensions(tileX, tileY);
        (*i)->Render();
    }
}

bool Drum::Started()
{
    return spining;
}

void Drum::Render()
{
    if (spining == false)
        return;
    for (std::list<std::auto_ptr<GameEntity>>::iterator i = LOH.get()->begin(); i != LOH.get()->end(); i++)
        (*i)->Render();
}

void Drum::tpb()
{
	LOH->pop_back();
	if (ramp == false) // check for slow stop
		switch (spinSpeed)
		{
		case 64:
			LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(*bot)));
			break;
		case 32:
			LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(*mid)));
			break;
		case 16:
			LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(*top)));
			break;
		default:
			LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
			fullStop = true;
            spining = false;
			break;
		}
	else
        LOH->push_front(std::auto_ptr<GameEntity>(new GameEntity(Renderer, Tiles[rand() % Tiles.size()])));
}


DrumStak::DrumStak(int x, int y, int hs, int vs, int tx, int ty) : baseShiftX(x), baseShiftY(y), hSpacer(hs), vSpacer(vs), startedWaiting(false), tileX(160), tileY(160)
{
    LOD = new std::list<Drum>(5);
    int c = 0;
    for (std::list<Drum>::iterator i = LOD->begin(); i != LOD->end(); i++, c++)
    {
        int Xshift = (160 + hSpacer) * c;
        Xshift += baseShiftX;
        std::cout << "prerender coordinats:" << Xshift << "," << baseShiftY << std::endl;
        i->SetStats(Xshift, baseShiftY, vSpacer, 160, 160);
    }
}

DrumStak::~DrumStak()
{
    delete LOD;
}

void DrumStak::Step()
{
    if (waitingSteps > 0)
        waitingSteps--;
    else if(startedWaiting == true)
    {
        waitingDrum->StartSpin(rand() % Tiles.size(), rand() % Tiles.size(), rand() % Tiles.size()); // TODO: Replace with a proper generator function
        if (waitingDrum != LOD->end())
            schStart(++waitingDrum, rand() % 32);
        else
            startedWaiting = false;
    }

    for (std::list<Drum>::iterator i = LOD->begin();i!=LOD->end();i++)
        i->Step();
    for (std::list<Drum>::iterator i = LOD->begin(); i != LOD->end(); i++)
        i->Render();
}

void DrumStak::LeverPull()
{
    bool anyoneSpining = false;
    for (std::list<Drum>::iterator i = LOD->begin(); i != LOD->end(); i++)
        anyoneSpining |= i->Started();
    if (false == anyoneSpining)
        schStart(LOD->begin(), rand() % 32);
    else
        for (std::list<Drum>::iterator i = LOD->begin(); i != LOD->end(); i++)
            if (i->Started())
            {
                i->ForceStop();
                break;
            }

}

void DrumStak::schStart(std::list<Drum>::iterator iter, int steps_waiting)
{
    startedWaiting = true;
    waitingDrum = iter;
    waitingSteps = steps_waiting;
}
