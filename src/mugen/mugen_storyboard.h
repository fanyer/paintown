#ifndef mugen_storyboard_h
#define mugen_storyboard_h

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "mugen_exception.h"
#include "mugen_fadetool.h"

class Bitmap;
class MugenItemContent;
class MugenSprite;
class MugenSound;
class MugenAnimation;
class MugenBackground;

struct MugenPoint{
    int x;
    int y;
};

class MugenLayer{
    public:
	MugenLayer();
	~MugenLayer();
	MugenPoint offset;
	int actionno;
	int startTime;
	MugenAnimation *animation;
	void act();
	void draw(const int xaxis, const int yaxis, Bitmap *bmp);
};

class MugenScene {
    public:
	MugenScene();
	~MugenScene();
	
	void act();
	
	void draw(Bitmap *bmp);
	
	MugenFadeTool fader;
	
	int clearColor;
	int ticker;
	int endTime;
	
	MugenPoint defaultAxis;
	
	std::string backgroundName;
	
	// Backgrounds
	std::vector<MugenBackground *>backgrounds;
	
	std::vector<MugenBackground *>foregrounds;
	
	// layers
	std::vector<MugenLayer *>layers;
};

class MugenStoryboard {
public:
	// Location at dataPath() + "mugen/stages/"
	MugenStoryboard( const string & s );

	~MugenStoryboard();
	
	void load() throw (MugenException);
	
	void run(Bitmap *bmp);
	
    private:
	
	std::string location;
	
	std::string spriteFile;
	
	int startscene;
	
	/* Sprites */
	std::map< unsigned int, std::map< unsigned int, MugenSprite * > > sprites;
	
	/* Animation Lists stored by action number */
	std::map< int, MugenAnimation * > animations;
	
	// Backgrounds
	std::vector<MugenScene *>scenes;
	
	void cleanup();
};

#endif
