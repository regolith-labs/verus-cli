#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "crypto/haraka.h"
#include "crypto/haraka_portable.h"
#include "crypto/verus_hash.h"
#include "crypto/verus_clhash.h" // Includes SOLUTION constants
#include "uint256.h"
#include "utilstrencodings.h"
#include "crypto/verus_clhash.h" // For IsCPUVerusOptimized

static std::vector<unsigned char> hexToBytes(const std::string& hex) {
    std::vector<unsigned char> out;
    out.reserve(hex.size()/2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        unsigned int byte;
        std::istringstream(hex.substr(i, 2)) >> std::hex >> byte;
        out.push_back(static_cast<unsigned char>(byte));
    }
    return out;
}

static std::string bytesToHex(const unsigned char* data, size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        oss << std::setw(2) << static_cast<int>(data[i]);
    }
    return oss.str();
}

static std::string bytesToBigEndianHexStr(const unsigned char* data, size_t len) {
    std::vector<unsigned char> reversed_data(len);
    for (size_t i = 0; i < len; ++i) {
        reversed_data[i] = data[len - 1 - i];
    }
    return bytesToHex(reversed_data.data(), len);
}

int main() {
    // Hardcoded input string from test.js
    std::string input_str = "Test1234Test1234Test1234Test1234Test1234Test1234Test1234Test1234Test1234Test1234Test1234Test1234";

    // Convert the string to bytes (assuming UTF-8 like in test.js)
    std::vector<unsigned char> bytes(input_str.begin(), input_str.end());

    // Print input bytes as hex
    std::cout << "Input Bytes (Hex): " << bytesToHex(bytes.data(), bytes.size()) << std::endl;

    // Initialize hashers
    CVerusHash::init();
    CVerusHashV2::init();

    // Check which implementation is being used
    if (IsCPUVerusOptimized()) {
        std::cout << "INFO: Using native optimized VerusHash implementations (AES-NI, PCLMULQDQ)." << std::endl;
    } else {
        std::cout << "INFO: Using portable C VerusHash implementations." << std::endl;
    }

    // --- Endianness Test ---
    // This section helps demonstrate how bytesToHex renders byte sequences.
    // It also indirectly shows the system's endianness by observing
    // the memory layout of a multi-byte integer.
    std::cout << "\n--- Endianness Test ---" << std::endl;
    uint32_t known_value = 0x01020304; // Represents 0x01 (MSB) ... 0x04 (LSB)
    std::cout << "Known integer value: 0x" << std::hex << known_value << std::dec << std::endl;
    std::cout << "Raw bytes of known_value (using bytesToHex): "
              << bytesToHex(reinterpret_cast<unsigned char*>(&known_value), sizeof(known_value)) << std::endl;
    std::cout << "  Interpretation: If system is little-endian, this will show LSB first (e.g., 04030201)." << std::endl;
    std::cout << "                  If system is big-endian, this will show MSB first (e.g., 01020304)." << std::endl;

    unsigned char big_endian_array[] = {0xAA, 0xBB, 0xCC, 0xDD};
    std::cout << "bytesToHex output for {0xAA, 0xBB, 0xCC, 0xDD}: "
              << bytesToHex(big_endian_array, sizeof(big_endian_array)) << std::endl;
    std::cout << "  Interpretation: bytesToHex prints bytes in the order they appear in the array." << std::endl;
    std::cout << "--- End of Endianness Test ---\n" << std::endl;

    unsigned char hash[32];

    // --- Test VerusHash V1 ---
    CVerusHash hasher_v1;
    hasher_v1.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V1 (Big-Endian Hex)    : " << bytesToBigEndianHexStr(hash, 32) << std::endl;
    std::cout << "Output Hash V1 (Little-Endian Hex) : " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2 (Default) ---
    CVerusHashV2 hasher_v2_default; // Uses default SOLUTION_VERUSHHASH_V2
    hasher_v2_default.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2 (Default) (Big-Endian Hex)    : " << bytesToBigEndianHexStr(hash, 32) << std::endl;
    std::cout << "Output Hash V2 (Default) (Little-Endian Hex) : " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2 (Explicit) ---
    CVerusHashV2 hasher_v2_explicit(SOLUTION_VERUSHHASH_V2);
    hasher_v2_explicit.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2 (Explicit) (Big-Endian Hex) : " << bytesToBigEndianHexStr(hash, 32) << std::endl;
    std::cout << "Output Hash V2 (Explicit) (Little-Endian Hex): " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2.1 ---
    CVerusHashV2 hasher_v2_1(SOLUTION_VERUSHHASH_V2_1);
    hasher_v2_1.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2.1 (Big-Endian Hex)            : " << bytesToBigEndianHexStr(hash, 32) << std::endl;
    std::cout << "Output Hash V2.1 (Little-Endian Hex)         : " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2.2 ---
    CVerusHashV2 hasher_v2_2(SOLUTION_VERUSHHASH_V2_2);
    hasher_v2_2.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2.2 (Big-Endian Hex)            : " << bytesToBigEndianHexStr(hash, 32) << std::endl;
    std::cout << "Output Hash V2.2 (Little-Endian Hex)         : " << bytesToHex(hash, 32) << std::endl;

    // Note: The main hash outputs above now include both big-endian and little-endian hex strings.
    // The bytesToHex function provides raw byte order (little-endian hex on this system),
    // and is also used for input display and the endianness test.
    // The bytesToBigEndianHexStr function provides conventional big-endian hex strings.
    return 0;
}
