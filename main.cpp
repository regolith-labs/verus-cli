#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "crypto/haraka.h"
#include "crypto/haraka_portable.h"
#include "crypto/verus_hash.h"
#include "crypto/verus_clhash.h"
#include "uint256.h"
#include "utilstrencodings.h"

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

    // initialize VerusHash V2
    CVerusHashV2::init();

    unsigned char hash[32];
    CVerusHashV2 hasher;
    hasher.Write(bytes.data(), bytes.size()).Finalize(hash);

    // Note: test.js reverses the output hex. We are keeping the original C++ behavior here (raw byte order).
    std::cout << "Output Hash (Raw Hex): " << bytesToHex(hash, 32) << std::endl;
    return 0;
}
