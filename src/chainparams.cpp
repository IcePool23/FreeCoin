#include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "util/system.h"
#include "util/strencodings.h"

#include <assert.h>

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        // Network identifier
        strNetworkID = "main";

        // Consensus parameters
        consensus.nSubsidyHalvingInterval = 210000; // Halving interval
        consensus.BIP16Exception = uint256S("0x00");
        consensus.BIP34Height = 0; // Activation height for BIP34
        consensus.BIP34Hash = uint256S("0x00");
        consensus.BIP65Height = 0; // Activation height for BIP65
        consensus.BIP66Height = 0; // Activation height for BIP66
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Initial difficulty
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // Two weeks
        consensus.nPowTargetSpacing = 10 * 60; // 10 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;

        // Difficulty adjustment interval
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        // Deployment of BIPs
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // Not used
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 0;   // Not used

        // Block version 2 required
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // Not used
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0;   // Not used

        // The best chain should have at least this much work
        consensus.nMinimumChainWork = uint256S("0x00");

        // Default assume valid block hash
        consensus.defaultAssumeValid = uint256S("0x00");

        // Message start strings to prevent accidental cross-connection
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;
        nDefaultPort = 8333; // Default port

        // Genesis block creation
        genesis = CreateGenesisBlock(1695158400, 2083236893, 0x1d00ffff, 1, 50 * COIN);

        // Calculate the Merkle root and genesis block hash
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000...")); // Replace with actual hash
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4..."));     // Replace with actual Merkle root

        // DNS seed nodes
        vSeeds.emplace_back("seed.freecoin.org");

        // Base58 prefixes
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 0); // Addresses start with '1'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 5); // Script addresses start with '3'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 128); // Private keys start with '5' or 'K' or 'L'

        // Fixed seeds
        vFixedSeeds.clear();

        // Mining requires peers
        fMiningRequiresPeers = true;

        // Default consistency checks
        fDefaultConsistencyChecks = false;

        // Require standard transactions
        fRequireStandard = true;

        // Enable mining support
        fMineBlocksOnDemand = false;

        // Checkpoint data
        checkpointData = {
            {
                {0, uint256S("0x00000000...")} // Replace with actual genesis block hash
            }
        };

        // Chain transaction data
        chainTxData = ChainTxData{
            1695158400, // UNIX timestamp of genesis block
            0,          // Number of transactions
            0.0         // Transactions per second
        };
    }
};
static CMainParams mainParams;
