#include "util/bitmap.h"
#include "menu/option_credits.h"
#include "menu/menu_global.h"
#include "menu/menu.h"
#include "globals.h"
#include "init.h"
#include "util/funcs.h"
#include "util/fire.h"

#include "input/keyboard.h"
#include "util/token.h"
#include "util/token_exception.h"
#include "util/file-system.h"
#include "util/font.h"
#include "input/input-map.h"
#include "input/input-manager.h"

using namespace std;

OptionCredits::OptionCredits( Token * token ) throw( LoadException ):
MenuOption(token, Event),
background(0),
music(""),
color(Bitmap::makeColor(255,255,255)),
title(Bitmap::makeColor(0,255,255)){
	/* Always */
	credits.push_back("Paintown");
	credits.push_back("");
	credits.push_back("Programming");
	credits.push_back("Jon Rafkind");
	credits.push_back("");
	credits.push_back("Contributions");
	credits.push_back("Miguel Gavidia");
	credits.push_back("");
	credits.push_back("Level design");
	credits.push_back("Jon Rafkind");
	credits.push_back("Miguel Gavidia");
	credits.push_back("");
	credits.push_back("Music");
	credits.push_back("aqua.s3m - Purple Motion");
	credits.push_back("c_heaven.xm - One Man Project");
	credits.push_back("elw-sick.xm - elwood");
	credits.push_back("experience.xm - elwood");
	credits.push_back("fall.xm - elwood");
	credits.push_back("kajahtaa.xm - cube");
	credits.push_back("kilimanz.mod - ???");
	credits.push_back("SM_TechTown.it - SaMPLeMaSTeR");
	credits.push_back("");
	credits.push_back("Email: jon@rafkind.com");
	credits.push_back("");
	
	if ( *token != "credits" )
		throw LoadException("Not a credit menu");

        readName(token);
	
	while ( token->hasTokens() ){
		try{
			Token * tok;
			*token >> tok;
			if ( *tok == "music" ) {
				/* Set music for credits */
				*tok >> music;
			} else if ( *tok == "background" ) {
				/* Create an image and push it back on to vector */
				std::string temp;
				*tok >> temp;
				if ( background ){
					delete background;
				}
				background = new Bitmap(Filesystem::find(temp));
				if ( background->getError() ){
					delete background;
					background = NULL;
				}
			} else if ( *tok == "additional" ) {
				std::string str;
				while ( tok->hasTokens() ){
					*tok >> str;
					credits.push_back(str);
				}
			} else if ( *tok == "titlecolor" ) {
				int r,b,g;
				*tok >> r >> g >> b;
				title = Bitmap::makeColor( r, b, g );
			} else if ( *tok == "color" ) {
				int r,b,g;
				*tok >> r >> g >> b;
				color = Bitmap::makeColor( r, b, g );
			} else {
				Global::debug( 3 ) <<"Unhandled menu attribute: "<<endl;
                                if (Global::getDebug() >= 3){
                                    tok->print(" ");
                                }
			}
		} catch ( const TokenException & ex ) {
			string m( "Menu parse error: " );
			m += ex.getReason();
			throw LoadException( m );
		} catch ( const LoadException & ex ) {
			throw ex;
		}
	}
	
    input.set(Keyboard::Key_ESC, 0, true, Exit);
    input.set(Joystick::Button2, 0, true, Exit);
}

OptionCredits::~OptionCredits(){
	if ( background ){
		delete background;
	}
}

void OptionCredits::logic(){
}

void OptionCredits::run( bool &endGame ){
    Keyboard key;

    const int maxCredits = credits.size();

    Global::speed_counter = 0;
    double min_y = GFX_Y;

    /* use Bitmap::temporaryBitmap here? */
    Bitmap tmp(GFX_X, GFX_Y);
    // Bitmap fireWork(GFX_X, GFX_Y);
    if (! music.empty()){
        MenuGlobals::setMusic(music);
    }

    Paintown::Fire fire;

    const Font & vFont = Font::getFont(Menu::getFont(), Menu::getFontWidth(), Menu::getFontHeight());

    bool quit = false;

    Bitmap::transBlender(0, 0, 0, 128);

    double think = 0;
    while (!quit){

        InputManager::poll();
        InputMap<CreditKey>::Output out = InputManager::getMap(input);
        quit = out[Exit];

        bool draw = false;
        if (Global::speed_counter > 0){
            think += Global::speed_counter * Global::LOGIC_MULTIPLIER;
            draw = true;

            while (think >= 1.0){
                think -= 1;
                min_y -= 0.5;
                if (min_y < -(int)(maxCredits * vFont.getHeight() * 1.1)){
                    min_y = GFX_Y;
                }
                fire.update();
            }

            Global::speed_counter = 0;
        }

        if (draw){
            if (background ){
                background->Blit(tmp);
            } else {
                tmp.fill(Bitmap::makeColor(0,0,0));
            }

            fire.draw(tmp);
            /*
            fire.draw(fireWork);
            fireWork.drawTrans(0, 0, tmp);
            */

            int y = (int) min_y;
            vector<std::string>::iterator b = credits.begin();
            vector<std::string>::iterator e = credits.end();
            bool isTitle = true;
            for ( /**/ ; b != e; b++ ){
                if ( isTitle ){
                    vFont.printf( 100, y, title, tmp, (*b), 0 );
                    isTitle = false;
                } else {
                    vFont.printf( 100, y, color, tmp, (*b), 0 );
                }
                y += vFont.getHeight() + 2;

                if ( (*b).empty() ){
                    isTitle = true;
                }
            }

            tmp.BlitToScreen();
        } else {
            Util::rest( 1 );
        }
    }

    InputManager::waitForRelease(input, Exit);
}
