# RSDKv2 Decompilation (OUYA Port)

An OUYA port of the [RSDKv2 decompilation](https://github.com/RSDKModding/RSDKv2-Decompilation) by RSDKModding, a complete decompilation of the Retro-Sonic Engine v2. It runs the Sonic Nexus 2008 demo.

This is based on my [Android port](https://github.com/Moonfire2560/RSDKv2-Decompilation-Android), with changes for the OUYA's hardware and controller (Android 4.1, 32-bit ARM, no Start or Select buttons).

Download the latest APK from the [Releases page](https://github.com/Moonfire2560/RSDKv2-Decompilation-OUYA/releases).

# Files you need

This port does **not** include any game files. You must supply `Data.bin` from your own copy of the Sonic Nexus 2008 demo. You can find information about the game [here](https://info.sonicretro.org/Sonic_Nexus).

Put it here on the OUYA's internal storage:

```
Internal storage (root)
└── RSDK
    └── v2
        └── Data.bin
```

Full path: `/sdcard/RSDK/v2/Data.bin`

1. Install the APK and launch it once. The game creates the `RSDK/v2` folder for you and tells you if `Data.bin` is missing.
2. Copy `Data.bin` into `/sdcard/RSDK/v2/` (for example with `adb push Data.bin /sdcard/RSDK/v2/`).
3. Launch the game again.

The game finds itself in the OUYA's **Play** menu, with no need to favorite it from Make -> Software.

# Controls

The OUYA controller has no Start or Select button, and the system (OUYA) button never reaches games, so two stick clicks are used instead:

| OUYA controller | Action |
| --- | --- |
| Left stick / D-pad | Move |
| O and U | Jump |
| R3 (click right stick) | Start |
| L3 (click left stick) | Select (acts as `ESC` in the dev menu) |

These are the defaults. Every button can be remapped in `settings.ini` under `[Controller 1]`. In this port the IDs 0-21 are the OUYA pad's raw button numbers (not SDL controller button values), and the file lists them in comments:

`O = 0, A = 1, U = 2, Y = 3, L3 = 7, R3 = 8, L1 = 9, R1 = 10, D-pad Up/Down/Left/Right = 11/12/13/14, L2 = 15, R2 = 16`

IDs 22 and up are the engine's extra IDs (triggers and stick directions) and are listed in the file too.

# Settings

`settings.ini` is created in `/sdcard/RSDK/v2/` on first launch. Two settings you may want to change:

- `DevMenu` - defaults to `false`. Set it to `true` to boot into the dev menu, which is also where mods are enabled.
- `ScreenWidth` - defaults to `320` (320x240). A wider value such as `424` gives a 16:9 picture (424x240) when used with a widescreen mod.

# Supported devices

Built for the original OUYA (Tegra 3, Android 4.1, `armeabi-v7a`). The game is very lightweight and runs on the OUYA without trouble.

The build also contains the Android changes from my Android port up to version 1.0.3 (Android TV launcher entry, cutscene fix and audio fixes).

---

# Building

- **Android SDK Platform:** 33, **NDK:** 22.1.7171670, **CMake:** 3.22.1+
- **SDL2:** 2.30.4, statically linked
- **Min SDK:** 16 (Android 4.1), **ABI:** `armeabi-v7a`
- **C++ runtime:** `c++_static` (needed on the OUYA)

The game data path is `/sdcard/RSDK/v2` (set as `BASE_PATH` in `android/app/src/main/cpp/CMakeLists.txt`).

The project expects a prebuilt SDL2 static library at `android/app/src/main/cpp/SDL2-2.30.4/build/armeabi-v7a/libSDL2.a`. It also expects `libcpufeatures.a` at `android/app/src/main/cpp/SDL2-2.30.4/obj/local/armeabi-v7a/`. Neither is stored in the repo.

To make them, build SDL2 with the NDK's `ndk-build` (this produces both `libSDL2.a` and `libcpufeatures.a` in `obj/local/armeabi-v7a/`), then copy `libSDL2.a` to `build/armeabi-v7a/`. On Windows, from a Command Prompt (replace `[NDK path]` with your NDK 22.1.7171670 folder, for example inside `Android\Sdk\ndk\`):

```
cd android\app\src\main\cpp\SDL2-2.30.4
[NDK path]\ndk-build.cmd NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk APP_PLATFORM=android-16 APP_ABI=armeabi-v7a
mkdir build\armeabi-v7a
copy obj\local\armeabi-v7a\libSDL2.a build\armeabi-v7a\
```

Then open the `android` folder in Android Studio and build, or run `./gradlew assembleDebug` from `android`.

# About the decompilation

Without assets from the Sonic Nexus 2008 demo, this decompilation will not run.

Additional tweaks over the original game, from the decompilation project:
* A built-in mod loader, allowing to easily create and play mods.
* A `settings.ini` file that the game uses to load all settings, similar to Sonic Mania.
* The dev menu can be accessed from anywhere by pressing `ESC` (on this port: L3) if enabled in the config.
* The `F12` pause and `F11` step over & fast forward debug features from Sonic Mania are ported and enabled if `devMenu` is enabled in the config.

For other platforms (Windows, Mac, Linux and more), see the [original decompilation](https://github.com/RSDKModding/RSDKv2-Decompilation).

# Credits and license

- Original RSDK (Retro-Engine) author: Christian "Taxman" Whitehead
- Decompilation authors: Rubberduckycooly and RMGRich
- OUYA port: Moonfire2560

This project is under the license in [LICENSE.md](./LICENSE.md): no commercial use, credit the original and decompilation developers, and do not distribute game assets.

# Contact

Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any extra questions you may need to know about the decompilation or modding it.
