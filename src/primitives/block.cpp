// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2018 FXTC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <util/strencodings.h>
#include <crypto/common.h>

#include <crypto/Lyra2Z.h>
#include <crypto/nist5.h>
#include <crypto/scrypt.h>
#include <crypto/x11.h>
#include <crypto/x16r.h>

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{
    uint256 powHash = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    switch (nVersion & ALGO_VERSION_MASK)
    {
        case ALGO_SHA256D: powHash = GetHash(); break;
        case ALGO_SCRYPT:  scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(powHash)); break;
        case ALGO_NIST5:   powHash = NIST5(BEGIN(nVersion), END(nNonce)); break;
        case ALGO_LYRA2Z:  lyra2z_hash(BEGIN(nVersion), BEGIN(powHash)); break;
        case ALGO_X11:     powHash = HashX11(BEGIN(nVersion), END(nNonce)); break;
        case ALGO_X16R:    powHash = HashX16R(BEGIN(nVersion), END(nNonce), hashPrevBlock); break;
        default:           break; // FXTC TODO: we should not be here
    }

    return powHash;
}

unsigned int CBlockHeader::GetAlgoEfficiency(int nBlockHeight) const
{
    switch (nVersion & ALGO_VERSION_MASK)
    {
        case ALGO_SHA256D: return       1;
        case ALGO_SCRYPT:  return   13747;
        case ALGO_NIST5:   return    2631;
        case ALGO_LYRA2Z:  return 2014035;
        case ALGO_X11:     return     477;
        case ALGO_X16R:    return  263100;
        default:           return       1; // FXTC TODO: we should not be here
    }

    return 1; // FXTC TODO: we should not be here
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
