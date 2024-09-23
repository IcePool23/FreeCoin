#include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "versionbitsinfo.h"
#include "arith_uint256.h"
#include "pow.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

static CBlock CreateGenesisBlock(
    const char* pszTimestamp, 
    const CScript& genesisOutputScript, 
    uint32_t nTime, 
    uint32_t nNonce, 
    uint32_t nBits, 
    int32_t nVersion, 
    const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.nTime = nTime;
    // Coinbase transaction has a single input whose scriptSig contains the timestamp
    txNew.vin.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) 
                                       << std::vector<unsigned char>(
                                           (const unsigned char*)pszTimestamp, 
                                           (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    // Coinbase transaction has a single output with the genesis reward
    txNew.vout.resize(1);
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    // Create the genesis block
    CBlock genesis;
    genesis.nTime = 1695465600; // 23.09.2024, Unix timestamp
    genesis.nBits = 0x1e0ffff0; // Reset to standard difficulty
    genesis.nNonce = 0; // Set to 0 for mining
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        // Consensus parameters
        consensus.nSubsidyHalvingInterval = 210000; // Number of blocks before reward halves
        consensus.BIP16Exception = uint256(); // No BIP16 exception
        consensus.BIP34Height = 0; // BIP34 enabled from genesis
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; // BIP65 enabled from genesis
        consensus.BIP66Height = 0; // BIP66 enabled from genesis

        // Difficulty adjustment parameters
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Initial difficulty
        consensus.nPowTargetTimespan =  24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing =  1 * 60; // 1 minute between blocks
        consensus.fPowAllowMinDifficultyBlocks = false; // No min difficulty blocks
        consensus.fPowNoRetargeting = false; // Difficulty retargeting is enabled
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        // Deployment of BIP68, BIP112, and BIP113
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // Always active
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL; // No timeout

        // The best chain should have at least this much work
        consensus.nMinimumChainWork = uint256S("0x00");

        // Default assumed blockchain size and chain state size
        consensus.defaultAssumeValid = uint256S("0x00");

        // Message start strings to prevent connecting to other networks
        pchMessageStart[0] = 0x46; // 'F'
        pchMessageStart[1] = 0x52; // 'R'
        pchMessageStart[2] = 0x45; // 'E'
        pchMessageStart[3] = 0x45; // 'E'
        nDefaultPort = 8555; // Default port for your network
        nPruneAfterHeight = 100000;

        // Build the genesis block
        const char* pszTimestamp = "FreeCoin genesis block on September 2023";
        const CScript genesisOutputScript = CScript() << ParseHex("0457fd...") << OP_CHECKSIG; // Replace with your public key
        genesis = CreateGenesisBlock(
            pszTimestamp, 
            genesisOutputScript, 
            1695158400, // nTime: Current UNIX timestamp
            2083236893, // nNonce: To be found
            0x1e0ffff0, // nBits: Initial difficulty
            1,          // nVersion
            50 * COIN   // genesisReward: 50 coins
        );

    consensus.hashGenesisBlock = uint256S("0x0"); // Clear genesis block hash
        genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

        // Uncomment after finding the correct nonce and hash
        // assert(consensus.hashGenesisBlock == uint256S("0x..."));
    // assert(consensus.hashGenesisBlock == uint256S("0x0"));

        // No DNS seeds or fixed seeds
        vSeeds.clear();
        vFixedSeeds.clear();

        // Base58 prefixes
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 35); // Addresses start with 'F'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 95); // Addresses start with 'f'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 163); // Secret keys start with '9' or 'c'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // xpub
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // xprv

        // Bech32 prefixes
        bech32_hrp = "fc";

        // Mining requires peers (since you have no peers, set this to false)
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true; // Allows mining without peers

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, uint256S("0x00")) // No checkpoints yet
        };

        chainTxData = ChainTxData{
            0, // nTime
            0, // nTxCount
            0  // dTxRate
        };
    }
};

static CMainParams mainParams;

const CChainParams& Params() {
    return mainParams;
}
