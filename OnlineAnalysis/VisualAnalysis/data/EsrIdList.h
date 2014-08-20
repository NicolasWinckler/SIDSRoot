/* 
 * File:   EsrIdList.h
 * Author: winckler
 *
 * Created on August 4, 2014, 3:25 PM
 */

#ifndef ESRIDLIST_H
#define	ESRIDLIST_H

namespace SIDS
{
    enum DataType 
    {   kUnknownData, 
        kPickup, 
        kRSA1, 
        kRSA2, 
        kRSA3, 
        kTCAP
    };
        
    enum ChannelId
    {
        kUnknownDecay, 
        kECDecay, 
        kBetaDecay, 
        kLossDecay,
        kScattering
    };
}

#endif	/* ESRIDLIST_H */

