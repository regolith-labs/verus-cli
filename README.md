# VerusHash V2 C++ Reference Implementation

This repository contains a C++ implementation of the VerusHash V2 algorithm. Its primary purpose is to serve as a reference and generate a **golden vector** (a known-good input and its corresponding output hash) for verifying other implementations, such as those in Rust targeting host or BPF environments.

## Purpose: Golden Vector Generation

The `main.cpp` program takes a hardcoded input string, converts it to bytes, and computes hashes using multiple versions of the VerusHash algorithm (V1, V2 default, V2 explicit, V2.1, and V2.2).

We have modified `main.cpp` to explicitly print:

1.  **Input Bytes (Hex):** The hexadecimal representation of the input string's bytes.
2.  **Output Hash (Raw Hex) for each version:** The hexadecimal representation of the raw 32-byte hash result for each tested VerusHash version, *without* any byte reversal.

This raw output is crucial for cross-language/platform verification, avoiding potential endianness confusion that might arise from reversed hex strings (like those used in some Javascript contexts). The output for each version allows for specific verification against different implementations or protocol requirements.

## Build

Use Docker to build the C++ executable in a consistent environment:

```sh
docker build -t verushash-builder .
```


## Run

Run the compiled executable within a Docker container:

```sh
docker run --rm verushash-builder
