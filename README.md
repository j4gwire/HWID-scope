# HWID-scope

This project is a C++ application that retrieves and displays detailed system information, including processor, RAM, motherboard, storage devices, and network interfaces, using the Windows Management Instrumentation (WMI) and Windows API.

## Features

- **Processor Information:** Number of processors, speed, core count, and thread count.
- **RAM Information:** Total physical memory, available memory, size, speed, manufacturer, and part number.
- **Motherboard Information:** Manufacturer and model.
- **Storage Devices Information:** Model and size of storage devices.
- **Network Information:** IP address of enabled network adapters.

## Requirements

- Windows operating system
- Visual Studio or any C++ compiler that supports WMI
- `wbemuuid.lib` for WMI support

## Getting Started

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/j4gwire/HWID-scope.git
   cd HWID-scope
   
### Build the project using Visual Studio or your preferred C++ compiler.
Usage:
Run the executable to display system information:
./hwidscope.exe

### How to Use:
After building the project, navigate to the output directory containing hwidscope.exe.
Double-click the executable or run it from the command line.
The application will display detailed information about your system.

### Code Overview
The main components of the code include:

WMI Queries: Utilizes WMI to fetch data about the system components.
Windows API: Used to access system information and hardware details.
Output Formatting: Information is formatted and displayed in a user-friendly manner.

### Dependencies
wbemuuid.lib: Required for WMI support.
Windows SDK: Ensure you have the Windows SDK installed for access to Windows API functions.

### Credits
This project is inspired by and has improved upon the original HWID Info Grabber by HeathHowren and previously improved by 0vm.
