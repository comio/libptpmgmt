/* SPDX-License-Identifier: LGPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright © 2021 Erez Geva <ErezGeva2@gmail.com> */

/** @file
 * @brief PTP signaling TLV structures
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2021 Erez Geva
 *
 */

#ifndef __PTPMGMT_SIG_H
#define __PTPMGMT_SIG_H

#include <vector>
#include <memory>
#include "types.h"

__PTPMGMT_NAMESPACE_BEGIN

#ifndef SWIG
/** MANAGEMENT TLV
 * @note: used for management TLV in a signaling message
 *  management message do not use this structure!
 */
struct MANAGEMENT_t : public BaseSigTlv {
    mng_vals_e managementId; /**< Management TLV id */
    std::unique_ptr<BaseMngTlv> tlvData; /**< Management TLV data */
};
#endif /* SWIG */
/** MANAGEMENT_ERROR_STATUS TLV
 * @note: used for error management TLV in a signaling message
 *  management message do not use this structure!
 */
struct MANAGEMENT_ERROR_STATUS_t : public BaseSigTlv {
    mng_vals_e managementId; /**< Management TLV id */
    managementErrorId_e managementErrorId; /**< Error code */
    PTPText_t displayData; /**< Error message */
};
/** Organization extension TLV
 * For
 * @li ORGANIZATION_EXTENSION
 * @li ORGANIZATION_EXTENSION_PROPAGATE
 * @li ORGANIZATION_EXTENSION_DO_NOT_PROPAGATE
 */
struct ORGANIZATION_EXTENSION_t : public BaseSigTlv {
    Octet_t organizationId[3]; /**< IEEE organization ID */
    Octet_t organizationSubType[3]; /**< sub-organization ID */
    Binary dataField; /**< organization own data */
};
/** PATH_TRACE TLV */
struct PATH_TRACE_t : public BaseSigTlv {
    std::vector<ClockIdentity_t> pathSequence; /**< clock id per path */
};
/** ALTERNATE_TIME_OFFSET_INDICATOR TLV */
struct ALTERNATE_TIME_OFFSET_INDICATOR_t : public BaseSigTlv {
    UInteger8_t keyField; /**< the index to the alternate timescale offsets */
    Integer32_t currentOffset; /**< Current offset */
    Integer32_t jumpSeconds; /**< Jump seconds */
    UInteger48_t timeOfNextJump; /**< Time of next jump */
    PTPText_t displayName; /**< description of the alternate timescale */
};
/** ENHANCED_ACCURACY_METRICS TLV */
struct ENHANCED_ACCURACY_METRICS_t : public BaseSigTlv {
    UInteger8_t bcHopCount; /**< number of Boundary Clocks in this TLV */
    UInteger8_t tcHopCount; /**< number of Transparent Clocks in this TLV */
    /** Max metric of Grandmaster inaccuracy in 2^16 nanoseconds units */
    TimeInterval_t maxGmInaccuracy;
    /** Var metrics of Grandmaster inaccuracy in seconds' square */
    Float64_t varGmInaccuracy;
    /** Max metric of Transient inaccuracy in 2^16 nanoseconds units */
    TimeInterval_t maxTransientInaccuracy;
    /** Var metrics of Transient inaccuracy in seconds' square */
    Float64_t varTransientInaccuracy;
    /** Max metric of Dynamic inaccuracy in 2^16 nanoseconds units */
    TimeInterval_t maxDynamicInaccuracy;
    /** Var metrics of Dynamic inaccuracy in seconds' square */
    Float64_t varDynamicInaccuracy;
    /** Max metric of Static inaccuracy in 2^16 nanoseconds units */
    TimeInterval_t maxStaticInstanceInaccuracy;
    /** Var metrics of Static inaccuracy in seconds' square */
    Float64_t varStaticInstanceInaccuracy;
    /** Max metric of Static Medium inaccuracy in 2^16 nanoseconds units */
    TimeInterval_t maxStaticMediumInaccuracy;
    /** Var metrics of Static Medium inaccuracy in seconds' square */
    Float64_t varStaticMediumInaccuracy;
};
/** L1_SYNC TLV */
struct L1_SYNC_t : public BaseSigTlv {
    /**
     * Bit fields flag
     * @li bit 0: TCR   L1SyncBasicPortDS.txCoherentIsRequired
     * @li bit 1: RCR   L1SyncBasicPortDS.rxCoherentIsRequired
     * @li bit 2: CR    L1SyncBasicPortDS.congruentIsRequired
     * @li bit 3: OPE   L1SyncBasicPortDS.optParamsEnabled
     */
    uint8_t flags1;
    const uint8_t flagsMask1 = 0xf; /**< mask for flags1 */
    /**
     * Bit fields flag
     * @li bit 0: ITC   L1SyncBasicPortDS.isTxCoherent
     * @li bit 1: IRC   L1SyncBasicPortDS.isRxCoherent
     * @li bit 2: IC    L1SyncBasicPortDS.isCongruent
     */
    uint8_t flags2;
    const uint8_t flagsMask2 = 0x7; /**< Mask for flags2 */
};
/** PORT_COMMUNICATION_AVAILABILITY */
struct PORT_COMMUNICATION_AVAILABILITY_t : public BaseSigTlv {
    /**
     * Bit fields syncMessageAvailability
     * @li bit 0 syncCapabilities.multicastCapable
     * @li bit 1 syncCapabilities.unicastCapable
     * @li bit 2 syncCapabilities.unicastNegotiationCapable @&@&
     *           unicastNegotiationPortDS.enable
     * @li bit 3 syncCapabilities.unicastNegotiationCapable.
     * flags from communicationCapabilitiesPortDS.syncCapabilities
     */
    uint8_t syncMessageAvailability;
    const uint8_t flagsMask1 = 0xf; /**< Mask for syncMessageAvailability */
    /**
     * Bit fields delayRespMessageAvailability
     * @li bit 0 delayRespCapabilities.multicastCapable
     * @li bit 1 delayRespCapabilities.unicastCapable
     * @li bit 2 delayRespCapabilities.unicastNegotiationCapable @&@&
     *           unicastNegotiationPortDS.enable
     * @li bit 3 delayRespCapabilities.unicastNegotiationCapable.
     * flags from communicationCapabilitiesPortDS.delayRespCapabilities.
     */
    uint8_t delayRespMessageAvailability;
    const uint8_t flagsMask2 = 0xf; /**< Mask for delayRespMessageAvailability */
};
/** PROTOCOL_ADDRESS TLV */
struct PROTOCOL_ADDRESS_t : public BaseSigTlv {
    PortAddress_t portProtocolAddress; /**< protocol address */
};
/** SLAVE_RX_SYNC_TIMING_DATA TLV record */
struct SLAVE_RX_SYNC_TIMING_DATA_rec_t {
    UInteger16_t sequenceId; /**< Sequence of the sync message */
    Timestamp_t syncOriginTimestamp; /**< sync Event Egress Timestamp value */
    /** aggregate value of the correctionField */
    TimeInterval_t totalCorrectionField;
    /** scaled Cumulative Rate Offset value */
    Integer32_t scaledCumulativeRateOffset;
    /** sync Event Ingress Timestamp value */
    Timestamp_t syncEventIngressTimestamp;
    /**
     * Get object size
     * @return object size
     */
    static size_t size() {
        return sizeof sequenceId + 2 * Timestamp_t::size() +
            TimeInterval_t::size() + sizeof scaledCumulativeRateOffset;
    }
};
/** SLAVE_RX_SYNC_TIMING_DATA TLV */
struct SLAVE_RX_SYNC_TIMING_DATA_t : public BaseSigTlv {

    /** Port identity of the received sync message. */
    PortIdentity_t syncSourcePortIdentity;
    /** Records of received synchronization messages */
    std::vector<SLAVE_RX_SYNC_TIMING_DATA_rec_t> list;
};
/** SLAVE_RX_SYNC_COMPUTED_DATA TLV record */
struct SLAVE_RX_SYNC_COMPUTED_DATA_rec_t {
    UInteger16_t sequenceId; /**< Sequence of the sync message */
    /** offsetFromMaster after calculation based on Sync message */
    TimeInterval_t offsetFromMaster;
    /** meanPathDelay after calculation based on Sync message */
    TimeInterval_t meanPathDelay;
    /** scaledNeighborRateRatio after calculation based on Sync message */
    Integer32_t scaledNeighborRateRatio;
    /**
     * Get object size
     * @return object size
     */
    static size_t size() {
        return sizeof sequenceId + 2 * TimeInterval_t::size() +
            sizeof scaledNeighborRateRatio;
    }
};
/** SLAVE_RX_SYNC_COMPUTED_DATA TLV */
struct SLAVE_RX_SYNC_COMPUTED_DATA_t : public BaseSigTlv {
    /** Port identity of the received sync message. */
    PortIdentity_t sourcePortIdentity;
    /**
     * Bit fields computedFlags
     * @li bit 0: scaledNeighborRateRatioValid
     * @li bit 1: meanPathDelayValid
     * @li bit 2: offsetFromMasterValid
    */
    uint8_t computedFlags;
    const uint8_t flagsMask = 0x7; /**< Mask for computedFlags */
    /** records of received sync messages */
    std::vector<SLAVE_RX_SYNC_COMPUTED_DATA_rec_t> list;
};

/** SLAVE_TX_EVENT_TIMESTAMPS TLV record */
struct SLAVE_TX_EVENT_TIMESTAMPS_rec_t {
    UInteger16_t sequenceId; /**< Sequence of the event message */
    /** egress Timestamp acquired for the event message */
    Timestamp_t eventEgressTimestamp;
    /**
     * Get object size
     * @return object size
     */
    static size_t size() { return sizeof sequenceId + Timestamp_t::size(); }
};
/** SLAVE_TX_EVENT_TIMESTAMPS TLV */
struct SLAVE_TX_EVENT_TIMESTAMPS_t : public BaseSigTlv {
    /** Port identity of the transmitted event message. */
    PortIdentity_t sourcePortIdentity;
    /**
     * Event massage type
     * @li Nibble:  event massage type
     * @li Nibble:  reserved
     */
    msgType_e eventMessageType;
    /** records of transmitted event messages */
    std::vector<SLAVE_TX_EVENT_TIMESTAMPS_rec_t> list;
};
/** CUMULATIVE_RATE_RATIO TLV */
struct CUMULATIVE_RATE_RATIO_t : public BaseSigTlv {
    /** (@<cumulativeRateRatio@> - 1) * 2^41 */
    Integer32_t scaledCumulativeRateRatio;
};
/**
 * SMPTE (the Society of Motion Picture and Television Engineers)
 * Management Message with Synchronization Metadata (SM)
 * Using Organization Extension TLV
 */
struct SMPTE_ORGANIZATION_EXTENSION_t : public BaseMngTlv {
    Octet_t organizationId[3]; /**< always SMPTE OUI 68 97 E8 */
    Octet_t organizationSubType[3]; /**< SM TLV version */
    /** Default video frame rate - the numerator for a division */
    UInteger32_t defaultSystemFrameRate_numerator;
    /** Default video frame rate - the denominator for a division */
    UInteger32_t defaultSystemFrameRate_denominator;
    /** Complementary information to clockClass */
    SMPTEmasterLockingStatus_e masterLockingStatus;
    uint8_t timeAddressFlags;  /**< SMPTE ST 12-1 flags */
    /** Offset in seconds of Local Time from grandmaster PTP time */
    Integer32_t currentLocalOffset;
    /** The size of the next discontinuity, in seconds, of Local Time */
    Integer32_t jumpSeconds;
    /**
     * The value of the seconds portion of the grandmaster PTP time
     * at the time that the next discontinuity of the currentLocalOffset will occur.
     */
    UInteger48_t timeOfNextJump;
    /**
     * The value of the seconds portion of the PTP time corresponding
     * to the next scheduled occurrence of the Daily Jam.
     */
    UInteger48_t timeOfNextJam;
    /**
     * The value of the seconds portion of the PTP time corresponding
     * to the previous occurrence of the Daily Jam.
     */
    UInteger48_t timeOfPreviousJam;
    /**
     * The value of currentLocalOffset at the time of the previous Daily Jam event.
     */
    Integer32_t previousJamLocalOffset;
    uint8_t daylightSaving; /**< Daylight saving flags */
    /**
     * The reason for the forthcoming discontinuity of currentLocalOffset
     * indicated by timeOfNextJump.
     */
    uint8_t leapSecondJump;
    /**
     * Get object size
     * @return object size
     */
    static size_t size() {
        return 3 * 2 + sizeof(UInteger32_t) * 2 + sizeof(uint8_t) * 3 +
            sizeof(Integer32_t) * 3 + sizeof_UInteger48_t * 3
            + sizeof(SMPTEmasterLockingStatus_e);
    }
};
/** SLAVE_DELAY_TIMING_DATA_NP TLV record
 * @note linuxptp implementation specific
 */
struct SLAVE_DELAY_TIMING_DATA_NP_rec_t {
    UInteger16_t sequenceId; /**< Sequence of the message */
    /** delay Origin Timestamp value */
    Timestamp_t delayOriginTimestamp;
    /** aggregate value of the correctionField */
    TimeInterval_t totalCorrectionField;
    /** delay Response Timestamp */
    Timestamp_t delayResponseTimestamp;
    /**
     * Get object size
     * @return object size
     */
    static size_t size() {
        return sizeof sequenceId + TimeInterval_t::size() +
            2 * Timestamp_t::size();
    }
};
/** SLAVE_DELAY_TIMING_DATA_NP TLV
 * @note linuxptp implementation specific
 */
struct SLAVE_DELAY_TIMING_DATA_NP_t : public BaseSigTlv {
    /** Port identity of the message. */
    PortIdentity_t sourcePortIdentity;
    /** records of messages */
    std::vector<SLAVE_DELAY_TIMING_DATA_NP_rec_t> list;
};

__PTPMGMT_NAMESPACE_END

#endif /* __PTPMGMT_SIG_H */
