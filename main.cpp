#include <iostream>
#include <string>
#include <cassert>
#include "IniFile.h"
/** config.ini - input
[general]
windowWidth=1080
windowHeight=720
fullscreen=Yes

[player]
x=100
y=200
name=John Doe
**/

int main() {
    IniFile cfg("config.ini");

    assert(cfg.read<bool>("general", "fullscreen") == true);

    assert(cfg.read<std::string>("player", "name") == "John Doe");
    assert(cfg.read<int>("player", "x") == 100);
    assert(cfg.read<int>("player", "y") == 200);

    assert(cfg.read<int>("player", "z") == 0);
    assert(cfg.read<int>("player", "z", 500) == 500);

    assert(cfg.sectionExists("general") == true);
    assert(cfg.sectionExists("player") == true);
    assert(cfg.sectionExists("AAAAAA") != true);

    assert(cfg.keyExists("general", "windowWidth") == true);
    assert(cfg.keyExists("general", "windowHeight") == true);
    assert(cfg.keyExists("general", "windowDepth") == false);
    assert(cfg.keyExists("asdasdadasd", "windowWidth") == false);

    //cfg.sections() == {"general", "player"};
    //cfg.keys("general") == {"windowWidth", "windowHeight", "fullscreen"};

    cfg.write<std::string>("player", "name", "Ivan Petrov Vasilevishcyhhsh");
    cfg.write<bool>("player", "bool", true);
    cfg.write<int>("newSection", "value", 137);
    cfg.save("config1.ini");
}

/** config.ini - output
[general]
windowWidth=1080
windowHeight=720
fullscreen=Yes

[player]
x=100
y=200
name=Ivan Petrov Vasilevishcyhhsh
bool=true

[newSection]
value=137
**/