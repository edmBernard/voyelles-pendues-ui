# Voyelles Pendues

This project [voyelles pendues](https://github.com/edmBernard/voyelles-pendues-ui) regroup the ui and interface part for a game between hangman game and word search where letter are vowels have been remove from word. The engine part is [voyelles pendues engine](https://github.com/edmBernard/voyelles-pendues-engine)

## Engine Dependencies

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

The project depends on:
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library.
- [spdlog](https://github.com/gabime/spdlog): Fast C++ logging library.
- [utfcpp](https://github.com/nemtrif/utfcpp): UTF-8 with C++ in a Portable Way.

```
./vcpkg install fmt spdlog utfcpp
```

## Build instruction

```bash
mdkir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```

## Logger

```bash
# On windows
$Env:SPDLOG_LEVEL = "debug"
# On Linux
export SPDLOG_LEVEL=debug
```

## Android

For Android we need to use ndk toolchain and vcpkg toolchain. The way to combine both is to add these argument to cmake :
```bash
-DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake
-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE:PATH=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake
-DVCPKG_TARGET_TRIPLET=arm64-android
```

## Disclaimer

It's a toy project. Mainly used to learn Qt Quick and Android development.
Your comments are welcome, if you spot error or improvement.
The original idea was taken from [vowels.io](vowels.io) but I would like to have french dictionnary.
