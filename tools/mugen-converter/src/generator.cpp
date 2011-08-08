#include "generator.h"

#include "ast/all.h"
#include "util/regex.h"

#include <ctime>
#include <list>

using namespace Mugen;

std::string getCurrentDate(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    strftime (buffer,80,"%A - %m/%d/%Y - %I:%M%p.",timeinfo);
    
    return std::string(buffer);
}

static int toLower( int c ){ return tolower( c );}

const std::string lowercase( const std::string &str ){
    std::string tempStr = str;
    transform( tempStr.begin(), tempStr.end(), tempStr.begin(), toLower );
    return tempStr;
}

/* Example on how to use the PythonStream */
void testStream(const std::string & file){
    Mugen::PythonStream out;
    out.open(file);
    out << "import mugen" << PythonStream::endl;
    out <<  "class TestCharacter(mugen.Character):" << PythonStream::endl;
        out << PythonStream::indent;
        out << "def __init__(self):" << PythonStream::endl;
            out << PythonStream::indent;
            out << "self.setName('TestCharacter')" << PythonStream::endl;
    out.clearIndents();
    out << "char = TestCharacter()" << PythonStream::endl;
    out << "print char.name" << PythonStream::endl;
    out.close();
}

/* Simplify use */
PythonStream::IndentType indent = PythonStream::indent;
PythonStream::IndentType unindent = PythonStream::unindent;
StreamEnd endl = PythonStream::endl;

// Content
Content::Content():
totalLines(0){
}
Content::Content(unsigned int level, const std::string & str):
totalLines(1){
    indentLevel.push_back(level);
    content.push_back(str);
}

Content::Content(const Content & copy):
totalLines(0){
    totalLines = copy.totalLines;
    indentLevel = copy.indentLevel;
    content = copy.content;
}
Content::~Content(){
}

const Content & Content::operator=(const Content & copy){
    totalLines = copy.totalLines;
    indentLevel = copy.indentLevel;
    content = copy.content;
    
    return *this;
}

void Content::addLine(unsigned int level, const std::string & str){
    indentLevel.push_back(level);
    content.push_back(str);
    totalLines++;
}

void Content::output(PythonStream & stream, unsigned int indentStart){
    for (unsigned int i = 0; i < totalLines; ++i){
        stream.setIndentLevel(indentStart + indentLevel[i]);
        stream << content[i] << endl;
    }
}

const unsigned int Content::getIndentLevel() const{
    if (totalLines!=0){
        return indentLevel[0];
    }
    return 0;
}


// Python definition
PythonDefinition::PythonDefinition(const Content & content):
defLine(content){
}
PythonDefinition::PythonDefinition(const PythonDefinition & copy){
    defLine = copy.defLine;
    content = copy.content;
}
PythonDefinition::~PythonDefinition(){
}
const PythonDefinition & PythonDefinition::operator=(const PythonDefinition & copy){
    defLine = copy.defLine;
    content = copy.content;
    return *this;
}
void PythonDefinition::addContent(const Content & cont){
    content.push_back(cont);
}
void PythonDefinition::addSpace(){
    content.push_back(Content(0,""));
}
void PythonDefinition::output(PythonStream & stream, unsigned int indentStart){
    defLine.output(stream, indentStart);
    stream.setIndentLevel(indentStart + defLine.getIndentLevel());
    for (std::vector<Content>::iterator i = content.begin(); i != content.end(); ++i){
        Content & cont = *i;
        cont.output(stream, indentStart + defLine.getIndentLevel());
    }
}

// Python class
PythonClass::PythonClass(const Content & content):
classLine(content),
init(Content(1,"def __init__(self):")){
}
PythonClass::PythonClass(const PythonClass & copy):
init(Content(1,"def __init__(self):")){
    classLine = copy.classLine;
    init = copy.init;
    definitions = definitions;
}
PythonClass::~PythonClass(){
}
const PythonClass & PythonClass::operator=(const PythonClass & copy){
    classLine = copy.classLine;
    init = copy.init;
    definitions = definitions;
    return *this;
}
void PythonClass::add(const PythonDefinition & def){
    definitions.push_back(def);
}
void PythonClass::output(PythonStream & stream, unsigned int indentStart){
    classLine.output(stream, indentStart);
    stream.setIndentLevel(indentStart + classLine.getIndentLevel());
    init.output(stream, indentStart + classLine.getIndentLevel());
    for (std::vector<PythonDefinition>::iterator i = definitions.begin(); i != definitions.end(); ++i){
        PythonDefinition & def = *i;
        def.output(stream, indentStart + classLine.getIndentLevel());
    }
}


CharacterGenerator::CharacterGenerator(const std::string & filename):
filename(filename),
directory(Mugen::stripFilename(filename)){
}

CharacterGenerator::~CharacterGenerator(){
}

void CharacterGenerator::output(const std::string & file){
    try {
        std::cout << "Preparing character definition file " << filename << " for output to file: " << file << std::endl;
        
        Mugen::PythonStream out;
        out.open(file);;
        
        out << "# File generated from original file '" << Mugen::stripDir(filename) << "' on " << getCurrentDate() << endl;
        out << endl;
        
        out << "import mugen" << endl;
        out << endl;
        // Create character class
        PythonClass character(Content(0, "class " + Mugen::stripExtension(file) + "(mugen.Character):"));
        handleBaseDef(character);
        handleCmdFile(character);
        character.output(out, 0);
        out.close();
    } catch (const Mugen::Cmd::ParseException & fail){
        std::cout << "Failed to parse " << file << " because " << fail.getReason() << std::endl;
    } catch (...){
        std::cout << "Unknown Exception Caught!\nAborting..." << std::endl;
    }
}

void CharacterGenerator::handleBaseDef(PythonClass & character){
    class BaseWalker: public Ast::Walker {
        public:
            BaseWalker(CharacterGenerator & generator, PythonClass & character):
            generator(generator),
            character(character){
                
            }
            
            CharacterGenerator & generator;
            PythonClass & character;
            std::string sectionName;
            
            virtual void onSection(const Ast::Section & section){
                sectionName = section.getName();
                character.getInit().addSpace();
                character.getInit().addContent(Content(1,"# [" + Mugen::stripDir(generator.filename) + "] Section - " + sectionName));
                std::cout << "Parsing Section: " << sectionName << std::endl;
            }
            virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                if (simple == "name"){
                    try{
                        character.getInit().addContent(Content(1,"self.name = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "displayname"){
                    try{
                        character.getInit().addContent(Content(1,"self.displayName = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "versiondate"){
                    try{
                        std::vector<int> numbers;
                        simple >> numbers;
                        std::stringstream out;
                        out << "self.versionDate = [";
                        unsigned int count = 0;
                        for (std::vector<int>::iterator it = numbers.begin(); it != numbers.end(); it++){
                            out << *it << (count != numbers.size()-1 ? "," : "");
                            count++;
                        }
                        out << "]";
                        character.getInit().addContent(Content(1,out.str()));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "mugenversion"){
                    try{
                        std::vector<int> numbers;
                        simple >> numbers;
                        std::stringstream out;
                        out << "self.mugenVersion = [";
                        unsigned int count = 0;
                        for (std::vector<int>::iterator it = numbers.begin(); it != numbers.end(); it++){
                            out << *it << (count != numbers.size()-1 ? "," : "");
                            count++;
                        }
                        out << "]";
                        character.getInit().addContent(Content(1, out.str()));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "author"){
                    try{
                        character.getInit().addContent(Content(1,"self.author = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "pal.defaults"){
                        std::vector<int> numbers;
                        simple >> numbers;
                        std::stringstream out;
                        out << "self.paletteDefaults = [";
                        unsigned int count = 0;
                        for (std::vector<int>::iterator it = numbers.begin(); it != numbers.end(); it++){
                            out << *it << (count != numbers.size()-1 ? "," : "");
                            count++;
                        }
                        out << "]";
                        character.getInit().addContent(Content(1,out.str()));
                } else if (simple == "cmd"){
                    try{
                        simple >> generator.commandFile;
                        character.getInit().addContent(Content(1,"self.cmdFile = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "cns"){
                    try{
                        simple >> generator.constantsFile;
                        character.getInit().addContent(Content(1,"self.cnsFile = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "st"){
                    try{
                        generator.stateFiles.push_back(simple.valueAsString());
                        character.getInit().addContent(Content(1,"self.stateFiles.append('" + simple.valueAsString() + "')"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "stcommon"){
                    try{
                        generator.stateFiles.push_back(simple.valueAsString());
                        character.getInit().addContent(Content(1,"self.stateFiles.append('" + simple.valueAsString() + "')"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (Util::matchRegex(simple.idString(), "st[0-9]+")){
                    try{
                        generator.stateFiles.push_back(simple.valueAsString());
                        character.getInit().addContent(Content(1,"self.stateFiles.append('" + simple.valueAsString() + "')"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "sprite"){
                    try{
                        character.getInit().addContent(Content(1,"self.spriteFile = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "anim"){
                    try{
                        simple >> generator.airFile;
                        character.getInit().addContent(Content(1,"self.animFile = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "sound"){
                    try{
                        character.getInit().addContent(Content(1,"self.soundFile = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (Util::matchRegex(simple.idString(), "pal[0-9]+")){
                    std::string id = simple.idString();
                    try{
                        character.getInit().addContent(Content(1,"self.palleteFiles[" + id.substr(3) + "] = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "intro.storyboard"){
                    try{
                        character.getInit().addContent(Content(1,"self.introStoryboard = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "ending.storyboard"){
                    try{
                        character.getInit().addContent(Content(1,"self.endingStoryboard = '" + simple.valueAsString() + "'"));
                    } catch (const Ast::Exception & fail){
                    }
                } else {
                    std::cout << "Unhandled option in " << sectionName << " Section: " << simple.toString() << std::endl;
                }
            }
            
            virtual void onAttributeKeyword (const Ast::AttributeKeyword &simple){
                std::cout << "Attribute keyword: " << simple.toString() << std::endl;
            }
            
            virtual void onAttributeArray (const Ast::AttributeArray &simple){
                std::cout << "Attribute array: " << simple.toString() << std::endl;
            }

            virtual void onNumber (const Ast::Number &simple){
                std::cout << "number: " << simple.toString() << std::endl;
            }
    };
    
    BaseWalker walker(*this, character);
    std::list<Ast::Section*> * sections = parseDef(filename);
    for (std::list<Ast::Section*>::iterator it = sections->begin(); it != sections->end(); it++){
        Ast::Section * section = *it;
        section->walk(walker);
        std::list<Ast::Attribute*>  attributes = section->getAttributes();
    }
    destroy(sections);
}
#if 0
void CharacterGenerator::handleConstants(Mugen::PythonStream & stream){
    
    // For re-use
    std::list<Ast::Section*> * sections;
    
    class CnsWalker: public Ast::Walker {
        public:
            CnsWalker(CharacterGenerator & character, PythonStream & stream ):
            character(character),
            stream(stream){
                
            }
            
            CharacterGenerator & character;
            PythonStream & stream;
            std::string sectionName;
            
            virtual void onSection(const Ast::Section & section){
                sectionName = section.getName();
                stream << endl;
                stream << "# [" << Mugen::stripDir(character.filename) << "] Section - " << sectionName << endl;
                std::cout << "Parsing Section: " << sectionName << std::endl;
            }
            virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                if (simple == "command.time"){
                    try{
                        stream << "self.commandTime = '" << simple.valueAsString() << "'" << endl;
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "command.buffer.time"){
                    try{
                        stream << "self.commandBufferTime = '" << simple.valueAsString() << "'" << endl;
                    } catch (const Ast::Exception & fail){
                    }
                } else {
                    std::cout << "Unhandled option in " << sectionName << " Section: " << simple.toString() << std::endl;
                }
            }
            
            virtual void onAttributeKeyword (const Ast::AttributeKeyword &simple){
                std::cout << "Attribute keyword: " << simple.toString() << std::endl;
            }
            
            virtual void onAttributeArray (const Ast::AttributeArray &simple){
                std::cout << "Attribute array: " << simple.toString() << std::endl;
            }

            virtual void onNumber (const Ast::Number &simple){
                std::cout << "number: " << simple.toString() << std::endl;
            }
    };
    
    // Command Walker
    CnsWalker cmd(*this, stream);
    sections = parseCmd(directory + commandFile);
    for (std::list<Ast::Section*>::iterator it = sections->begin(); it != sections->end(); it++){
        Ast::Section * section = *it;
        if (section->getName() == "Defaults"){
            section->walk(cmd);
            std::list<Ast::Attribute*>  attributes = section->getAttributes();
            break;
        }
    }
    destroy(sections);
    
    CnsWalker cns(*this, stream);
    sections = parseCmd(directory + constantsFile);
    bool found[4] = { false, false, false, false };
    for (std::list<Ast::Section*>::iterator it = sections->begin(); it != sections->end(); it++){
        Ast::Section * section = *it;
        if (section->getName() == "Data"){
            section->walk(cns);
            std::list<Ast::Attribute*>  attributes = section->getAttributes();
            found[0] = true;
        } else if (section->getName() == "Size"){
            section->walk(cns);
            std::list<Ast::Attribute*>  attributes = section->getAttributes();
            found[1] = true;
        } else if (section->getName() == "Velocity"){
            section->walk(cns);
            std::list<Ast::Attribute*>  attributes = section->getAttributes();
            found[2] = true;
        } else if (section->getName() == "Movement"){
            section->walk(cns);
            std::list<Ast::Attribute*>  attributes = section->getAttributes();
            found[3] = true;
        } else if (found[0] == true && found[1] == true && found[2] == true && found[3] == true) {
            break;
        }
    }
    destroy(sections);
}
#endif

void CharacterGenerator::handleCmdFile(PythonClass & character){
    
    class Command{
        public:
            Command(){
            }
            std::string name;
            std::string command;
            std::string time;
            std::string bufferTime;
            
            std::string get(){
                return "self.addCommand('" + name + "', '" + command + "', '" + time + "', '" + bufferTime + "')";
            }
    };
        
    class CmdWalker: public Ast::Walker {
        public:
            CmdWalker(CharacterGenerator & generator, PythonClass & character ):
            generator(generator),
            character(character),
            currentCommand(NULL){
                
            }
            
            ~CmdWalker(){
                if (currentCommand != NULL){
                    delete currentCommand;
                }
            }
            
            CharacterGenerator & generator;
            PythonClass & character;
            std::string sectionName;
            Command * currentCommand;
            
            virtual void onSection(const Ast::Section & section){
                sectionName = lowercase(section.getName());
                character.getInit().addSpace();
                character.getInit().addContent(Content(1,"# [" + Mugen::stripDir(generator.filename) + "] Section - " + section.getName()));
                if (sectionName == "command"){
                    // Create new command class to put away and store the previous one
                    std::cout << "Found command" << std::endl;
                    if (currentCommand == NULL){
                        currentCommand = new Command();
                    } else if (currentCommand != NULL){
                        // Add command
                        character.getInit().addContent(Content(1,currentCommand->get()));
                        delete currentCommand;
                        currentCommand = new Command();
                    }
                } else if (Util::matchRegex(sectionName, "state")){
                    // Create state handler
                }
                std::cout << "Parsing Section: " << section.getName() << std::endl;   
            }
            virtual void onAttributeSimple(const Ast::AttributeSimple & simple){
                if (simple == "command.time"){
                    try{
                        character.getInit().addContent(Content(1,"self.commandTime('" + simple.valueAsString() + "')"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "command.buffer.time"){
                    try{
                        character.getInit().addContent(Content(1,"self.commandBufferTime('" + simple.valueAsString() + "')"));
                    } catch (const Ast::Exception & fail){
                    }
                } else if (simple == "name"){
                    if (currentCommand != NULL){
                        currentCommand->name = simple.valueAsString();
                    }
                } else if (simple == "command"){
                    if (currentCommand != NULL){
                        std::string command;
                        try {
                            std::string cmd;
                            simple >> cmd;
                            command += cmd + ",";
                        } catch (Ast::Exception &ex){
                        }
                        currentCommand->command = command.substr(0,command.size()-1);
                    }
                } else if (simple == "time"){
                    if (currentCommand != NULL){
                        currentCommand->time = simple.valueAsString();
                    }
                } else if (simple == "buffer.time"){
                    if (currentCommand != NULL){
                        currentCommand->bufferTime = simple.valueAsString();
                    }
                } else if (simple == "type"){
                    // TODO add to state
                } else if (simple == "value"){
                    // TODO add to state
                } else if (simple == "triggerall"){
                    // TODO add to state
                } else if (Util::matchRegex(simple.idString(), "trigger[0-9]+")){
                    // TODO add to state
                } else if (Util::matchRegex(simple.idString(), "var([0-9]+)")){
                    // TODO add to state
                } else {
                    std::cout << "Unhandled option in " << sectionName << " Section: " << simple.toString() << std::endl;
                }
            }
            
            virtual void onAttributeKeyword (const Ast::AttributeKeyword &simple){
                std::cout << "Attribute keyword: " << simple.toString() << std::endl;
            }
            
            virtual void onAttributeArray (const Ast::AttributeArray &simple){
                std::cout << "Attribute array: " << simple.toString() << std::endl;
            }

            virtual void onNumber (const Ast::Number &simple){
                std::cout << "number: " << simple.toString() << std::endl;
            }
            
            virtual void complete (){
                // Add last command and state to character
                if (currentCommand != NULL){
                    character.getInit().addContent(Content(1,currentCommand->get()));
                }
                // TODO add state
            }
    };
    
    // Command Walker
    CmdWalker cmd(*this, character);
    std::list<Ast::Section*> * sections = parseCmd(directory + commandFile);
    for (std::list<Ast::Section*>::iterator it = sections->begin(); it != sections->end(); it++){
        Ast::Section * section = *it;
        section->walk(cmd);
        std::list<Ast::Attribute*>  attributes = section->getAttributes();
    }
    destroy(sections);
    cmd.complete();
}