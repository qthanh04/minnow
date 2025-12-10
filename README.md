Stanford CS 144 Networking Lab
==============================

These labs are open to the public under the (friendly, but also mandatory)
condition that to preserve their value as a teaching tool, solutions not
be posted publicly by anybody.

Website: https://cs144.stanford.edu

To set up the build system: `cmake -S . -B build`

To compile: `cmake --build build`

To run tests: `cmake --build build --target check[checkpoint_num]`

or `cmake --build build --target test`

To run speed benchmarks: `cmake --build build --target speed`

To run clang-tidy (which suggests improvements): `cmake --build build --target tidy`

To format code: `cmake --build build --target format`

## macOS Compatibility Notes

This codebase has been modified to support compilation on macOS (Darwin) in addition to Linux. The following changes were made:

### Platform-Specific Headers and Functions

1. **Endianness Functions**: Added compatibility macros for `htobe32()` and `be32toh()` functions (Linux-specific) that use `htonl()` and `ntohl()` on macOS:
   - `util/ipv4_header.cc`
   - `util/arp_message.cc`
   - `util/address.cc`

2. **I/O Vector Structure**: Replaced Linux-specific `#include <bits/types/struct_iovec.h>` with conditional includes:
   - On Linux: uses `<bits/types/struct_iovec.h>`
   - On macOS: uses `<sys/uio.h>`
   - Modified in: `util/file_descriptor.hh`

3. **Linux-Specific Socket Features**:
   - `PacketSocket` class is only compiled on Linux (`util/socket.hh`)
   - `SO_DOMAIN` and `SO_PROTOCOL` socket options are only used on Linux (`util/socket.cc`)
   - `sockaddr_ll` and `AF_PACKET` are conditionally compiled (`util/address.cc`, `util/socket.cc`)

4. **TUN/TAP Devices**: TUN/TAP device support is Linux-only:
   - `util/tun.cc` includes Linux headers conditionally
   - Implementation throws runtime error on macOS

### Code Fixes

1. **Template Member Access**: Fixed `borrowed_obj_` member access in `Ref<T>` template class by using explicit `this->` qualifier (`util/ref.hh`)

2. **String View Support**: Added `#include <string_view>` to `util/ref.hh` for C++17 string_view support

3. **Type Conversions**: Fixed narrowing conversion warnings in `util/socket.cc` for `msg_iovlen` field

4. **Parser Fixes**: 
   - Fixed `min()` function call with explicit type casting (`util/parser.cc`)
   - Changed `release()` to `get()` in `concatenate_all_remaining()` method

5. **Checksum Calculation**: Modified `IPv4Header::compute_checksum()` to iterate over `vector<Ref<string>>` and convert each to `string_view` for `InternetChecksum::add()` (`util/ipv4_header.cc`)

6. **Missing Includes**: Added `#include <sstream>` to `util/helpers.cc`

### Building on macOS

The project should now compile successfully on macOS using the standard build commands. Note that some Linux-specific features (like TUN/TAP devices and packet sockets) will not be available on macOS and will throw runtime errors if used.
