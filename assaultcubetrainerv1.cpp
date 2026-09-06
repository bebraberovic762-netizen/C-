#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstdio>

int GreetingAndOptions() {
    std::cout << "==================================================" << std::endl;
    std::cout << "                         AXON" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "[*] initializing..." << std::endl;
    Sleep(1500);
    system("cls");

    std::cout << "[*] Welcome to axon" << std::endl;
    std::cout << "[*] Pick:" << std::endl;
    std::cout << "  1. Read Health (1)" << std::endl;
    std::cout << "  2. Write Health (2)" << std::endl;

    int option;
    bool Asking;
    Asking = true;
    while (Asking) {
        std::cin >> option;
        if (option != 1 && option != 2) {
            std::cout << "  [!] Invalid option.";
        } else {
            Asking = false;
        }
    }
    return option;
}

int main()
{
    constexpr uint32_t kBase            = 0x400000;
    constexpr uint32_t kPlayerPtrOffset = 0x0017E0A8; 
    constexpr uint32_t kHealthOffset    = 0xEC;
    constexpr uint32_t kPlayerPtrAddr   = kBase + kPlayerPtrOffset;

    DWORD pid = 0;
    printf("[*] open AssaultCube, then type its PID from Task Manager (Details tab): ");
    if (scanf_s("%lu", &pid) != 1 || pid == 0) {
        printf("[*] bad pid\n");
        return 1;
    }

    HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProc) {
        printf("[*] OpenProcess failed, GetLastError = %lu\n", GetLastError());
        printf("[*] common causes: wrong PID, game closed, or you built x64\n");
        return 1;
    }

    uint32_t playerobj = 0;
    ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(kPlayerPtrAddr), &playerobj, sizeof(playerobj), NULL);
    int hp = 0;
    uint32_t PlayerHealthAtrribute = playerobj + kHealthOffset;

    int choose = GreetingAndOptions();
    bool runningtask;

    if (choose == 1) {
        runningtask = true;
        while (runningtask) {
            ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(PlayerHealthAtrribute), &hp, sizeof(hp), NULL);
            std::cout << "\r [!] Current Health: " << hp << std::flush;
            Sleep(10);
        }
    } else if (choose == 2) {
        int HealthOpt;
        int DesiredHP;

        std::cout << "[*] Do you want a specific health value or the maximum value: " << std::endl;
        std::cout << "[*] Pick:" << std::endl;
        std::cout << "  1. Specific health value (1)" << std::endl;
        std::cout << "  2. Maximum health value (2)" << std::endl;
        bool AskingHealthOpt;
        AskingHealthOpt = true;
        while (AskingHealthOpt) {
            std::cin >> HealthOpt;
            if (HealthOpt != 1 && HealthOpt != 2) {
                std::cout << "  [!] Invalid option.";
            } else {
                AskingHealthOpt = false;
            }
        }

        if (HealthOpt == 1) {
            std::cin >> DesiredHP;
            runningtask = true;
            while (runningtask) {
                if (!WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(PlayerHealthAtrribute), &DesiredHP, sizeof(DesiredHP), NULL)) {
                    std::cout << "[!] WPM Failed, Error: " << GetLastError() << " \n" << std::endl;
                } else {
                    std::cout << "\rHealth initialized." << std::flush;
                    Sleep(5);
                }
            }
        } else {
            std::int32_t MaxGameHPVal = 2147483647; // maximum in game health value is 2147483647 (32 bit long)
            runningtask = true;
            while (runningtask) {
                if (!WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(PlayerHealthAtrribute), &MaxGameHPVal, sizeof(MaxGameHPVal), NULL)) {
                    std::cout << "[!] WPM Failed, Error: " << GetLastError() << " \n" << std::endl;
                } else {
                    std::cout << "\rHealth initialized. (2147483647)" << std::flush;
                    Sleep(5);
                }
            }
        }
    }
}
