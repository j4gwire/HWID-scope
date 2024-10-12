#include <iostream>
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <vector>
#include <string>
#include <Wbemidl.h>
#include <comdef.h>
#include <fstream>

#pragma comment(lib, "wbemuuid.lib")

// Function to get processor information
void getProcessorInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    std::cout << "Processor Information:" << std::endl;
    std::cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << std::endl;

    // Fetch additional details using WMI
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                            (LPVOID *)&pLoc);
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0,
                               &pSvc);

    hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                            bstr_t("SELECT * FROM Win32_Processor"),
                            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                            NULL, &pEnumerator);

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // Processor speed
        hres = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
        std::cout << "Processor Speed: " << vtProp.uintVal << " MHz" << std::endl;
        VariantClear(&vtProp);

        // Core count and thread count
        hres = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
        std::cout << "Core Count: " << vtProp.uintVal << std::endl;
        VariantClear(&vtProp);

        hres = pclsObj->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
        std::cout << "Thread Count: " << vtProp.uintVal << std::endl;
        VariantClear(&vtProp);
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// Function to get RAM information
void getRAMInfo() {
    std::cout << "\nRAM Information:" << std::endl;

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    std::cout << "Total Physical Memory: " << memInfo.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
    std::cout << "Available Memory: " << memInfo.ullAvailPhys / (1024 * 1024) << " MB" << std::endl;

    // For detailed RAM information using WMI
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                            (LPVOID *)&pLoc);
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0,
                               &pSvc);

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                            bstr_t("SELECT * FROM Win32_PhysicalMemory"),
                            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                            NULL, &pEnumerator);

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // RAM Capacity
        hres = pclsObj->Get(L"Capacity", 0, &vtProp, 0, 0);
        std::cout << "RAM Size: " << vtProp.ullVal / (1024 * 1024) << " MB" << std::endl;
        VariantClear(&vtProp);

        // Speed
        hres = pclsObj->Get(L"Speed", 0, &vtProp, 0, 0);
        std::cout << "RAM Speed: " << vtProp.uintVal << " MHz" << std::endl;
        VariantClear(&vtProp);

        // Manufacturer
        hres = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
        std::wcout << "RAM Manufacturer: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);

        // Part Number
        hres = pclsObj->Get(L"PartNumber", 0, &vtProp, 0, 0);
        std::wcout << "RAM Part Number: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// Function to get motherboard information
void getMotherboardInfo() {
    std::cout << "\nMotherboard Information:" << std::endl;

    // For motherboard information using WMI
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                            (LPVOID *)&pLoc);
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0,
                               &pSvc);

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                            bstr_t("SELECT * FROM Win32_BaseBoard"),
                            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                            NULL, &pEnumerator);

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // Motherboard Manufacturer
        hres = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
        std::wcout << "Manufacturer: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);

        // Motherboard Product
        hres = pclsObj->Get(L"Product", 0, &vtProp, 0, 0);
        std::wcout << "Model: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// Function to get storage devices information
void getStorageDevices() {
    std::cout << "\nStorage Devices Information:" << std::endl;

    // For storage devices using WMI
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                            (LPVOID *)&pLoc);
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0,
                               &pSvc);

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                            bstr_t("SELECT * FROM Win32_DiskDrive"),
                            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                            NULL, &pEnumerator);

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // Storage Device Model
        hres = pclsObj->Get(L"Model", 0, &vtProp, 0, 0);
        std::wcout << "Model: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);

        // Storage Device Size
        hres = pclsObj->Get(L"Size", 0, &vtProp, 0, 0);
        std::cout << "Size: " << vtProp.ullVal / (1024 * 1024 * 1024) << " GB" << std::endl;
        VariantClear(&vtProp);
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// Function to get network information
void getNetworkInfo() {
    std::cout << "\nNetwork Information:" << std::endl;

    // For network interfaces using WMI
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                            (LPVOID *)&pLoc);
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0,
                               &pSvc);

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                            bstr_t("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = TRUE"),
                            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                            NULL, &pEnumerator);

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // IP Address
        hres = pclsObj->Get(L"IPAddress", 0, &vtProp, 0, 0);
        if (vtProp.vt == VT_ARRAY) {
            SAFEARRAY *pSafeArray = vtProp.parray;
            long lLower, lUpper;
            SafeArrayGetLBound(pSafeArray, 1, &lLower);
            SafeArrayGetUBound(pSafeArray, 1, &lUpper);
            for (long i = lLower; i <= lUpper; i++) {
                BSTR bstrVal;
                SafeArrayGetElement(pSafeArray, &i, &bstrVal);
                std::wcout << "IP Address: " << bstrVal << std::endl;
                SysFreeString(bstrVal);
            }
        }
        VariantClear(&vtProp);

        // MAC Address
        hres = pclsObj->Get(L"MACAddress", 0, &vtProp, 0, 0);
        std::wcout << "MAC Address: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// Function to get operating system details
void getOSDetails() {
    std::cout << "\nOperating System Details:" << std::endl;

    // For operating system information using WMI
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
                            (LPVOID *)&pLoc);
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0,
                               &pSvc);

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(bstr_t("WQL"),
                            bstr_t("SELECT * FROM Win32_OperatingSystem"),
                            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                            NULL, &pEnumerator);

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // OS Name
        hres = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
        std::wcout << "OS Name: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);

        // OS Version
        hres = pclsObj->Get(L"Version", 0, &vtProp, 0, 0);
        std::cout << "OS Version: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);

        // Build Number
        hres = pclsObj->Get(L"BuildNumber", 0, &vtProp, 0, 0);
        std::cout << "Build Number: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);
        
        // Uptime
        hres = pclsObj->Get(L"LastBootUpTime", 0, &vtProp, 0, 0);
        std::wcout << "Last Boot Up Time: " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// Function to get audio device information
void getAudioDeviceInfo() {
    std::cout << "\nAudio Device Information:" << std::endl;

    // For audio devices, you can use Windows API or third-party libraries
    // For simplicity, let's just show a message for now
    std::cout << "Audio devices information can be fetched using WASAPI or DirectSound APIs." << std::endl;
}

// Function to get peripheral devices information
void getPeripheralDevices() {
    std::cout << "\nPeripheral Devices Information:" << std::endl;

    // For USB devices, you can use SetupDiGetClassDevs and related functions
    std::cout << "Connected USB devices information can be fetched using SetupAPI." << std::endl;
}

int main() {
    getProcessorInfo();
    getRAMInfo();
    getMotherboardInfo();
    getStorageDevices();
    getNetworkInfo();
    getOSDetails();
    getAudioDeviceInfo();
    getPeripheralDevices();

    return 0;
}
