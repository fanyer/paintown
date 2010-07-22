#include "mod.h"
#include "level/utils.h"
#include "util/bitmap.h"
#include "util/funcs.h"
#include "util.h"
#include "debug.h"
#include "pack-reader.h"
#include "util/tokenreader.h"
#include "util/token.h"
#include "util/load_exception.h"
#include "exceptions/exception.h"
#include <vector>
#include "init.h"
#include "input/input-manager.h"
#include "input/input-map.h"

using namespace std;

/* FIXME: put this in a header somewhere */
namespace Bor{

class ParseException: std::exception {
public:
    std::string getReason() const;
    virtual ~ParseException() throw();
};

}

namespace Paintown{

OpenborMod::OpenborMod(const Filesystem::AbsolutePath & path):
reader(path){
}

const string OpenborMod::getMenu(){
    return "menu/main.txt";
}

/* player1 = 83
 * player2 = 238
 */
    
std::vector<Level::LevelInfo> OpenborMod::getLevels(){
    vector<Level::LevelInfo> levels;
    Level::LevelInfo level;
    levels.push_back(level);
    return levels;
}
    
Bitmap * OpenborMod::createBitmap(const Filesystem::RelativePath & path){
    try{
        char * data = reader.readFile(reader.getFile(path.path()));
        Bitmap * bitmap = new Bitmap(data, reader.getFileLength(path.path()));
        delete[] data;
        return bitmap;
    } catch (const Bor::PackError & error){
        throw LoadException(__FILE__, __LINE__, error, "Could not create bitmap");
    }
}

static bool isOpenborPlayer(Bor::PackReader & reader, const string & path){
    char * data = NULL;
    try{
        TokenReader tokens;
        char * data = reader.readFile(reader.getFile(path));
        string parsed = Bor::doParse(data, reader.getFileLength(path));
        // Global::debug(0) << "Bor input: '" << parsed << "'" << endl;
        delete[] data;
        data = NULL;

        /* will either succeed or throw TokenException */
        Token * start = tokens.readTokenFromString(parsed);

        Token * type = start->findToken("_/type");
        if (type != NULL){
            string kind;
            *type >> kind;
            return kind == "player";
        } else {
            return false;
        }
    } catch (const TokenException & t){
        Global::debug(0) << "Failed to parse pak file " << path << " " << t.getTrace() << endl;
    } catch (const Bor::PackError & p){
        Global::debug(0) << "Failed to parse pak file " << path << " " << p.getTrace() << endl;
    } catch (const Bor::ParseException & e){
        Global::debug(0) << "Failed to parse pak file " << path << " " << e.getReason() << endl;
    }

    delete[] data;
    return false;
}

#if 0
static PlayerVector findOpenborPlayers(Bor::PackReader & reader){
    PlayerVector players;
    vector<string> paths = reader.findPaths("data/chars/*/*.txt");
    sort(paths.begin(), paths.end());
    for (vector<string>::iterator it = paths.begin(); it != paths.end(); it++){
        string file = *it;
        if (isOpenborPlayer(reader, file)){
            players.push_back(playerInfo(new Bor::DisplayCharacter(reader, file), Filesystem::AbsolutePath(file)));
        }
    }

    return players;
}
#endif

namespace Select{
    enum Input{
        Up, Down, Left,
        Right, Remap, Quit,
        Choose,
    };
}

Filesystem::AbsolutePath OpenborMod::selectPlayer(const string & message, const Level::LevelInfo & info, int & remap){
    Bitmap background(makeBitmap(Filesystem::RelativePath("data/bgs/select.gif")));
    Bitmap work(GFX_X / 2, GFX_Y / 2);
    background.Blit(work);
    work.BlitToScreen();
    InputMap<Select::Input> input;
    input.set(Keyboard::Key_ESC, 0, true, Select::Quit);
    InputManager::waitForPress(input, Select::Quit);

    throw Exception::Base(__FILE__, __LINE__);

    /* show data/bgs/select.gif
     * get all players
     * display selected player at 83, 155
     */
}

}