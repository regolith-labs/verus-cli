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

    unsigned char hash[32];

    // --- Test VerusHash V1 ---
    CVerusHash hasher_v1;
    hasher_v1.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V1 (Raw Hex)  : " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2 (Default) ---
    CVerusHashV2 hasher_v2_default; // Uses default SOLUTION_VERUSHHASH_V2
    hasher_v2_default.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2 (Default)  : " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2 (Explicit) ---
    CVerusHashV2 hasher_v2_explicit(SOLUTION_VERUSHHASH_V2);
    hasher_v2_explicit.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2 (Explicit) : " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2.1 ---
    CVerusHashV2 hasher_v2_1(SOLUTION_VERUSHHASH_V2_1);
    hasher_v2_1.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2.1          : " << bytesToHex(hash, 32) << std::endl;

    // --- Test VerusHash V2.2 ---
    CVerusHashV2 hasher_v2_2(SOLUTION_VERUSHHASH_V2_2);
    hasher_v2_2.Write(bytes.data(), bytes.size()).Finalize(hash);
    std::cout << "Output Hash V2.2          : " << bytesToHex(hash, 32) << std::endl;

    // Note: test.js reverses the output hex. We are keeping the original C++ behavior here (raw byte order).
    return 0;
}
