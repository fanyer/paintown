#include <allegro.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <vector>
#include <map>

#include "globals.h"
#include "mugen_reader.h"
#include "mugen_section.h"
#include "mugen_item_content.h"
#include "mugen_item.h"
#include "mugen_character.h"
#include "mugen_animation.h"
#include "mugen_sprite.h"
#include "mugen_stage.h"

#include "util/bitmap.h"
#include "util/funcs.h"

#include "factory/font_render.h"

#include "gui/keyinput_manager.h"

using namespace std;

static void showCollision( const std::vector< MugenArea > &vec, Bitmap &bmp, int x, int y, int color, int &start ){
    int next = start;
    for( unsigned int i = 0; i < vec.size(); ++i ){
	bmp.rectangle( x + vec[i].x1, y + vec[i].y1, x + vec[i].x2, y + vec[i].y2, color );
	Font::getDefaultFont().printf( 15, 310 + next, color, bmp, "CLSN: x1(%i),y1(%i),x2(%i),y2(%i)",0, vec[i].x1,vec[i].y1,vec[i].x2,vec[i].y2 );
	next+=10;
    }
    start = next;
}

static bool isArg( const char * s1, const char * s2 ){
	return strcasecmp( s1, s2 ) == 0;
}

static void showOptions(){
    Global::debug(0) << "M.U.G.E.N. Config Reader:" << endl;
    Global::debug(0) << "-f <file>: Load a M.U.G.E.N. config file and output to screen." << endl;
    Global::debug(0) << "-c <name>: Load a M.U.G.E.N. character and output some data about it.\n         ie: 'data/mugen/chars/name' only pass name." << endl;
    Global::debug(0) << "-s <name>: Load a M.U.G.E.N. stage and output some data about it.\n         ie: 'data/mugen/stages/name.def' only pass name.def." << endl;
    Global::debug(0) << "-l <level>: Set debug level." << endl;
    Global::debug(0) << endl;
}

/* testing testing 1 2 3 */
void testPCX(){
    unsigned char data[1 << 18];
    FILE * f = fopen("x.pcx", "r");
    int length;
    length = fread(data, sizeof(char), 1<<18, f);
    Global::debug(0) << "Size is " << length << endl;
    fclose(f);
    Bitmap b = Bitmap::memoryPCX(data, length);
    // Bitmap b("x.pcx");
    if (b.getError()){
        Global::debug(0) << "what the hell" << endl;
    }
    Bitmap work(640, 480);
    work.circleFill(40, 40, 100, Bitmap::makeColor(255, 0, 0));
    b.draw(0, 0, work);
    Global::debug(0) << "Width " << b.getWidth() << " Height " << b.getHeight() << endl;
    work.BlitToScreen();
    Util::rest(3000);
}

void showCharacter(const string & ourFile){
    set_color_depth(16);
    Bitmap::setGfxModeWindowed(640, 480);
    Global::debug(0) << "Trying to load character: " << ourFile << "..." << endl;
    MugenCharacter character( ourFile );
    character.load();
    Global::debug(0) << "Loaded character: \"" << character.getName() << "\" successfully." << endl;
    bool quit = false;
    bool animate = false;
    bool showClsn1 = false;
    bool showClsn2 = false;
    bool moveImage = false;
   
    map<int,MugenAnimation*>::const_iterator it = character.getAnimations().begin();
    unsigned int currentAnim = 0;
    unsigned int lastAnim = character.getAnimations().size() -1;
    unsigned int currentFrame = 0;
    
    if (it->second->getFrames().size() == 0){
        Global::debug(0) << "No frames!" << endl;
        exit(0);
    }

    Bitmap work( 640, 480 );
    
    int xaxis = 260;
    int yaxis = 230;

    while( !quit ){
        work.clear();
        keyInputManager::update();
	if( animate ) it->second->logic();
        
        if( keyInputManager::keyState(keys::UP, true) ){
            if( currentAnim < lastAnim ){
                currentAnim++;
                it++;
            }
            currentFrame = 0;
        }
        else if( keyInputManager::keyState(keys::DOWN, true) ){
            if( currentAnim > 0 ){
                currentAnim--;
                it--;
            }
            currentFrame = 0;
        }
        else if( keyInputManager::keyState(keys::LEFT, true) && !animate){
	    it->second->forwardFrame();
	    currentFrame = it->second->getCurrentPosition();
        }
        else if( keyInputManager::keyState(keys::RIGHT, true) && !animate){
	    it->second->backFrame();
	    currentFrame = it->second->getCurrentPosition();
        }
        else if( keyInputManager::keyState(keys::SPACE, true) ){
            animate = !animate;
        }
        else if( keyInputManager::keyState('a', true) ){
            showClsn1 = !showClsn1;
	    it->second->toggleOffense();
        }
        else if( keyInputManager::keyState('d', true) ){
            showClsn2 = !showClsn2;
	    it->second->toggleDefense();
        }
	
	if( mouse_b & 1 )moveImage = true;
	else if( !(mouse_b & 1) )moveImage = false;
	
        quit |= keyInputManager::keyState(keys::ESC, true );
	
	if( moveImage ){ xaxis=mouse_x; yaxis =mouse_y; }
	it->second->render( xaxis, yaxis, work );
	
	int start = 10;
	if( showClsn2 )showCollision( it->second->getCurrentFrame()->defenseCollision, work, xaxis, yaxis, Bitmap::makeColor( 0,255,0 ), start  );
	if( showClsn1 )showCollision( it->second->getCurrentFrame()->attackCollision, work, xaxis, yaxis,  Bitmap::makeColor( 255,0,0 ), start  );
	
        Font::getDefaultFont().printf( 15, 250, Bitmap::makeColor( 255, 255, 255 ), work, "Current Animation: %i (%s), Frame: %i, xoffset: %i, yoffset: %i", 0, it->first, MugenAnimation::getName(MugenAnimationType(it->first)).c_str() ,currentFrame, it->second->getFrames()[currentFrame]->xoffset, it->second->getFrames()[currentFrame]->yoffset );
        if(it->second->getCurrentFrame()->sprite!=0)Font::getDefaultFont().printf( 15, 270, Bitmap::makeColor( 255, 255, 255 ), work, "Length: %d | x-axis: %d | y-axis: %d | Group: %d | Image: %d",0, it->second->getCurrentFrame()->sprite->length, it->second->getCurrentFrame()->sprite->x, it->second->getCurrentFrame()->sprite->y, it->second->getCurrentFrame()->sprite->groupNumber, it->second->getCurrentFrame()->sprite->imageNumber);
        Font::getDefaultFont().printf( 15, 280, Bitmap::makeColor( 255, 255, 255 ), work, "Bitmap info - Width: %i Height: %i",0, it->second->getCurrentFrame()->bmp->getWidth(), it->second->getCurrentFrame()->bmp->getHeight() );
        Font::getDefaultFont().printf( 15, 290, Bitmap::makeColor( 255, 255, 255 ), work, "(space) Animation enabled:            %i",0, animate );
        Font::getDefaultFont().printf( 15, 300, Bitmap::makeColor( 255, 255, 255 ), work, "(d)     Show Defense enabled (green): %i",0, showClsn2 );
        Font::getDefaultFont().printf( 15, 310, Bitmap::makeColor( 255, 255, 255 ), work, "(a)     Show Attack enabled (red):    %i",0, showClsn1 );
	
	show_mouse(work.getBitmap());

        work.BlitToScreen();
        Util::rest(1);
    }

}

void showStage(const string & ourFile){
    set_color_depth(16);
    Bitmap::setGfxModeWindowed(640, 480);
    Global::debug(0) << "Trying to load stage: " << ourFile << "..." << endl;
    MugenStage stage( ourFile );
    stage.load();
    Global::debug(0) << "Loaded stage: \"" << stage.getName() << "\" successfully." << endl;
    bool quit = false;
    
    Bitmap work( 320, 240 );
    Bitmap back( 640, 480 );
    
    while( !quit ){
        keyInputManager::update();
        
	stage.logic();
        
        if( keyInputManager::keyState(keys::UP, false) ){
           stage.moveCamera(0,-1);
        }
        if( keyInputManager::keyState(keys::DOWN, false) ){
           stage.moveCamera(0,1);
        }
        if( keyInputManager::keyState(keys::LEFT, false)){
           stage.moveCamera(-1,0);
        }
        if( keyInputManager::keyState(keys::RIGHT, false)){
           stage.moveCamera(1,0);
        }
	if( keyInputManager::keyState(keys::SPACE, true)){
	    stage.reset();
	}
	
        quit |= keyInputManager::keyState(keys::ESC, true );
	
	stage.render(&work);
	work.Stretch(back);
	Font::getDefaultFont().printf( 15, 220, Bitmap::makeColor( 255, 255, 255 ), back, "viewport x: %i  |  viewport y: %i",0, stage.getCameraX(), stage.getCameraY() );
	back.BlitToScreen();
        Util::rest(1);
    }

}

int main( int argc, char ** argv ){
	
	if(argc <= 1){
	    showOptions();
	    return 0;
	}

	const char * FILE_ARG = "-f";
	const char * CHAR_ARG = "-c";
        const char * DEBUG_ARG = "-l";
	const char * STAGE_ARG = "-s";
	std::string ourFile;
	int configLoaded = -1;

        allegro_init();
        install_timer();
	install_keyboard();
	install_mouse();

	for ( int q = 1; q < argc; q++ ){
		if ( isArg( argv[ q ], FILE_ARG ) ){
			q += 1;
			if ( q < argc ){
				ourFile = std::string( argv[ q ] );
				configLoaded = 0;
			}
			else{
                            Global::debug(0) << "Error no file given!" << endl;
			  showOptions();
			  return 0;
			}
		} else if ( isArg( argv[ q ], CHAR_ARG ) ){
			q += 1;
			if ( q < argc ){
				ourFile = std::string( argv[ q ] );
				configLoaded = 1;
			}
			else{
                            Global::debug(0) << "Error no file given!" << endl;
			  showOptions();
			  return 0;
			}
		} else if ( isArg( argv[ q ], STAGE_ARG ) ){
			q += 1;
			if ( q < argc ){
				ourFile = std::string( argv[ q ] );
				configLoaded = 2;
			}
			else{
                            Global::debug(0) << "Error no file given!" << endl;
			  showOptions();
			  return 0;
			}
		} else if (isArg(argv[q], DEBUG_ARG)){
                    q += 1;
                    if (q < argc){
                        istringstream i( argv[ q ] );
                        int f;
                        i >> f;
                        Global::setDebug( f );
                    } else {
                        Global::debug(0) << "No number given for " << DEBUG_ARG << endl;
                    }
                } else {
		    // WHAT?
		    showOptions();
		    return 0;
		}
	}
	
	if( configLoaded == 0 ){
	    MugenReader reader( ourFile );
	    
	    std::vector< MugenSection * > collection;
	    
	    try{
		collection = reader.getCollection();
                Global::debug(0) << endl << "---------------------------------------------------------" << endl;
		for( unsigned int i = 0; i < collection.size(); ++i ){
                    Global::debug(0) << collection[i]->getHeader() << endl;
                    Global::debug(0) << "---------------------------------------------------------" << endl;
		    while( collection[i]->hasItems() ){
			MugenItemContent *content = collection[i]->getNext();
                        while( content->hasItems() ){
                            Global::debug(0) << content->getNext()->query();
                            if( content->hasItems() ) Global::debug(0) << ",";
                        }
                        Global::debug(0) << endl;
		    }
                    Global::debug(0) << "---------------------------------------------------------" << endl;
		}
                Global::debug(0) << endl;
	    }
	    catch( MugenException &ex){
                Global::debug(0) << "Problem loading file, error was: " << ex.getReason() << endl;
		return 1;
	    }
	}
	else if (configLoaded == 1){
	    try{
                showCharacter(ourFile);
            } catch( MugenException &ex){
                Global::debug(0) << "Problem loading file, error was: " << ex.getReason() << endl;
		return 1;
	    } catch(...){
		Global::debug(0) << "Unknown problem loading file" << endl;
		return 1;
	    }
	}
	else if ( configLoaded == 2 ){
	    try{
                showStage(ourFile);
            } catch( MugenException &ex){
                Global::debug(0) << "Problem loading file, error was: " << ex.getReason() << endl;
		return 1;
	    } catch(...){
		Global::debug(0) << "Unknown problem loading file" << endl;
		return 1;
	    }
	}
	else showOptions();
	
	return 0;
}
