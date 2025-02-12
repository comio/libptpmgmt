/* SPDX-License-Identifier: LGPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright © 2021 Erez Geva <ErezGeva2@gmail.com> */

/** @file
 * @brief Parse signaling TLVs
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2021 Erez Geva
 *
 */

#include "comp.h"

__PTPMGMT_NAMESPACE_BEGIN

#define A(n) bool MsgProc::n##_f(n##_t &d)
A(ORGANIZATION_EXTENSION)
{
    return proc(d.organizationId, 3) ||
        proc(d.organizationSubType, 3) ||
        proc(d.dataField, m_left);
}
A(PATH_TRACE)
{
    return vector_o(d.pathSequence);
}
A(ALTERNATE_TIME_OFFSET_INDICATOR)
{
    return proc(d.keyField) || proc(d.currentOffset) || proc(d.jumpSeconds) ||
        proc48(d.timeOfNextJump) || proc(d.displayName);
}
A(ENHANCED_ACCURACY_METRICS)
{
    return proc(d.bcHopCount) || proc(d.tcHopCount) || proc(reserved) ||
        proc(reserved) || proc(d.maxGmInaccuracy) || proc(d.varGmInaccuracy) ||
        proc(d.maxTransientInaccuracy) || proc(d.varTransientInaccuracy) ||
        proc(d.maxDynamicInaccuracy) || proc(d.varDynamicInaccuracy) ||
        proc(d.maxStaticInstanceInaccuracy) ||
        proc(d.varStaticInstanceInaccuracy) ||
        proc(d.maxStaticMediumInaccuracy) || proc(d.varStaticMediumInaccuracy);
}
A(L1_SYNC)
{
    return procFlags(d.flags1, d.flagsMask1) || procFlags(d.flags2, d.flagsMask2);
}
A(PORT_COMMUNICATION_AVAILABILITY)
{
    return procFlags(d.syncMessageAvailability, d.flagsMask1) ||
        procFlags(d.delayRespMessageAvailability, d.flagsMask2);
}
A(PROTOCOL_ADDRESS)
{
    return proc(d.portProtocolAddress);
}
A(SLAVE_RX_SYNC_TIMING_DATA)
{
    if(proc(d.syncSourcePortIdentity))
        return true;
    return vector_o(d.list);
}
A(SLAVE_RX_SYNC_COMPUTED_DATA)
{
    if(proc(d.sourcePortIdentity) || procFlags(d.computedFlags, d.flagsMask) ||
        proc(reserved))
        return true;
    return vector_o(d.list);
}
A(SLAVE_TX_EVENT_TIMESTAMPS)
{
    if(proc(d.sourcePortIdentity) || proc(d.eventMessageType) || proc(reserved))
        return true;
    return vector_o(d.list);
}
A(CUMULATIVE_RATE_RATIO)
{
    return proc(d.scaledCumulativeRateRatio);
}
A(SMPTE_ORGANIZATION_EXTENSION)
{
    bool ret = proc(d.organizationId, 3);
    if(ret)
        return true;
    if(memcmp(d.organizationId, "\x68\x97\xe8", 3) != 0) {
        m_err = MNG_PARSE_ERROR_INVALID_ID;
        return true;
    }
    return proc(d.organizationSubType, 3) ||
        proc(d.defaultSystemFrameRate_numerator) ||
        proc(d.defaultSystemFrameRate_denominator) ||
        proc(d.masterLockingStatus) || proc(d.timeAddressFlags) ||
        proc(d.currentLocalOffset) || proc(d.jumpSeconds) ||
        proc48(d.timeOfNextJump) || proc48(d.timeOfNextJam) ||
        proc48(d.timeOfPreviousJam) || proc(d.previousJamLocalOffset) ||
        proc(d.daylightSaving) || proc(d.leapSecondJump);
}
bool MsgProc::proc(SLAVE_RX_SYNC_TIMING_DATA_rec_t &d)
{
    return proc(d.sequenceId) || proc(d.syncOriginTimestamp) ||
        proc(d.totalCorrectionField) || proc(d.scaledCumulativeRateOffset) ||
        proc(d.syncEventIngressTimestamp);
}
bool MsgProc::proc(SLAVE_RX_SYNC_COMPUTED_DATA_rec_t &d)
{
    return proc(d.sequenceId) || proc(d.offsetFromMaster) ||
        proc(d.meanPathDelay) || proc(d.scaledNeighborRateRatio);
}
bool MsgProc::proc(SLAVE_TX_EVENT_TIMESTAMPS_rec_t &d)
{
    return proc(d.sequenceId) || proc(d.eventEgressTimestamp);
}
A(SLAVE_DELAY_TIMING_DATA_NP)
{
    if(proc(d.sourcePortIdentity))
        return true;
    return vector_o(d.list);
}
bool MsgProc::proc(SLAVE_DELAY_TIMING_DATA_NP_rec_t &d)
{
    return proc(d.sequenceId) || proc(d.delayOriginTimestamp) ||
        proc(d.totalCorrectionField) || proc(d.delayResponseTimestamp);
}

__PTPMGMT_NAMESPACE_END
