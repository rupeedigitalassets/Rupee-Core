// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
		(0, uint256("0x00007b078f0fbad3b7b341ed9828260ecfd9cc9f4ae971b45ecf697beac63fb0"))
		(1, uint256("0xde8c0cefa4a219c93189810036ef977b7c745272498388b3ac4e6df6d2c4d35d"))
		(2, uint256("0x7eee5b685b960ac83eb7e5978b08ea014d85057387f91a861d39a3ff076f5c57"))
		(489, uint256("0x714f3b3df9c143792adaa57103eed76ea9b9b3e464bb21f07a3472e404278799"))
		(1129, uint256("0xd7a70674839de12456fe0ec1b3d05405ccd2f504980a41a48d6b26061ac891c1"))
		(1872, uint256("0x32245ed3c56526f2e6a04e40329cede550f9b55ba4a23eca0f2e558b14d681dd"))
		(2018, uint256("0x58f0c77f6f097ee8ee5a32bb1596aeaadfd9b15ac1fd90dee36a4d8a85885472"))
		(3369, uint256("0x65449c7a9d75bd6137908bcbbf01ba031f5187a057a09f4b0e5b5202273c3078"))
		(4206, uint256("0x69ebe394a16818610c49152a370389d702b02f830f58d7d8f6d8afe547041091"))
		(4341, uint256("0xf1a91c522d80a1c7cd9eac4420a9da2833211c5631286cbf63d26760c371e4ce"))
		(5191, uint256("0x10f0d7a558ae36971a5bb2dc08071b3c81782479e311884c4944e26ac05a2914"))
		(5904, uint256("0x800f9af0701c0e88247f60f63894af42a5caa167786656a2e1c5c3de0f2c8a76"))
		(6951, uint256("0x57ef6dfaf4a1e4c376d95482079df7c0075dafcb40b4d74cfcbcea4eee2b7c7b"))	    
	    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
