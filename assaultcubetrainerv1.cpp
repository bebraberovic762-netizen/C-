#include <iostream>
#include <windows.h>
#include <cmath>
#include <string.h>


int main() {
    HWND hwnd = FindWindowA(NULL, "AssaultCube");
    if (hwnd == NULL) {
        std::cout << "Game window not found" << std::endl;
        return 1;
    }

    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);

    HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (handle == NULL) {
        std::cout << "Could not open process" << std::endl;
        return 1;
    }

    
    int HP;
    int HPWrite;
    std::string InfAmmo;
    bool InfAmmoBool;
    auto MaxAmmo = 65535;
    std::cout << "Please enter how much hp you want to be written: ";
    std::cin >> HPWrite;
    std::cout << "Do you want Unlimited ammo? (y/n): ";
    std::cin >> InfAmmo;

    if (InfAmmo == "y") {
        InfAmmoBool = true;
    }
    else {
        InfAmmoBool = false;
    }
    
    bool ReadAndWrite = true;

    while (ReadAndWrite) {
        if (!WriteProcessMemory(handle, (void*)0x0089099C, &HPWrite, sizeof(HPWrite), NULL)) {
            DWORD error = GetLastError();
            std::cout << "[!] WriteProcessMemory failed. Error code: " << error << std::endl;
            ReadAndWrite = false;
        }

        Sleep(20);
        if (!ReadProcessMemory(handle, (void*)0x0089099C, &HP, sizeof(HP), NULL)) {
            DWORD error = GetLastError();
            std::cout << "[!] ReadProcessMemory failed. Error code: " << error << std::endl;
            ReadAndWrite = false;
        }

        Sleep(20);
        
        if (InfAmmoBool) {
            if (!WriteProcessMemory(handle, (void*)0x008909F0, &MaxAmmo, sizeof(MaxAmmo), NULL)) {
                DWORD error = GetLastError();
                std::cout << "[!] WriteProccessMemory failed. Error code: " << error << std::endl;
                ReadAndWrite = false;
            }
        }

        std::cout << "\rCurrent HP: " << HP << "[*] ammo initialized." << std::flush;
        Sleep(20);
    }

    CloseHandle(handle);
    return 0;
}
