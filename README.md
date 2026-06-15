# ADB File Manager

A file manager for Linux distributions that utilises Android Debug Bridge (ADB) for file transfer, instead of the traditional Media Transfer Protocol (MTP).

## Requirements

- Any Linux distribution
- Android device
- USB debugging enabled in Developer Settings

## About the project

**Current status**: Work-In-Progress

This project is based on a similar app for Windows, [T0biasCZe](https://github.com/T0biasCZe)'s [AdbFileManager](https://github.com/T0biasCZe/AdbFileManager) written in C#.

Media Transfer Protocol is a hassle when it comes to transferring files, since it is very slow and can sometimes fail randomly. I decided to build a file manager that does not use MTP; it instead uses Android Debug Bridge (ADB) for transferring files, which is much faster than MTP. Occasionally, my Android devices refuse to show up in Nemo for no reason, whereas ADB detects them perfectly fine.

The app is written in C++/Qt so as to make it as fast as possible without compromising on system resource consumption.

### Currently implemented

- [x] List contents of internal storage (`adb shell ls -Ll /sdcard`)
- [x] List devices
- [x] Console log
- [x] Settings window (only persists `-a` flag during `adb pull`)
- [x] Download ADB if not already installed

### To be implemented (highest to lowest priority)

- [ ] Navigation
- [ ] `adb push <source> <destination>` / `adb pull [-a] <source> <destination>`
- [ ] Multi-device support (ability to use one device at a time)
- [ ] Skip downloading ADB if already present

## How to build

This project uses CMake as its build system.

Required dependencies:
- `cmake`
- a C++ compiler (`g++` or `clang`)
- Qt6 development libraries as per your distribution

#### Steps:

1. Clone this repo
```bash
git clone https://github.com/donut6150/AdbFileManager-Linux
cd AdbFileManager-Linux
```

#### A. CLI build

2. Configure build directory
```bash
cmake -S . -B build
```
3. Compile binary
```bash
cmake --build build
```
4. Run
```bash
./build/AdbFileManager
```

#### B. Visual Studio Code / VSCodium:

2. Open the project folder in Visual Studio Code
3. Install the following extensions:
- `Extension Pack for C/C++` by `KylinIdeTeam` (includes `C/C++ Debug`, `CMake IntelliSence`, `Kylin CMake Workflow`)
- `CodeLLDB` by `vadimcn`
- `clangd` by `llvm-vs-code-extensions`
4. Open the Command Palette (`Ctrl`+`Shift`+`P`) > "Reload Window"
5. Command Palette again > "CMake: Configure"
6. You should see a file named `compile_commands.json` appear in the source folder
7. Select configuration from the `Run & Debug` panel (`Debug`/`Release`)
8. Press `F5`, or go to the `Run & Debug` panel and click on the green play button next to `Debug` on top