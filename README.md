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
   git clone https://github.com/ScribeAegis/HWID-scope.git
   cd HWID-scope

Build the project.

2. Usage:
- Run the executable to display system information:
   ```bash
  ./hwidscope.exe
