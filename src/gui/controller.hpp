#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <SFML/Audio.hpp>
#include <cmath>

// #define EMULATE_OLD_UMPK

#include "../core/disassembler.hpp"
#include "../core/dj.hpp"
#include "../core/umpk80.hpp"



#ifdef EMULATE_OLD_UMPK
#define OS_FILE "./data/old.bin"
#else
#define OS_FILE "./data/scaned-os.bin"
#endif

class Controller {
public:
    Controller(class GuiApp& gui)
        : _umpkThread(&Controller::_umpkWork, this),
          _disasm(*(new std::vector<uint8_t>())) {}

    ~Controller() { _umpkThread.detach(); }

    void decompileToFile(std::string filename, uint16_t fromAdr, uint16_t len);

    void onBtnStart();
    void onButtonStop();
    void onBtnNextCommand();
    void onBtnReset();

    void setUmpkKey(KeyboardKey key, bool value);
    bool isUmpkRunning() { return !_isUmpkFreezed; }

    uint8_t getDisplayDigit(int digit) { return _umpk.getDisplayDigit(digit); }

    uint8_t port5Out() { return _umpk.port5OutGet(); }
    void port5In(uint8_t data);

    void onSetProgramCounter(uint16_t value);

    std::vector<uint8_t> readBinaryFile(std::string path);

    void loadProgramToMemory(uint16_t position, std::vector<uint8_t> &program);
    
    Umpk80 &getUmpk() { return _umpk; }
    Disassembler &getDisasm() { return _disasm; }

private:
    Umpk80 _umpk;
    Disassembler _disasm;
    Dj dj;

    std::thread _umpkThread;
    std::mutex _umpkMutex;

    bool _isUmpkFreezed = true;

private:
    void _loadSystem();
    void _handleHooks(Cpu &cpu);
    void _umpkWork();
    void _copyTestToMemory(uint16_t startAdr, uint8_t *test, size_t size);
};

#endif // CONTROLLER_HPP
