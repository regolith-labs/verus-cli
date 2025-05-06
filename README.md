# VerusHash V2 C++ Reference Implementation

This repository contains a C++ implementation of the VerusHash V2 algorithm. Its primary purpose is to serve as a reference and generate a **golden vector** (a known-good input and its corresponding output hash) for verifying other implementations, such as those in Rust targeting host or BPF environments.

## Purpose: Golden Vector Generation

The `main.cpp` program takes a hardcoded input string, converts it to bytes, and computes hashes using multiple versions of the VerusHash algorithm (V1, V2 default, V2 explicit, V2.1, and V2.2).

We have modified `main.cpp` to explicitly print:

1.  **Input Bytes (Hex):** The hexadecimal representation of the input string's bytes, printed in raw order using the `bytesToHex` function.
2.  **Output Hash (Big-Endian Hex) for each version:** The hexadecimal representation of the 32-byte hash result, with bytes reversed before hex conversion to produce a conventional big-endian string (e.g., `hash[31]` printed first, then `hash[30]`, ..., `hash[0]`). This is done using the `bytesToBigEndianHexStr` function and aligns with common hash display formats, like that in `test.js`.
3.  **Output Hash (Little-Endian Hex) for each version:** The hexadecimal representation of the 32-byte hash result, printed in raw byte order as it exists in memory (e.g., `hash[0]` printed first, then `hash[1]`, ..., `hash[31]`). This is done using the `bytesToHex` function. On typical x86/x64 systems, this will appear as a little-endian hex string.

This provides both conventional big-endian hex output for easy comparison with tools and explorers, and little-endian (raw byte order) hex output for direct byte-level inspection. The behavior of `bytesToHex` is also demonstrated by the "Endianness Test" in `main.cpp`.

### Output Byte Order (Endianness)

`main.cpp` now uses two helper functions for converting byte arrays to hexadecimal strings:

1.  **`bytesToHex(data, len)`:** This function prints the raw byte sequence of its input as it exists in memory. `data[0]` is converted to hex and printed first, then `data[1]`, and so on. This is effectively a big-endian representation *of the byte array itself* (the first byte of the array is the first byte printed). This function is used to display the "Input Bytes (Hex)", the "Little-Endian Hex" representation of output hashes, and in the "Endianness Test".
2.  **`bytesToBigEndianHexStr(data, len)`:** This function first reverses the order of bytes in the input `data` (e.g., `data[len-1]` becomes the first byte, `data[0]` the last) and then converts this reversed sequence to a hexadecimal string using logic similar to `bytesToHex`. This produces a conventional big-endian hexadecimal string for the hash value, which is standard for hash representation in many contexts (e.g., `test.js` also produces this format). This function is used for displaying the "Big-Endian Hex" representation of output hashes.

To demonstrate the behavior of `bytesToHex` (raw memory dump) and the system's underlying endianness, an "Endianness Test" section remains in `main.cpp`. When run, it will output:
1.  A known 32-bit integer (`0x01020304`).
2.  The raw byte representation of this integer as printed by `bytesToHex`.
    *   On a **little-endian** system (common for x86/x64 CPUs), the memory layout for `0x01020304` is `04 03 02 01`. `bytesToHex` will print `04030201`.
    *   On a **big-endian** system, the memory layout is `01 02 03 04`. `bytesToHex` will print `01020304`.
3.  The output of `bytesToHex` for a predefined byte array `{0xAA, 0xBB, 0xCC, 0xDD}`. This will always print as `aabbccdd`, confirming `bytesToHex` prints bytes in their given array order.

This test confirms that `bytesToHex` provides a direct, raw byte dump (which appears as little-endian for multi-byte values on your system). The "Output Hash" sections now provide both this raw/little-endian view and the conventional big-endian representation via `bytesToBigEndianHexStr` for easier comparison with other tools.

## Build

Use Docker to build the C++ executable in a consistent environment:

```sh
docker build -t verushash-builder .
```


## Run

Run the compiled executable within a Docker container:

```sh
docker run --rm verushash-builder
