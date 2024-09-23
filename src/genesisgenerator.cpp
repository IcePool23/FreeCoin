#include "arith_uint256.h"
#include "chainparams.h"
#include "consensus/merkle.h"
#include "hash.h"
#include "primitives/block.h"
#include "primitives/transaction.h"
#include "script/script.h"
#include "uint256.h"
#include "utilstrencodings.h"

#include <iostream>
#include <string>

int main() {
    // Parameters for the genesis block
    const char* pszTimestamp = "FreeCoin genesis block on September 2023";
    const CScript genesisOutputScript = CScript() << ParseHex("04bfcab5f9e4f6b1e3f1d9...") << OP_CHECKSIG; // Replace with your actual public key

    uint32_t nTime = 1695158400; // Current UNIX timestamp
    uint32_t nNonce = 0;
    uint32_t nBits = 0x1e0ffff0; // Initial difficulty
    int32_t nVersion = 1;
    CAmount genesisReward = 50 * COIN;

    // Create the genesis block
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    // txNew.nTime = nTime; // Remove or comment out this line
    txNew.vin.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
        << std::vector<unsigned char>(
            (const unsigned char*)pszTimestamp,
            (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout.resize(1);
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    // Calculate the difficulty target
    arith_uint256 hashTarget = arith_uint256().SetCompact(nBits);

    // Mining loop
    uint256 hash;
    while (true) {
        hash = genesis.GetHash();
        if (UintToArith256(hash) <= hashTarget) {
            // Found a valid genesis block
            break;
        }
        if (genesis.nNonce % 1000000 == 0) {
            std::cout << "nNonce: " << genesis.nNonce << " Hash: " << hash.ToString() << std::endl;
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
            // If nNonce overflows, increment nTime
            ++genesis.nTime;
        }
    }

    // Output the results
    std::cout << "Genesis Block Found!" << std::endl;
    std::cout << "nTime: " << genesis.nTime << std::endl;
    std::cout << "nNonce: " << genesis.nNonce << std::endl;
    std::cout << "Hash: " << hash.ToString() << std::endl;
    std::cout << "Merkle Root: " << genesis.hashMerkleRoot.ToString() << std::endl;

    return 0;
}
