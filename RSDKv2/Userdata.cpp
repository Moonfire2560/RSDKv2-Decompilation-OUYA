#include "RetroEngine.hpp"

int NO_GLOBALVARIABLES;
int GlobalVariables[GLOBALVAR_COUNT];
char GlobalVariableNames[GLOBALVAR_COUNT][0x20];

char gamePath[0x100];

void InitUserdata()
{
    // userdata files are loaded from this directory
    sprintf(gamePath, "%s", BASE_PATH);
#if RETRO_USE_MOD_LOADER
    sprintf(modsPath, "%s", BASE_PATH);
#endif

    char buffer[0x200];
#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_UWP
    if (!usingCWD)
        sprintf(buffer, "%s/settings.ini", getResourcesPath());
    else
        sprintf(buffer, "%ssettings.ini", gamePath);
#elif RETRO_PLATFORM == RETRO_iOS
    sprintf(buffer, "%s/settings.ini", getDocumentsPath());
#else
    sprintf(buffer, BASE_PATH"/settings.ini");
#endif
    FileIO *file = fOpen(buffer, "rb");
    IniParser ini;
    if (!file) {
        ini.SetBool("Dev", "DevMenu", Engine.devMenu = false);
        ini.SetBool("Dev", "EngineDebugMode", engineDebugMode = false);
        ini.SetInteger("Dev", "StartingCategory", Engine.startList = 0xFF);
        ini.SetInteger("Dev", "StartingScene", Engine.startStage = 0xFF);
        ini.SetInteger("Dev", "FastForwardSpeed", Engine.fastForwardSpeed = 8);
        sprintf(Engine.dataFile, "%s", "Data.bin");
        ini.SetString("Dev", "DataFile", Engine.dataFile);

        Engine.startList_Game  = Engine.startList;
        Engine.startStage_Game = Engine.startStage;

        ini.SetBool("Window", "FullScreen", Engine.startFullScreen = DEFAULT_FULLSCREEN);
        ini.SetBool("Window", "Borderless", Engine.borderless = false);
        ini.SetBool("Window", "VSync", Engine.vsync = false);
        ini.SetBool("Window", "EnhancedScaling", Engine.enhancedScaling = true);
        ini.SetInteger("Window", "WindowScale", Engine.windowScale = 2);
        ini.SetInteger("Window", "ScreenWidth", SCREEN_XSIZE = DEFAULT_SCREEN_XSIZE);
        ini.SetInteger("Window", "RefreshRate", Engine.refreshRate = 60);
        ini.SetInteger("Window", "ColourMode", Engine.ColourMode = 1);

        ini.SetFloat("Audio", "BGMVolume", bgmVolume / (float)MAX_VOLUME);
        ini.SetFloat("Audio", "SFXVolume", sfxVolume / (float)MAX_VOLUME);

#if RETRO_USING_SDL2
        ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
        ini.SetInteger("Keyboard 1", "Up", inputDevice[INPUT_UP].keyMappings = SDL_SCANCODE_UP);
        ini.SetInteger("Keyboard 1", "Down", inputDevice[INPUT_DOWN].keyMappings = SDL_SCANCODE_DOWN);
        ini.SetInteger("Keyboard 1", "Left", inputDevice[INPUT_LEFT].keyMappings = SDL_SCANCODE_LEFT);
        ini.SetInteger("Keyboard 1", "Right", inputDevice[INPUT_RIGHT].keyMappings = SDL_SCANCODE_RIGHT);
        ini.SetInteger("Keyboard 1", "A", inputDevice[INPUT_BUTTONA].keyMappings = SDL_SCANCODE_A);
        ini.SetInteger("Keyboard 1", "B", inputDevice[INPUT_BUTTONB].keyMappings = SDL_SCANCODE_S);
        ini.SetInteger("Keyboard 1", "C", inputDevice[INPUT_BUTTONC].keyMappings = SDL_SCANCODE_D);
        ini.SetInteger("Keyboard 1", "Start", inputDevice[INPUT_START].keyMappings = SDL_SCANCODE_RETURN);

        ini.SetComment("Controller 1", "IC1Comment", "Controller Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_GameControllerButton)");
        ini.SetInteger("Controller 1", "Up", inputDevice[INPUT_UP].contMappings = SDL_CONTROLLER_BUTTON_DPAD_UP);
        ini.SetInteger("Controller 1", "Down", inputDevice[INPUT_DOWN].contMappings = SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        ini.SetInteger("Controller 1", "Left", inputDevice[INPUT_LEFT].contMappings = SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        ini.SetInteger("Controller 1", "Right", inputDevice[INPUT_RIGHT].contMappings = SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        ini.SetInteger("Controller 1", "A", inputDevice[INPUT_BUTTONA].contMappings = SDL_CONTROLLER_BUTTON_A);
        ini.SetInteger("Controller 1", "B", inputDevice[INPUT_BUTTONB].contMappings = SDL_CONTROLLER_BUTTON_B);
        ini.SetInteger("Controller 1", "C", inputDevice[INPUT_BUTTONC].contMappings = SDL_CONTROLLER_BUTTON_X);
#if RETRO_PLATFORM == RETRO_ANDROID
        // OUYA: the OUYA controller has no Start button (and the system button never reaches the game),
        // so clicking the right stick in (R3) is used as Start instead.
        // SDL reports it as SDL_CONTROLLER_BUTTON_RIGHTSTICK, which is raw joystick button 8 on the OUYA
        ini.SetInteger("Controller 1", "Start", inputDevice[INPUT_START].contMappings = SDL_CONTROLLER_BUTTON_RIGHTSTICK);
#else
        ini.SetInteger("Controller 1", "Start", inputDevice[INPUT_START].contMappings = SDL_CONTROLLER_BUTTON_START);
#endif
#endif

#if RETRO_USING_SDL1
        ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
        ini.SetInteger("Keyboard 1", "Up", inputDevice[INPUT_UP].keyMappings = SDLK_UP);
        ini.SetInteger("Keyboard 1", "Down", inputDevice[INPUT_DOWN].keyMappings = SDLK_DOWN);
        ini.SetInteger("Keyboard 1", "Left", inputDevice[INPUT_LEFT].keyMappings = SDLK_LEFT);
        ini.SetInteger("Keyboard 1", "Right", inputDevice[INPUT_RIGHT].keyMappings = SDLK_RIGHT);
        ini.SetInteger("Keyboard 1", "A", inputDevice[INPUT_BUTTONA].keyMappings = SDLK_a);
        ini.SetInteger("Keyboard 1", "B", inputDevice[INPUT_BUTTONB].keyMappings = SDLK_s);
        ini.SetInteger("Keyboard 1", "C", inputDevice[INPUT_BUTTONC].keyMappings = SDLK_d);
        ini.SetInteger("Keyboard 1", "Start", inputDevice[INPUT_START].keyMappings = SDLK_RETURN);

        ini.SetComment("Controller 1", "IC1Comment", "Controller Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_GameControllerButton)");
        ini.SetInteger("Controller 1", "Up", inputDevice[INPUT_UP].contMappings = 1);
        ini.SetInteger("Controller 1", "Down", inputDevice[INPUT_DOWN].contMappings = 2);
        ini.SetInteger("Controller 1", "Left", inputDevice[INPUT_LEFT].contMappings = 3);
        ini.SetInteger("Controller 1", "Right", inputDevice[INPUT_RIGHT].contMappings = 4);
        ini.SetInteger("Controller 1", "A", inputDevice[INPUT_BUTTONA].contMappings = 5);
        ini.SetInteger("Controller 1", "B", inputDevice[INPUT_BUTTONB].contMappings = 6);
        ini.SetInteger("Controller 1", "C", inputDevice[INPUT_BUTTONC].contMappings = 7);
        ini.SetInteger("Controller 1", "Start", inputDevice[INPUT_START].contMappings = 8);
#endif

        ini.Write(BASE_PATH"/settings.ini");
    }
    else {
        fClose(file);
        ini = IniParser(BASE_PATH"/settings.ini");

        if (!ini.GetBool("Dev", "DevMenu", &Engine.devMenu))
            Engine.devMenu = false;
        if (!ini.GetBool("Dev", "EngineDebugMode", &engineDebugMode))
            engineDebugMode = false;
        if (!ini.GetInteger("Dev", "StartingCategory", &Engine.startList))
            Engine.startList = 0xFF;
        if (!ini.GetInteger("Dev", "StartingScene", &Engine.startStage))
            Engine.startStage = 0xFF;
        if (!ini.GetInteger("Dev", "FastForwardSpeed", &Engine.fastForwardSpeed))
            Engine.fastForwardSpeed = 8;

        Engine.startList_Game  = Engine.startList;
        Engine.startStage_Game = Engine.startStage;

        if (!ini.GetString("Dev", "DataFile", Engine.dataFile))
            StrCopy(Engine.dataFile, "Data.bin");

        if (!ini.GetBool("Window", "FullScreen", &Engine.startFullScreen))
            Engine.startFullScreen = DEFAULT_FULLSCREEN;
        if (!ini.GetBool("Window", "Borderless", &Engine.borderless))
            Engine.borderless = false;
        if (!ini.GetBool("Window", "VSync", &Engine.vsync))
            Engine.vsync = false;
        if (!ini.GetBool("Window", "EnhancedScaling", &Engine.enhancedScaling))
            Engine.enhancedScaling = true;
        if (!ini.GetInteger("Window", "WindowScale", &Engine.windowScale))
            Engine.windowScale = 2;
        if (!ini.GetInteger("Window", "ScreenWidth", &SCREEN_XSIZE))
            SCREEN_XSIZE = DEFAULT_SCREEN_XSIZE;
        if (!ini.GetInteger("Window", "RefreshRate", &Engine.refreshRate))
            Engine.refreshRate = 60;
        int cm = Engine.ColourMode;
        if (!ini.GetInteger("Window", "ColourMode", &cm))
            cm = 1;
        Engine.ColourMode = cm;

        float bv = 0, sv = 0;
        if (!ini.GetFloat("Audio", "BGMVolume", &bv))
            bv = 1.0f;
        if (!ini.GetFloat("Audio", "SFXVolume", &sv))
            sv = 1.0f;

        bgmVolume = bv * MAX_VOLUME;
        sfxVolume = sv * MAX_VOLUME;

        if (bgmVolume > MAX_VOLUME)
            bgmVolume = MAX_VOLUME;
        if (bgmVolume < 0)
            bgmVolume = 0;

        if (sfxVolume > MAX_VOLUME)
            sfxVolume = MAX_VOLUME;
        if (sfxVolume < 0)
            sfxVolume = 0;

#if RETRO_USING_SDL2
        if (!ini.GetInteger("Keyboard 1", "Up", &inputDevice[INPUT_UP].keyMappings))
            inputDevice[0].keyMappings = SDL_SCANCODE_UP;
        if (!ini.GetInteger("Keyboard 1", "Down", &inputDevice[INPUT_DOWN].keyMappings))
            inputDevice[1].keyMappings = SDL_SCANCODE_DOWN;
        if (!ini.GetInteger("Keyboard 1", "Left", &inputDevice[INPUT_LEFT].keyMappings))
            inputDevice[2].keyMappings = SDL_SCANCODE_LEFT;
        if (!ini.GetInteger("Keyboard 1", "Right", &inputDevice[INPUT_RIGHT].keyMappings))
            inputDevice[3].keyMappings = SDL_SCANCODE_RIGHT;
        if (!ini.GetInteger("Keyboard 1", "A", &inputDevice[INPUT_BUTTONA].keyMappings))
            inputDevice[4].keyMappings = SDL_SCANCODE_A;
        if (!ini.GetInteger("Keyboard 1", "B", &inputDevice[INPUT_BUTTONB].keyMappings))
            inputDevice[5].keyMappings = SDL_SCANCODE_S;
        if (!ini.GetInteger("Keyboard 1", "C", &inputDevice[INPUT_BUTTONC].keyMappings))
            inputDevice[6].keyMappings = SDL_SCANCODE_D;
        if (!ini.GetInteger("Keyboard 1", "Start", &inputDevice[INPUT_START].keyMappings))
            inputDevice[7].keyMappings = SDL_SCANCODE_RETURN;

        if (!ini.GetInteger("Controller 1", "Up", &inputDevice[INPUT_UP].contMappings))
            inputDevice[0].contMappings = SDL_CONTROLLER_BUTTON_DPAD_UP;
        if (!ini.GetInteger("Controller 1", "Down", &inputDevice[INPUT_DOWN].contMappings))
            inputDevice[1].contMappings = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        if (!ini.GetInteger("Controller 1", "Left", &inputDevice[INPUT_LEFT].contMappings))
            inputDevice[2].contMappings = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        if (!ini.GetInteger("Controller 1", "Right", &inputDevice[INPUT_RIGHT].contMappings))
            inputDevice[3].contMappings = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
        if (!ini.GetInteger("Controller 1", "A", &inputDevice[INPUT_BUTTONA].contMappings))
            inputDevice[4].contMappings = SDL_CONTROLLER_BUTTON_A;
        if (!ini.GetInteger("Controller 1", "B", &inputDevice[INPUT_BUTTONB].contMappings))
            inputDevice[5].contMappings = SDL_CONTROLLER_BUTTON_B;
        if (!ini.GetInteger("Controller 1", "C", &inputDevice[INPUT_BUTTONC].contMappings))
            inputDevice[6].contMappings = SDL_CONTROLLER_BUTTON_X;
        if (!ini.GetInteger("Controller 1", "Start", &inputDevice[INPUT_START].contMappings))
#if RETRO_PLATFORM == RETRO_ANDROID
            // OUYA: same as above, R3 is Start when the settings.ini has no Start entry
            inputDevice[7].contMappings = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
#else
            inputDevice[7].contMappings = SDL_CONTROLLER_BUTTON_START;
#endif
#endif

#if RETRO_USING_SDL1
        if (!ini.GetInteger("Keyboard 1", "Up", &inputDevice[INPUT_UP].keyMappings))
            inputDevice[0].keyMappings = SDLK_UP;
        if (!ini.GetInteger("Keyboard 1", "Down", &inputDevice[INPUT_DOWN].keyMappings))
            inputDevice[1].keyMappings = SDLK_DOWN;
        if (!ini.GetInteger("Keyboard 1", "Left", &inputDevice[INPUT_LEFT].keyMappings))
            inputDevice[2].keyMappings = SDLK_LEFT;
        if (!ini.GetInteger("Keyboard 1", "Right", &inputDevice[INPUT_RIGHT].keyMappings))
            inputDevice[3].keyMappings = SDLK_RIGHT;
        if (!ini.GetInteger("Keyboard 1", "A", &inputDevice[INPUT_BUTTONA].keyMappings))
            inputDevice[4].keyMappings = SDLK_a;
        if (!ini.GetInteger("Keyboard 1", "B", &inputDevice[INPUT_BUTTONB].keyMappings))
            inputDevice[5].keyMappings = SDLK_s;
        if (!ini.GetInteger("Keyboard 1", "C", &inputDevice[INPUT_BUTTONC].keyMappings))
            inputDevice[6].keyMappings = SDLK_d;
        if (!ini.GetInteger("Keyboard 1", "Start", &inputDevice[INPUT_START].keyMappings))
            inputDevice[7].keyMappings = SDLK_RETURN;

        if (!ini.GetInteger("Controller 1", "Up", &inputDevice[INPUT_UP].contMappings))
            inputDevice[0].contMappings = 1;
        if (!ini.GetInteger("Controller 1", "Down", &inputDevice[INPUT_DOWN].contMappings))
            inputDevice[1].contMappings = 2;
        if (!ini.GetInteger("Controller 1", "Left", &inputDevice[INPUT_LEFT].contMappings))
            inputDevice[2].contMappings = 3;
        if (!ini.GetInteger("Controller 1", "Right", &inputDevice[INPUT_RIGHT].contMappings))
            inputDevice[3].contMappings = 4;
        if (!ini.GetInteger("Controller 1", "A", &inputDevice[INPUT_BUTTONA].contMappings))
            inputDevice[4].contMappings = 5;
        if (!ini.GetInteger("Controller 1", "B", &inputDevice[INPUT_BUTTONB].contMappings))
            inputDevice[5].contMappings = 6;
        if (!ini.GetInteger("Controller 1", "C", &inputDevice[INPUT_BUTTONC].contMappings))
            inputDevice[6].contMappings = 7;
        if (!ini.GetInteger("Controller 1", "Start", &inputDevice[INPUT_START].contMappings))
            inputDevice[7].contMappings = 8;
#endif
    }
    SetScreenSize(SCREEN_XSIZE, SCREEN_YSIZE);

    // Support for extra controller types SDL doesn't recognise
#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_UWP
    if (!usingCWD)
        sprintf(buffer, "%s/controllerdb.txt", getResourcesPath());
    else
        sprintf(buffer, "%scontrollerdb.txt", gamePath);
#else
    sprintf(buffer, BASE_PATH "controllerdb.txt");
#endif

#if RETRO_USING_SDL2
    file = fOpen(buffer, "rb");
    if (file) {
        fClose(file);

        int nummaps = SDL_GameControllerAddMappingsFromFile(buffer);
        if (nummaps >= 0)
            PrintLog("loaded %d controller mappings from '%s'\n", buffer, nummaps);
    }
#endif
}

void WriteSettings() {
    IniParser ini;

    ini.SetComment("Dev", "DevMenuComment", "Enable this flag to activate dev menu via the ESC key");
    ini.SetBool("Dev", "DevMenu", Engine.devMenu);
    ini.SetComment("Dev", "DebugModeComment", "Enable this flag to activate features used for debugging the engine (may result in slightly slower game speed)");
    ini.SetBool("Dev", "EngineDebugMode", engineDebugMode);
    ini.SetComment("Dev", "SCComment", "Sets the starting category ID");
    ini.SetInteger("Dev", "StartingCategory", Engine.startList);
    ini.SetComment("Dev", "SSComment", "Sets the starting scene ID");
    ini.SetInteger("Dev", "StartingScene", Engine.startStage);
    ini.SetComment("Dev", "FFComment", "Determines how fast the game will be when fastforwarding is active");
    ini.SetInteger("Dev", "FastForwardSpeed", Engine.fastForwardSpeed);

    ini.SetComment("Dev", "DataFileComment", "Determines what Datafile will be loaded");
    ini.SetString("Dev", "DataFile", Engine.dataFile);

    ini.SetComment("Window", "FSComment", "Determines if the window will be fullscreen or not");
    ini.SetBool("Window", "FullScreen", Engine.startFullScreen);
    ini.SetComment("Window", "BLComment", "Determines if the window will be borderless or not");
    ini.SetBool("Window", "Borderless", Engine.borderless);
    ini.SetComment("Window", "VSComment", "Determines if VSync will be active or not");
    ini.SetBool("Window", "VSync", Engine.vsync);
    ini.SetComment("Window", "ESComment", "Determines if Enhanced Scaling will be active or not. Only affects non-multiple resolutions.");
    ini.SetBool("Window", "EnhancedScaling", Engine.enhancedScaling);
    ini.SetComment("Window", "WSComment", "How big the window will be");
    ini.SetInteger("Window", "WindowScale", Engine.windowScale);
    ini.SetComment("Window", "SWComment", "How wide the base screen will be in pixels");
    ini.SetInteger("Window", "ScreenWidth", SCREEN_XSIZE);
    ini.SetComment("Window", "RRComment", "Determines the target FPS");
    ini.SetInteger("Window", "RefreshRate", Engine.refreshRate);
    ini.SetComment("Window", "CMComment", "Determines the output colour mode (0 = 8-bit, 1 = 16-bit, 2 = 32-bit)");
    ini.SetInteger("Window", "ColourMode", Engine.ColourMode);

    ini.SetFloat("Audio", "BGMVolume", bgmVolume / (float)MAX_VOLUME);
    ini.SetFloat("Audio", "SFXVolume", sfxVolume / (float)MAX_VOLUME);

#if RETRO_USING_SDL2
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
#endif
#if RETRO_USING_SDL1
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDLKeycodeLookup)");
#endif
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
    ini.SetInteger("Keyboard 1", "Up", inputDevice[INPUT_UP].keyMappings);
    ini.SetInteger("Keyboard 1", "Down", inputDevice[INPUT_DOWN].keyMappings);
    ini.SetInteger("Keyboard 1", "Left", inputDevice[INPUT_LEFT].keyMappings);
    ini.SetInteger("Keyboard 1", "Right", inputDevice[INPUT_RIGHT].keyMappings);
    ini.SetInteger("Keyboard 1", "A", inputDevice[INPUT_BUTTONA].keyMappings);
    ini.SetInteger("Keyboard 1", "B", inputDevice[INPUT_BUTTONB].keyMappings);
    ini.SetInteger("Keyboard 1", "C", inputDevice[INPUT_BUTTONC].keyMappings);
    ini.SetInteger("Keyboard 1", "Start", inputDevice[INPUT_START].keyMappings);

#if RETRO_USING_SDL2
    ini.SetComment("Controller 1", "IC1Comment", "Controller Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_GameControllerButton)");
#if RETRO_PLATFORM == RETRO_ANDROID
    // OUYA: Input.cpp reads raw joystick buttons (it bypasses SDL's controller mapping), so IDs 0-21 below are the
    // pad's own button numbers rather than SDL_GameControllerButton values. IDs 22+ are still the engine's extra IDs
    ini.SetComment("Controller 1", "IC1CommentOUYA1", "OUYA controller: IDs 0-21 are raw button numbers, not SDL_GameControllerButton values:");
    ini.SetComment("Controller 1", "IC1CommentOUYA2", "O = 0, A = 1, U = 2, Y = 3, L3 = 7, R3 = 8, L1 = 9, R1 = 10");
    ini.SetComment("Controller 1", "IC1CommentOUYA3", "D-pad Up = 11, Down = 12, Left = 13, Right = 14, L2 = 15, R2 = 16");
    ini.SetComment("Controller 1", "IC1CommentOUYA4", "L3 (7) is also hard-wired as Select (acts as ESC in the dev menu)");
#endif
    ini.SetComment("Controller 1", "IC1Comment2", "Extra buttons can be mapped with the following IDs:");
    ini.SetComment("Controller 1", "IC1Comment3", "CONTROLLER_BUTTON_ZL           = 22");
    ini.SetComment("Controller 1", "IC1Comment4", "CONTROLLER_BUTTON_ZR           = 23");
    ini.SetComment("Controller 1", "IC1Comment5", "CONTROLLER_BUTTON_LSTICK_UP    = 24");
    ini.SetComment("Controller 1", "IC1Comment6", "CONTROLLER_BUTTON_LSTICK_DOWN  = 25");
    ini.SetComment("Controller 1", "IC1Comment7", "CONTROLLER_BUTTON_LSTICK_LEFT  = 26");
    ini.SetComment("Controller 1", "IC1Comment8", "CONTROLLER_BUTTON_LSTICK_RIGHT = 27");
    ini.SetComment("Controller 1", "IC1Comment9", "CONTROLLER_BUTTON_RSTICK_UP    = 28");
    ini.SetComment("Controller 1", "IC1Comment10", "CONTROLLER_BUTTON_RSTICK_DOWN  = 29");
    ini.SetComment("Controller 1", "IC1Comment11", "CONTROLLER_BUTTON_RSTICK_LEFT  = 30");
    ini.SetComment("Controller 1", "IC1Comment12", "CONTROLLER_BUTTON_RSTICK_RIGHT = 31");
#endif
    ini.SetInteger("Controller 1", "Up", inputDevice[INPUT_UP].contMappings);
    ini.SetInteger("Controller 1", "Down", inputDevice[INPUT_DOWN].contMappings);
    ini.SetInteger("Controller 1", "Left", inputDevice[INPUT_LEFT].contMappings);
    ini.SetInteger("Controller 1", "Right", inputDevice[INPUT_RIGHT].contMappings);
    ini.SetInteger("Controller 1", "A", inputDevice[INPUT_BUTTONA].contMappings);
    ini.SetInteger("Controller 1", "B", inputDevice[INPUT_BUTTONB].contMappings);
    ini.SetInteger("Controller 1", "C", inputDevice[INPUT_BUTTONC].contMappings);
    ini.SetInteger("Controller 1", "Start", inputDevice[INPUT_START].contMappings);

    ini.Write(BASE_PATH"/settings.ini");
}