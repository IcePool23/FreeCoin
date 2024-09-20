// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021 The Free Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include "chainparamsseeds.h"

//TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}
/*

// Set the difficulty target for KAWPOW (adjust for testing or real deployment)
arith_uint256 test;
bool fNegative;
bool fOverflow;
test.SetCompact(0x207fffff, &fNegative, &fOverflow);  // Low difficulty for easier mining
std::cout << "Test threshold: " << test.GetHex() << "\n\n";

int genesisNonce = 0;
uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

for (int i = 0; i < 40000000; i++) {
    // Try mining a new genesis block with the incremented nonce
    genesis = CreateGenesisBlock(1725667200, i, 0x207fffff, 1, 50 * COIN);  // Adjust time and reward

    // Calculate the block hash using KAWPOW algorithm
    consensus.hashGenesisBlock = genesis.GetKAWPOWHash();

    // Keep track of the best hash (smallest value)
    arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
    if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
        BestBlockHash = consensus.hashGenesisBlock;
        std::cout << "Best Block Hash: " << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
        std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
    }

    // Store the previous hash
    TempHashHolding = consensus.hashGenesisBlock;

    // If the hash meets the difficulty target, stop mining
    if (UintToArith256(BestBlockHash) < test) {
        genesisNonce = i;
        break;
    }
}

// Print the final mined genesis block details
std::cout << "\nGenesis Block Mined:\n";
std::cout << "hashGenesisBlock: " << BestBlockHash.GetHex() << std::endl;
std::cout << "Genesis Nonce: " << genesisNonce << std::endl;
std::cout << "Genesis Merkle Root: " << genesis.hashMerkleRoot.GetHex() << std::endl;

// Finalize the genesis block with the mined values
genesis.nNonce = genesisNonce;
genesis.hashPrevBlock = TempHashHolding;

// Set the consensus hash to the mined genesis block hash
consensus.hashGenesisBlock = genesis.GetKAWPOWHash();

// After mining, use these assert statements to lock in the correct values
assert(consensus.hashGenesisBlock == uint256S("0x" + BestBlockHash.GetHex()));
assert(genesis.hashMerkleRoot == uint256S("0x" + genesis.hashMerkleRoot.GetHex()));





/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "BTC reaches new all time high. 2024";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
	consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
	consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
	consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
	consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
	consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
	//return consensus.nBIP34Enabled;
	return consensus.nBIP65Enabled;
}
bool CChainParams::BIP66() {
	//return consensus.nBIP34Enabled;
	return consensus.nBIP66Enabled;
}

bool CChainParams::CSVEnabled() const{
	return consensus.nCSVEnabled;
}


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;  //~ 4 yrs at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
	consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // Approx 80% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 0; // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (RIP2)
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1540944000; // Oct 31, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1572480000; // Oct 31, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1578920400; // UTC: Mon Jan 13 2020 13:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1610542800; // UTC: Wed Jan 13 2021 13:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1588788000; // UTC: Wed May 06 2020 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1620324000; // UTC: Thu May 06 2021 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1593453600; // UTC: Mon Jun 29 2020 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1624989600; // UTC: Mon Jun 29 2021 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1597341600; // UTC: Thu Aug 13 2020 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1628877600; // UTC: Fri Aug 13 2021 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016       consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;


        // The best chain should have at least this much work
        consensus.nMinimumChainWork = uint256S("0x00"); // Block 2383567

        // By default assume that the signatures in ancestors of this block are valid. Block# 1040000
        consensus.defaultAssumeValid = uint256S("0x00"); // Block 2383560

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x46; // F
        pchMessageStart[1] = 0x52; // R
        pchMessageStart[2] = 0x45; // E
        pchMessageStart[3] = 0x45; // E
        nDefaultPort = 9876;
        nPruneAfterHeight = 100000;

	//genesis = CreateGenesisBlock(1725667200, 1, 0x207fffff, 1, 50 * COIN);

	// Print details of the created genesis block
std::cout << "Genesis block details:" << std::endl;
std::cout << "block.nTime = " << genesis.nTime << std::endl;
std::cout << "block.nNonce = " << genesis.nNonce << std::endl;
std::cout << "block.nBits = " << genesis.nBits << std::endl;
std::cout << "block.nVersion = " << genesis.nVersion << std::endl;
//std::cout << "block.GetHash() = " << genesis.GetHash().ToString() << std::endl;
std::cout << "block.hashMerkleRoot = " << genesis.hashMerkleRoot.ToString() << std::endl;
std::cout << "block.GetKAWPOWHeaderHash() = " << genesis.GetKAWPOWHeaderHash().ToString() << std::endl;

bool fMineGenesisBlock = false;  // Manually set this to true to mine the genesis block

if (fMineGenesisBlock) {
    // Print information to mine the genesis block
    std::cout << "Mining the genesis block for Freecoin..." << std::endl;

    arith_uint256 test;
    bool fNegative;
    bool fOverflow;
    
    // Set difficulty target for genesis block mining (adjust as needed)
    test.SetCompact(0x207fffff, &fNegative, &fOverflow);
//    std::cout << "Test threshold: " << test.GetHex() << "\n\n";
    std::cout << "Difficulty target (threshold): " << test.GetHex() << "\n\n";

    int genesisNonce = 0;
    uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
    uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    // Timestamp for the genesis block (adjust nGenesisTime if needed)
    int64_t nGenesisTime = 1725667200; // Adjust to current or significant timestamp

    // Start mining loop
    for (int i = 0; i < 40000000; i++) {
        // Create the genesis block with current nonce, difficulty bits, and block reward
        genesis = CreateGenesisBlock(nGenesisTime, i, 0x207fffff, 1, 50 * COIN);
        
        // Get the hash of the genesis block (adjust if using a specific algorithm later)
        consensus.hashGenesisBlock = genesis.GetKAWPOWHeaderHash();

        // Convert the current best block hash to arithmetic for comparison
        arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);

            // Debug output for each nonce iteration
        if (i % 10000 == 0) {  // Print every 10,000 nonce increments
            std::cout << "Trying nonce: " << i << std::endl;
            std::cout << "Current hash: " << consensus.hashGenesisBlock.GetHex() << std::endl;
            std::cout << "Best hash so far: " << BestBlockHash.GetHex() << std::endl;
        }


        // Check if the newly mined hash is better than the previous best hash
        if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
            BestBlockHash = consensus.hashGenesisBlock;
	    std::cout << "New best hash found!" << std::endl;
            std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
            std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        }

        // Update the temporary holding hash
        TempHashHolding = consensus.hashGenesisBlock;

        // Check if the newly mined hash meets the difficulty target
        if (BestBlockHashArith < test) {
            genesisNonce = i; // Store the nonce that found the valid hash
	    std::cout << "Valid genesis block mined!" << std::endl;
            break; // Exit the loop once the genesis block is successfully mined
        }
    }

    // Print the results of the mining process
    std::cout << "\n\nGenesis block mined successfully!\n";
    std::cout << "hashGenesisBlock: 0x" << BestBlockHash.GetHex() << std::endl;
    std::cout << "Genesis Nonce: " << genesisNonce << std::endl;
    std::cout << "Genesis Merkle Root: " << genesis.hashMerkleRoot.GetHex() << std::endl;

    // Once the block is mined, you can update the genesis block with the mined hash
    genesis.hashPrevBlock = TempHashHolding;

    // Insert the found nonce, block hash, and merkle root into the code
    consensus.hashGenesisBlock = BestBlockHash;
    genesis.nNonce = genesisNonce;

    // Return after mining
    return;
}


/*
if (fMineGenesisBlock) {
            // Mining the genesis block
            std::cout << "Mining the genesis block..." << std::endl;

            arith_uint256 test;
            bool fNegative;
            bool fOverflow;
            test.SetCompact(0x207fffff, &fNegative, &fOverflow);  // Difficulty target for genesis block mining

            int genesisNonce = 0;
            uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
            uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

            for (int i = 0; i < 40000000; i++) {
                // Attempt to mine the genesis block with incremented nonce
                genesis = CreateGenesisBlock(1725667200, i, 0x207fffff, 1, 50 * COIN);  // Adjust timestamp and reward

                consensus.hashGenesisBlock = genesis.GetKAWPOWHash();  // Use KAWPOW for mining

                arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
                if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
                    BestBlockHash = consensus.hashGenesisBlock;
                    std::cout << "Best Block Hash: " << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
                    std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
                }

                TempHashHolding = consensus.hashGenesisBlock;

                // Break if a valid genesis block hash is found
                if (UintToArith256(BestBlockHash) < test) {
                    genesisNonce = i;
                    break;
                }
            }

            // Print the mined genesis block details
            std::cout << "\nGenesis Block Mined:\n";
            std::cout << "hashGenesisBlock: " << BestBlockHash.GetHex() << std::endl;
            std::cout << "Genesis Nonce: " << genesisNonce << std::endl;
            std::cout << "Genesis Merkle Root: " << genesis.hashMerkleRoot.GetHex() << std::endl;

            // Finalize the genesis block with the mined values
            genesis.nNonce = genesisNonce;
            genesis.hashPrevBlock = TempHashHolding;

            // Set the consensus hash for the mined genesis block
            consensus.hashGenesisBlock = genesis.GetKAWPOWHash();
        } else {
            // Use hardcoded genesis block values (after mining, replace these with the mined values)
            genesis = CreateGenesisBlock(1725667200, 2083236893, 0x1d00ffff, 1, 50 * COIN);  // Replace nonce and bits after mining
            consensus.hashGenesisBlock = genesis.GetKAWPOWHash();
            assert(consensus.hashGenesisBlock == uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"));  // Replace with mined hash
            assert(genesis.hashMerkleRoot == uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"));  // Replace with mined Merkle root
        }

/*if (fMineGenesisBlock || genesis.GetHash() != uint256S("0x0000000000000000000000000000000000000000000000000000000000000000")) {
    // Print some debug information
    std::cout << "Mining the genesis block..." << std::endl;
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
    uint256 thash;

    // Mining loop
    while (true) {
        thash = genesis.GetHash();
        if (UintToArith256(thash) <= hashTarget)
            break;
        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
            ++genesis.nTime;
        }
    }

    std::cout << "Genesis block mined!" << std::endl;
    std::cout << "block.nTime = " << genesis.nTime << std::endl;
    std::cout << "block.nNonce = " << genesis.nNonce << std::endl;
    std::cout << "block.GetHash = " << genesis.GetHash().ToString() << std::endl;
}*/

/*
if (fMineGenesisBlock || genesis.GetHash() != uint256S("0x0000000000000000000000000000000000000000000000000000000000000000")) {
    // Print some debug information
    std::cout << "Mining the genesis block..." << std::endl;
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
    uint256 thash;

    // Mining loop
    int debugInterval = 100000; // Adjust this value to control how often to print debug info
    int count = 0;

    while (true) {
        thash = genesis.GetHash();

        // Print debug information at regular intervals
        if (++count % debugInterval == 0) {
            std::cout << "Current nonce: " << genesis.nNonce << std::endl;
            std::cout << "Current time: " << genesis.nTime << std::endl;
            std::cout << "Current hash: " << thash.ToString() << std::endl;
        }

	        // Print current hash and compare to target
        if (++count % debugInterval == 0) {
            std::cout << "Current hash: " << thash.ToString() << std::endl;
            std::cout << "Hash is " << (UintToArith256(thash) <= hashTarget ? "below" : "above") << " target." << std::endl;
        }


        if (UintToArith256(thash) <= hashTarget) {
            std::cout << "Genesis block mined!" << std::endl;
            std::cout << "Final block.nTime = " << genesis.nTime << std::endl;
            std::cout << "Final block.nNonce = " << genesis.nNonce << std::endl;
            std::cout << "Final block.GetHash = " << genesis.GetHash().ToString() << std::endl;
            break;
        }

        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
            ++genesis.nTime;
        }
    }
}
*/

/*

	if (fMineGenesisBlock || genesis.GetHash() != uint256S("0x0000000000000000000000000000000000000000000000000000000000000000")) {
    std::cout << "Mining the genesis block..." << std::endl;
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
    uint256 thash;

    int debugInterval = 100000; // Adjust this value to control how often to print debug info
    int count = 0;

    std::cout << "Initial block state:" << std::endl;
    std::cout << "Initial block.nTime = " << genesis.nTime << std::endl;
    std::cout << "Initial block.nNonce = " << genesis.nNonce << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        thash = genesis.GetHash();

        // Print block header details and hash comparison
        if (++count % debugInterval == 0) {
            std::cout << "Current nonce: " << genesis.nNonce << std::endl;
            std::cout << "Current time: " << genesis.nTime << std::endl;
            std::cout << "Hash Target: " << hashTarget.ToString() << std::endl;
            std::cout << "Current hash: " << thash.ToString() << std::endl;
            std::cout << "Hash value: " << UintToArith256(thash).ToString() << std::endl;
            std::cout << "Target value: " << hashTarget.ToString() << std::endl;
            std::cout << "Hash is " << (UintToArith256(thash) <= hashTarget ? "below" : "above") << " target." << std::endl;
        }

        if (UintToArith256(thash) <= hashTarget) {
            std::cout << "Genesis block mined!" << std::endl;
            std::cout << "Final block.nTime = " << genesis.nTime << std::endl;
            std::cout << "Final block.nNonce = " << genesis.nNonce << std::endl;
            std::cout << "Final block.GetHash = " << genesis.GetHash().ToString() << std::endl;
            break;
        }

        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
            ++genesis.nTime;
            std::cout << "Nonce wrapped around to 0, incrementing time." << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Mining took " << elapsed.count() << " seconds." << std::endl;
}

*/
/*
if (fMineGenesisBlock || genesis.GetHash() != uint256S("0x0000000000000000000000000000000000000000000000000000000000000000")) {
    std::cout << "Mining the genesis block..." << std::endl;
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
    std::cout << "Hash Target: " << hashTarget.ToString() << std::endl;
    uint256 thash;

    int debugInterval = 100000; // Adjust this value to control how often to print debug info
    int count = 0;

    std::cout << "Initial block state:" << std::endl;
    std::cout << "Initial block.nTime = " << genesis.nTime << std::endl;
    std::cout << "Initial block.nNonce = " << genesis.nNonce << std::endl;
    std::cout << "Difficulty Bits (nBits): " << genesis.nBits << std::endl;

    // Print target value and hash function outputs
    std::cout << "Hash Target: " << hashTarget.ToString() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        thash = genesis.GetHash();

        // Print block header details and hash comparison
        if (++count % debugInterval == 0) {
            std::cout << "Current nonce: " << genesis.nNonce << std::endl;
            std::cout << "Current time: " << genesis.nTime << std::endl;
            std::cout << "Current hash: " << thash.ToString() << std::endl;
            std::cout << "Hash value: " << UintToArith256(thash).ToString() << std::endl;
            std::cout << "Target value: " << hashTarget.ToString() << std::endl;
            std::cout << "Hash is " << (UintToArith256(thash) <= hashTarget ? "below" : "above") << " target." << std::endl;
        }

        if (UintToArith256(thash) <= hashTarget) {
            std::cout << "Genesis block mined!" << std::endl;
            std::cout << "Final block.nTime = " << genesis.nTime << std::endl;
            std::cout << "Final block.nNonce = " << genesis.nNonce << std::endl;
            std::cout << "Final block.GetHash = " << genesis.GetHash().ToString() << std::endl;
            break;
        }

        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
            ++genesis.nTime;
            std::cout << "Nonce wrapped around to 0, incrementing time." << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Mining took " << elapsed.count() << " seconds." << std::endl;
}
*/
/*
if (fMineGenesisBlock || genesis.GetHash() != uint256S("0x0000000000000000000000000000000000000000000000000000000000000000")) {
    std::cout << "Mining the genesis block..." << std::endl;
    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
    uint256 thash;

    while (true) {
        thash = genesis.GetHash();

        // Debug output
        std::cout << "Current nonce: " << genesis.nNonce << std::endl;
        std::cout << "Current time: " << genesis.nTime << std::endl;
        std::cout << "Current hash: " << thash.ToString() << std::endl;
        std::cout << "Hash value: " << UintToArith256(thash).ToString() << std::endl;
        std::cout << "Target value: " << hashTarget.ToString() << std::endl;
        std::cout << "Hash is " << (UintToArith256(thash) <= hashTarget ? "below" : "above") << " target." << std::endl;

        if (UintToArith256(thash) <= hashTarget) {
            std::cout << "Genesis block mined!" << std::endl;
            std::cout << "Final block.nTime = " << genesis.nTime << std::endl;
            std::cout << "Final block.nNonce = " << genesis.nNonce << std::endl;
            std::cout << "Final block.GetHash = " << genesis.GetHash().ToString() << std::endl;
            break;
        }

        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
            ++genesis.nTime;
        }
    }
}


*/



/*
	genesis = CreateGenesisBlock(1725667200, 14795909, 0x207fffff, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetKAWPOWHeaderHash();

        assert(consensus.hashGenesisBlock == uint256S("0x"));
        assert(genesis.hashMerkleRoot == uint256S("0x"));

*/
/*
bool fMineGenesisBlock = true;

if (fMineGenesisBlock) {
    std::cout << "Mining the genesis block for Freecoin using KAWPOW..." << std::endl;

    arith_uint256 target;
    bool fNegative;
    bool fOverflow;

    // Set the difficulty target (adjust if needed)
    target.SetCompact(0x207fffff, &fNegative, &fOverflow);
    std::cout << "Difficulty target (threshold): " << target.GetHex() << "\n\n";

    int genesisNonce = 0;
    uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
    uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    int64_t nGenesisTime = 1725667200;  // Adjust the timestamp if needed

    // Mining loop to find the genesis block
    for (int i = 0; i < 40000000; i++) {
        genesis = CreateGenesisBlock(nGenesisTime, i, 0x207fffff, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetKAWPOWHeaderHash();

        arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);

        if (i % 10000 == 0) {  // Print updates every 10,000 nonce increments
            std::cout << "Trying nonce: " << i << std::endl;
            std::cout << "Current hash: " << consensus.hashGenesisBlock.GetHex() << std::endl;
            std::cout << "Best hash so far: " << BestBlockHash.GetHex() << std::endl;
        }

        // Check if the current hash is better than the best hash so far
        if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
            BestBlockHash = consensus.hashGenesisBlock;
            std::cout << "New best hash found: " << BestBlockHash.GetHex() << " with nonce: " << i << "\n";
        }

        TempHashHolding = consensus.hashGenesisBlock;

        // Check if the current hash meets the difficulty target
        if (BestBlockHashArith < target) {
            genesisNonce = i;
            std::cout << "Valid genesis block mined!" << std::endl;
            break;
        }
    }

    // Output the results
    std::cout << "\n\nGenesis block mined successfully!\n";
    std::cout << "hashGenesisBlock: 0x" << BestBlockHash.GetHex() << std::endl;
    std::cout << "Genesis Nonce: " << genesisNonce << std::endl;
    std::cout << "Genesis Merkle Root: " << genesis.hashMerkleRoot.GetHex() << std::endl;

    genesis.hashPrevBlock = TempHashHolding;

    // Set the found nonce and hash in the code
    consensus.hashGenesisBlock = BestBlockHash;
    genesis.nNonce = genesisNonce;

    return;  // Exit after mining
}



*/

	// Ensure the genesis block has no parent block
//	genesis.hashPrevBlock.SetNull();
	// Hardcode the mined genesis block values
//	genesis = CreateGenesisBlock(1725667200, 1, 0x207fffff, 1, 50 * COIN);

	// Use the KAWPOW hash function
//	consensus.hashGenesisBlock = genesis.GetKAWPOWHeaderHash();

	// Update assertions with the correct genesis block hash and Merkle root
//	assert(consensus.hashGenesisBlock == uint256S("0x62eb42f3cc7ac29adbf72a29f44d584462ce9a5badf38abe83a6af570902e271"));
//	assert(genesis.hashMerkleRoot == uint256S("0xa6fd779b45b3d852831b0cf77b680f2c1cb0492a508b14e6daeacacad9eab03a"));

	genesis.hashPrevBlock.SetNull();  // Ensure this is set for the genesis block

	// Set the genesis block parameters.
	genesis = CreateGenesisBlock(1725667200, 1, 0x207fffff, 1, 50 * COIN);

	// Verify that the calculated genesis block hash and merkle root match.
	consensus.hashGenesisBlock = genesis.GetKAWPOWHeaderHash();
	assert(consensus.hashGenesisBlock == uint256S("0x62eb42f3cc7ac29adbf72a29f44d584462ce9a5badf38abe83a6af570902e271"));
	assert(genesis.hashMerkleRoot == uint256S("0xa6fd779b45b3d852831b0cf77b680f2c1cb0492a508b14e6daeacacad9eab03a"));





        //genesis = CreateGenesisBlock(1514999494, 25023712, 0x1e00ffff, 4, 5000 * COIN);
	//genesis = CreateGenesisBlock(1722460661, 106315, 0x1e0ffff0, 1, 50 * COIN);
	//genesis = CreateGenesisBlock(1722463036, 70010, 0x1e0ffff0, 1, 50 * COIN);

        //consensus.hashGenesisBlock = genesis.GetX16RHash();


	//consensus.hashGenesisBlock = genesis.GetX16RHash();

	//genesis = CreateGenesisBlock("BTC reaches new all time high. 2024", 
        //CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG, 
        //1724006558, 47167, 0x1e0ffff0, 1, 50 * COIN);

	//assert(consensus.hashGenesisBlock == uint256S("0x0000586db229f6c4c9da641289dd832de45943d0f0f39d1b63bd6496cf45516d"));
	//assert(genesis.hashMerkleRoot == uint256S("0x"));

        //consensus.hashGenesisBlock = genesis.GetX16RHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x00001312d5eb5473df9206acbbc554af7ba93e70f0ff2cc74665e13ab6395173"));
        //assert(genesis.hashMerkleRoot == uint256S("0x5838681a8a58b109ea5eb580344fd04116ee5d1f3c054a412aeb17aefcd43576"));

        //assert(consensus.hashGenesisBlock == uint256S("0000006b444bc2f2ffe627be9d9e7e7a0730000870ef6eb6da46c8eae389df90"));
        //assert(genesis.hashMerkleRoot == uint256S("28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));
	//assert(consensus.hashGenesisBlock == uint256S("0x0000be3f53ebd1f05698638cbabfce679c480fc4036ffd3d7d4dec4e8e660fdd"));
	//assert(genesis.hashMerkleRoot == uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"));


	//genesis = CreateGenesisBlock(1514999494, 61428, 0x1e00ffff, 4, 5000 * COIN);

	//consensus.hashGenesisBlock = genesis.GetX16RHash();

	//assert(consensus.hashGenesisBlock == uint256S("000003cb3f7f2814cc61f4bf31a4d838ed039bbbbb45cfcb8b9b45864cb12876"));
	//assert(genesis.hashMerkleRoot == uint256S("5f2d19d9094253cc9429a6f3e326fe3ed6f61e384f4b98fcd072aab4ff186fad"));




        //vSeeds.emplace_back("seed-free.bitactivate.com", false);
        //vSeeds.emplace_back("seed-free.freecoin.com", false);
        //vSeeds.emplace_back("seed-free.freecoin.org", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,80);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,140);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,150);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // Free BIP44 cointype in mainnet is '175'
        nExtCoinType = 175;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                { 535721, uint256S("0x000000000001217f58a594ca742c8635ecaaaf695d1a63f6ab06979f1c159e04")},
                { 697376, uint256S("0x000000000000499bf4ebbe61541b02e4692b33defc7109d8f12d2825d4d2dfa0")},
                { 740000, uint256S("0x00000000000027d11bf1e7a3b57d3c89acc1722f39d6e08f23ac3a07e16e3172")},
                { 909251, uint256S("0x000000000000694c9a363eff06518aa7399f00014ce667b9762f9a4e7a49f485")},
                { 1040000, uint256S("0x000000000000138e2690b06b1ddd8cf158c3a5cf540ee5278debdcdffcf75839")},
                { 1186833, uint256S("0x0000000000000d4840d4de1f7d943542c2aed532bd5d6527274fc0142fa1a410")},
                { 2383550, uint256S("0x0000000000008927ed21a1e3bb87d3e1020646e8cc94354a1f8fc608395e15dc")}
            }
        };

		// 20969961 transactions as of block #2383625 at 2022-07-28 22:02:22 (UTC)
		// previously set at 6709969 txns by time 1577939273 ==>
        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Free chain
            // Stats as of 0x00000000000016ec03d8d93f9751323bcc42137b1b4df67e6a11c4394fd8e5ad window size 43200
            1659045742, // * UNIX timestamp of last known number of transactions
            20969961,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            5.7       // * estimated number of transactions per second after that timestamp
        };

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "RXissueAssetXXXXXXXXXXXXXXXXXhhZGt";
        strReissueAssetBurnAddress = "RXReissueAssetXXXXXXXXXXXXXXVEFAWu";
        strIssueSubAssetBurnAddress = "RXissueSubAssetXXXXXXXXXXXXXWcwhwL";
        strIssueUniqueAssetBurnAddress = "RXissueUniqueAssetXXXXXXXXXXWEAe58";
        strIssueMsgChannelAssetBurnAddress = "RXissueMsgChanneLAssetXXXXXXSjHvAY";
        strIssueQualifierAssetBurnAddress = "RXissueQuaLifierXXXXXXXXXXXXUgEDbC";
        strIssueSubQualifierAssetBurnAddress = "RXissueSubQuaLifierXXXXXXXXXVTzvv5";
        strIssueRestrictedAssetBurnAddress = "RXissueRestrictedXXXXXXXXXXXXzJZ1q";
        strAddNullQualifierTagBurnAddress = "RXaddTagBurnXXXXXXXXXXXXXXXXZQm5ya";

            //Global Burn Address
        strGlobalBurnAddress = "RXBurnXXXXXXXXXXXXXXXXXXXXXXWUo9FV";

        // DGW Activation
        nDGWActivationBlock = 338778;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 435456; // Asset activated block height
        nMessagingActivationBlock = 1092672; // Messaging activated block height
        nRestrictedActivationBlock = 1092672; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 0; // UTC: Wed May 06 2020 18:00:00
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

/**
 * Testnet (v7)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2100000;  //~ 4 yrs at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;

        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1310; // Approx 65% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1310;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1533924000; // UTC: Fri Aug 10 2018 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1577257200; // UTC: Wed Dec 25 2019 07:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1310;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 6;  //Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1570428000; // UTC: Mon Oct 07 2019 06:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1577257200; // UTC: Wed Dec 25 2019 07:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1310;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1586973600; // UTC: Wed Apr 15 2020 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1618509600; // UTC: Thu Apr 15 2021 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1310;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1593453600; // UTC: Mon Jun 29 2020 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1624989600; // UTC: Mon Jun 29 2021 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1597341600; // UTC: Thu Aug 13 2020 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1628877600; // UTC: Fri Aug 13 2021 18:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000168050db560b4");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000006272208605c4df3b54d4d5515759105e7ffcb258e8cd8077924ffef1");


        pchMessageStart[0] = 0x52; // R
        pchMessageStart[1] = 0x56; // V
        pchMessageStart[2] = 0x4E; // N
        pchMessageStart[3] = 0x54; // T
        nDefaultPort = 18770;
        nPruneAfterHeight = 1000;

        uint32_t nGenesisTime = 1537466400;  // Thursday, September 20, 2018 12:00:00 PM GMT-06:00

        // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
//        /////////////////////////////////////////////////////////////////


//        arith_uint256 test;
//        bool fNegative;
//        bool fOverflow;
//        test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
//        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
//
//        int genesisNonce = 0;
//        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
//        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
//        for (int i=0;i<40000000;i++) {
//            genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 2, 5000 * COIN);
//            //genesis.hashPrevBlock = TempHashHolding;
//            // Depending on when the timestamp is on the genesis block. You will need to use GetX16RHash or GetX16RV2Hash. Replace GetHash() with these below
//            consensus.hashGenesisBlock = genesis.GetHash();
//
//            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
//            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
//                BestBlockHash = consensus.hashGenesisBlock;
//                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
//                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
//            }
//
//            TempHashHolding = consensus.hashGenesisBlock;
//
//            if (BestBlockHashArith < test) {
//                genesisNonce = i - 1;
//                break;
//            }
//            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
//        }
//        std::cout << "\n";
//        std::cout << "\n";
//        std::cout << "\n";
//
//        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
//        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
//        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
//
//        std::cout << "\n";
//        std::cout << "\n";
//        int totalHits = 0;
//        double totalTime = 0.0;
//
//        for(int x = 0; x < 16; x++) {
//            totalHits += algoHashHits[x];
//            totalTime += algoHashTotal[x];
//            std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
//        }
//
//        std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
//
//        genesis.hashPrevBlock = TempHashHolding;
//
//        return;

//        /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(nGenesisTime, 15615880, 0x1e00ffff, 2, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetKAWPOWHeaderHash();

        //Test MerkleRoot and GenesisBlock
        assert(consensus.hashGenesisBlock == uint256S("0x000000ecfc5e6324a079542221d00e10362bdc894d56500c414060eea8a3ad5a"));
        assert(genesis.hashMerkleRoot == uint256S("28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.emplace_back("seed-testnet-free.bitactivate.com", false);
        vSeeds.emplace_back("seed-testnet-free.freecoin.com", false);
        vSeeds.emplace_back("seed-testnet-free.freecoin.org", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Free BIP44 cointype in testnet
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                    { 225, uint256S("0x000003465e3e0167322eb8269ce91246bbc211e293bc5fbf6f0a0d12c1ccb363")},
                    {223408, uint256S("0x000000012a0c09dd6456ab19018cc458648dec762b04f4ddf8ef8108eae69db9")},
                    {232980, uint256S("0x000000007b16ae547fce76c3308dbeec2090cde75de74ab5dfcd6f60d13f089b")},
                    {257610, uint256S("0x000000006272208605c4df3b54d4d5515759105e7ffcb258e8cd8077924ffef1")}
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Free chain
            // Stats as of 00000023b66f46d74890287a7b1157dd780c7c5fdda2b561eb96684d2b39d62e window size 43200
            1543633332, // * UNIX timestamp of last known number of transactions
            146666,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.02        // * estimated number of transactions per second after that timestamp
        };

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "n1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
        strReissueAssetBurnAddress = "n1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
        strIssueSubAssetBurnAddress = "n1issueSubAssetXXXXXXXXXXXXXbNiH6v";
        strIssueUniqueAssetBurnAddress = "n1issueUniqueAssetXXXXXXXXXXS4695i";
        strIssueMsgChannelAssetBurnAddress = "n1issueMsgChanneLAssetXXXXXXT2PBdD";
        strIssueQualifierAssetBurnAddress = "n1issueQuaLifierXXXXXXXXXXXXUysLTj";
        strIssueSubQualifierAssetBurnAddress = "n1issueSubQuaLifierXXXXXXXXXYffPLh";
        strIssueRestrictedAssetBurnAddress = "n1issueRestrictedXXXXXXXXXXXXZVT9V";
        strAddNullQualifierTagBurnAddress = "n1addTagBurnXXXXXXXXXXXXXXXXX5oLMH";

        // Global Burn Address
        strGlobalBurnAddress = "n1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 6048; // Asset activated block height
        nMessagingActivationBlock = 10080; // Messaging activated block height
        nRestrictedActivationBlock = 10080; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1585159200; //Wed Mar 25 2020 18:00:00 UTC
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 0; // GMT: Sun Mar 3, 2019 5:00:00 PM
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 999999999999ULL; // UTC: Wed Dec 25 2019 07:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 208;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 288;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 400;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 500;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x43; // C
        pchMessageStart[1] = 0x52; // R
        pchMessageStart[2] = 0x4F; // O
        pchMessageStart[3] = 0x57; // W
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

// This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
//        /////////////////////////////////////////////////////////////////
//
//
//        arith_uint256 test;
//        bool fNegative;
//        bool fOverflow;
//        test.SetCompact(0x207fffff, &fNegative, &fOverflow);
//        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
//
//        int genesisNonce = 0;
//        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
//        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
//        for (int i=0;i<40000000;i++) {
//            genesis = CreateGenesisBlock(1533751200, i, 0x207fffff, 2, 5000 * COIN);
//            //genesis.hashPrevBlock = TempHashHolding;
//            consensus.hashGenesisBlock = genesis.GetHash();
//
//            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
//            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
//                BestBlockHash = consensus.hashGenesisBlock;
//                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
//                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
//            }
//
//            TempHashHolding = consensus.hashGenesisBlock;
//
//            if (BestBlockHashArith < test) {
//                genesisNonce = i - 1;
//                break;
//            }
//            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
//        }
//        std::cout << "\n";
//        std::cout << "\n";
//        std::cout << "\n";
//
//        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
//        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
//        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
//
//        std::cout << "\n";
//        std::cout << "\n";
//        int totalHits = 0;
//        double totalTime = 0.0;
//
//        for(int x = 0; x < 16; x++) {
//            totalHits += algoHashHits[x];
//            totalTime += algoHashTotal[x];
//            std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
//        }
//
//        std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
//
//        genesis.hashPrevBlock = TempHashHolding;
//
//        return;

//        /////////////////////////////////////////////////////////////////


        genesis = CreateGenesisBlock(1524179366, 1, 0x207fffff, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        assert(consensus.hashGenesisBlock == uint256S("0x0b2c703dc93bb63a36c4e33b85be4855ddbca2ac951a7a0a29b8de0408200a3c "));
        assert(genesis.hashMerkleRoot == uint256S("0x28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Free BIP44 cointype in regtest
        nExtCoinType = 1;

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "n1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
        strReissueAssetBurnAddress = "n1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
        strIssueSubAssetBurnAddress = "n1issueSubAssetXXXXXXXXXXXXXbNiH6v";
        strIssueUniqueAssetBurnAddress = "n1issueUniqueAssetXXXXXXXXXXS4695i";
        strIssueMsgChannelAssetBurnAddress = "n1issueMsgChanneLAssetXXXXXXT2PBdD";
        strIssueQualifierAssetBurnAddress = "n1issueQuaLifierXXXXXXXXXXXXUysLTj";
        strIssueSubQualifierAssetBurnAddress = "n1issueSubQuaLifierXXXXXXXXXYffPLh";
        strIssueRestrictedAssetBurnAddress = "n1issueRestrictedXXXXXXXXXXXXZVT9V";
        strAddNullQualifierTagBurnAddress = "n1addTagBurnXXXXXXXXXXXXXXXXX5oLMH";

        // Global Burn Address
        strGlobalBurnAddress = "n1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP";

        // DGW Activation
        nDGWActivationBlock = 200;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 0; // Asset activated block height
        nMessagingActivationBlock = 0; // Messaging activated block height
        nRestrictedActivationBlock = 0; // Restricted activated block height

        // TODO, we need to figure out what to do with this for regtest. This effects the unit tests
        // For now we can use a timestamp very far away
        // If you are looking to test the kawpow hashing function in regtest. You will need to change this number
        nKAAAWWWPOWActivationTime = 3582830167;
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &GetParams() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network, bool fForceBlockNetwork)
{
    SelectBaseParams(network);
    if (fForceBlockNetwork) {
        bNetwork.SetNetwork(network);
    }
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
	globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
	globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
	globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
	globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
	globalChainParams->TurnOffBIP66();
}
