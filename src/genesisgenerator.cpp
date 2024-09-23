#include "consensus/consensus.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "util.h"
#include "utilstrencodings.h"
#include "crypto/hash.h"
#include <iostream>

int main() {
    // Parameters for the genesis block
    const char* pszTimestamp = "FreeCoin genesis block on September 2023";
    const CScript genesisOutputScript = CScript() << ParseHex("04bfcab5f9e4f6b1e3f1d9...") << OP_CHECKSIG;
    uint32_t nTime = 1695158400; // Use the same nTime as in chainparams.cpp
    uint32_t nNonce = 0;
    uint32_t nBits = 0x1e0ffff0; // Same as in chainparams.cpp
    int32_t nVersion = 1;
    const CAmount genesisReward = 50 * COIN;

    // Create the genesis block
    CBlock genesis = CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);

    // Continuously increment nNonce until the hash meets the difficulty target
    arith_uint256 hashTarget = arith_uint256().SetCompact(nBits);
    while (UintToArith256(genesis.GetHash()) > hashTarget) {
        nNonce++;
        genesis.nNonce = nNonce;
        if (nNonce % 1000000 == 0) {
            std::cout << "Nonce: " << nNonce << ", Hash: " << genesis.GetHash().ToString() << std::endl;
        }
    }

    // Output the results
    std::cout << "Genesis Block Found!" << std::endl;
    std::cout << "nTime: " << nTime << std::endl;
    std::cout << "nNonce: " << nNonce << std::endl;
    std::cout << "Hash: " << genesis.GetHash().ToString() << std::endl;
    std::cout << "Merkle Root: " << genesis.hashMerkleRoot.ToString() << std::endl;

    return 0;
}
