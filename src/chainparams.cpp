// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 FXTC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 0 << OP_0 << 504365040 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
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
    const char* pszTimestamp = "CNN 29/May/2018 A dramatic rescue in Paris";
    const CScript genesisOutputScript = CScript() << 0x0 << OP_CHECKSIG; // you find it, you get it
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2 * 365 * 1440; // 2 common years
        consensus.nMinimumSubsidy = 0.00100000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
	consensus.nMasternodePaymentsStartBlock = 50;
        consensus.nMasternodePaymentsIncreaseBlock = 50;
        consensus.nMasternodePaymentsIncreasePeriod = 365 * 1440; // 1 common year
        consensus.nMasternodeCollateralMinimum = 1000; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100000; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 24;

        consensus.nBudgetPaymentsStartBlock = 365 * 1440; // 1 common year
        consensus.nBudgetPaymentsCycleBlocks = 10958; // weekly
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 86400; // 1 day

        consensus.nSuperblockStartBlock = 365 * 1440; // 1 common year
        consensus.nSuperblockCycle = 10958; // weekly

        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x00");
        consensus.BIP34Height = 0; // genesis
        consensus.BIP34Hash = uint256S("0x00000ed7c000e25fe4684526c41ad13b4131449f76d7a089321fd2aebd5431a5"); // genesis hash
        consensus.BIP65Height = 0; // genesis
        consensus.BIP66Height = 0; // genesis
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 86400; // one day
        consensus.nPowTargetSpacing = 60; // one minute
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000600015"); // block 5

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000005bdaeb941c7b686f7f6a03e8cf625a8d59162636a3af16fb46b44c9fc4a"); // block 5

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x98;
        pchMessageStart[1] = 0x46;
        pchMessageStart[2] = 0x64;
        pchMessageStart[3] = 0x89;
        nDefaultPort = 9468;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 240;
        m_assumed_chain_state_size = 3;

        genesis = CreateGenesisBlock(1527587168, 6942643, 0x1e0ffff0, 536870912, consensus.nMinimumSubsidy);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000ed7c000e25fe4684526c41ad13b4131449f76d7a089321fd2aebd5431a5"));
        assert(genesis.hashMerkleRoot == uint256S("0x4de1c1f2b3faa48f6ec2a176bd6b83bb94c02e5449f6ea54de34827b98e8fbc4"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("seeds.fixedtradecoin.org"); // FXTC founder seeds
        //vSeeds.emplace_back("seeds.on.another.domain"); // dns seeds template

        // FXTC prefix 'f'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,95);
        // FXTC prefix 'F'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,36);
        // FXTC prefix 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,207);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "fxtc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04491f11731dd8c8a6f6418e69d01cf35a93327971c3c207dde66b58fc2e7e5c9510b5a80a61bc4982fea1533e96f25a1319cd602e14edd1702d652cd3da367bcb";

        founderAddress = "fXdevXfLwHZ4TLya3enVvg8BJXPRKqFBrR";

        checkpointData = {
            {
                {     0, uint256S("0x00000ed7c000e25fe4684526c41ad13b4131449f76d7a089321fd2aebd5431a5")},
                {     1, uint256S("0x00000143a05a6935178b8c697b53d7e38de5edb493381fd8f719dc4ac1c7e924")},
                {     2, uint256S("0x00000498c7854ef659323e985d7fb565a133286511b057431d9fbab8d18df7a0")},
                {     3, uint256S("0x00000d08644aae5cd3ba625f794f665cd374bdc64cabd4693e1ae4444357c4f0")},
                {     4, uint256S("0x00000554ec651b44f6c2cb68982bcf492bec887499ad2bca2ed8b9de39039127")},
                {     5, uint256S("0x000005bdaeb941c7b686f7f6a03e8cf625a8d59162636a3af16fb46b44c9fc4a")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 000005bdaeb941c7b686f7f6a03e8cf625a8d59162636a3af16fb46b44c9fc4a (height 5).
            1527896258, // * UNIX timestamp of last known number of transactions
            5,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0           // * estimated number of transactions per second after that timestamp
        };

        // FXTC TODO: we need to resolve fee calculation bug and disable fallback
        ///* disable fallback fee on mainnet */
        //m_fallback_fee_enabled = false;
        m_fallback_fee_enabled = true;
        //
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2 * 365 * 1440; // 2 common years
        consensus.nMinimumSubsidy = 0.00100000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 10; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 10;
        consensus.nMasternodePaymentsIncreasePeriod = 25;
        consensus.nMasternodeCollateralMinimum = 10; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 1000; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 4200;
        consensus.nSuperblockCycle = 24;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x00");
        consensus.BIP34Height = 0; // genesis
        consensus.BIP34Hash = uint256S("0x00000706ff7091032d187cf1c2515a5d7891b6797f414526ef8a277491117d08"); // genesis hash
        consensus.BIP65Height = 0; // genesis
        consensus.BIP66Height = 0; // genesis
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // one day
        consensus.nPowTargetSpacing = 1 * 60; // one minute
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000600015"); // block 5

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000080d1e920f5ad1ca8b48b669428eb285beeeea05a2ef06c8a7b37664ea46"); // block 5

        pchMessageStart[0] = 0x89;
        pchMessageStart[1] = 0x64;
        pchMessageStart[2] = 0x46;
        pchMessageStart[3] = 0x98;
        nDefaultPort = 19468;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 30;
        m_assumed_chain_state_size = 2;

        genesis = CreateGenesisBlock(1527587168, 17129207, 0x1e0ffff0, 536870912, consensus.nMinimumSubsidy);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000706ff7091032d187cf1c2515a5d7891b6797f414526ef8a277491117d08"));
        assert(genesis.hashMerkleRoot == uint256S("0x4de1c1f2b3faa48f6ec2a176bd6b83bb94c02e5449f6ea54de34827b98e8fbc4"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seeds.fixedtradecoin.org");

        // FXTC prefix 't'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,128);
        // FXTC prefix 'T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,65);
        // FXTC prefix 'Y'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,214);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "testnet1fxtc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04c7cfbb6d4e912aa67acd1ccb997425211376f480d45c1850d70ef3dbfbc6ed663429dc73b67fa8b8c8cb49dac68903929879090a34937bc2b65db6003cbbfed4";

        founderAddress = "tXdevXRj7MeN6GF68NVwqsHonLYeS8GPzV";

        checkpointData = {
            {
                {0, uint256S("0x00000706ff7091032d187cf1c2515a5d7891b6797f414526ef8a277491117d08")},
                {1, uint256S("0x00000d6d8b4a171281ec0c6feb82340da83f33b55477d782d3c1b35384d77833")},
                {2, uint256S("0x00000052e310708beb6f16770ae6781af243095abbc6930a39dd20625637873b")},
                {3, uint256S("0x00000fa4eeb40622f75e9bbb6fe34dd076ddd084ee0255b69b91d1ab7f07431e")},
                {4, uint256S("0x0000069f22511d97b2337099c18d218956937d9737bdd8e7fc1c381dd35271e9")},
                {5, uint256S("0x0000080d1e920f5ad1ca8b48b669428eb285beeeea05a2ef06c8a7b37664ea46")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 00000706ff7091032d187cf1c2515a5d7891b6797f414526ef8a277491117d08 (height 0)
            1527900028,
            5,
            0
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMinimumSubsidy = 10000.00000000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
	consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nMasternodeCollateralMinimum = 1; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 0; // BIP34 has activated on regtest (blocks v1 are rejected in tests)
        consensus.BIP34Hash = uint256S("0x000002439e312808612fecdf76cfcbb2d702844856ad91a67830283a33e5f8e8"); //genesis hash
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // one day
        consensus.nPowTargetSpacing = 1; // one second
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x99;
        pchMessageStart[1] = 0x44;
        pchMessageStart[2] = 0x66;
        pchMessageStart[3] = 0x88;
        nDefaultPort = 29468;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateVersionBitsParametersFromArgs(args);

        genesis = CreateGenesisBlock(1523632983, 29889593, 0x1e0ffff0, 536870912, consensus.nMinimumSubsidy);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000002439e312808612fecdf76cfcbb2d702844856ad91a67830283a33e5f8e8"));
        assert(genesis.hashMerkleRoot == uint256S("0xd72fc21c2e3315e4ea511b091eabd1e10fcb0518d56e692f51a5b39f5675091c"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        founderAddress = "cXdevX1jzA1Bd6hZVTcTEPh5NH8iDx1s4Z";

        checkpointData = {
            {
                {0, uint256S("0x000002439e312808612fecdf76cfcbb2d702844856ad91a67830283a33e5f8e8")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        // Bitcoin defaults
        // FXTC prefix 'c'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        // FXTC prefix 'C'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,28);
        // FXTC prefix 'Z'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,221);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "regtest1fxtc";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateVersionBitsParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateVersionBitsParametersFromArgs(const ArgsManager& args)
{
    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
