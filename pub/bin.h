/* SPDX-License-Identifier: LGPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright © 2021 Erez Geva <ErezGeva2@gmail.com> */

/** @file
 * @brief Provide Binary class
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2021 Erez Geva
 *
 */

#ifndef __PTPMGMT_BIN_H
#define __PTPMGMT_BIN_H

#include "name.h"

__PTPMGMT_NAMESPACE_BEGIN

const size_t EUI48 = 6; /**< 48 bits MAC address length */
const size_t EUI64 = 8; /**< 64 bits MAC address length */

/**
 * @brief Hold octets
 * @note functions returning std::string for readable strings
 */
class Binary
{
  private:
    uint8_t *m_buf; /* Used buffer */
    size_t m_alloc; /* Allocation size */
    size_t m_size; /* Binary size */
    void init();
    bool iResize(size_t newAllocSize);
  public:
    Binary();
    ~Binary();
    /**
     * Copy constructor
     * @param[in] rhs object
     */
    Binary(const Binary &rhs);
    /**
     * Constructor from buffer
     * @param[in] buf pointer to Binary octets
     * @param[in] length
     */
    Binary(const void *buf, const size_t length);
    /**
     * Constructor buffer with fixed length
     * @param[in] length of buffer
     * @param[in] set value to set all octests
     */
    Binary(const size_t length, uint8_t set = 0);
    /**
     * Constructor from binary based string
     * @param[in] string string with octets content
     */
    Binary(const std::string &string);
    /**
     * Copy value from another object
     * @param[in] rhs Binary to assign
     * @return reference to itself
     */
    Binary &operator=(const Binary &rhs);
    /**
     * Get Binary length
     * @return Binary length
     */
    size_t length() const { return m_size; }
    /**
     * Get Binary length
     * @return Binary length
     */
    size_t size() const { return m_size; }
    /**
     * Return if Binary is empty
     * @return true if empty
     */
    bool empty() const { return m_size == 0; }
    /**
     * Get pointer to Binary
     * @return pointer to octets
     */
    const uint8_t *get() const { return m_buf; }
    /**
     * Get binary content in a string
     * @return string with binary content
     * @note for scripts, to use unpacking assignment
     *       and split octets into array or list
     */
    std::string getBinString() const { return std::string((char *)m_buf, m_size); }
    /**
     * Set new value
     * @param[in] buf pointer to Binary octets
     * @param[in] length
     * @return reference to itself
     */
    Binary &setBin(const void *buf, const size_t length);
    /**
     * Set new value from binary based string
     * @param[in] string string with octets content
     * @return reference to itself
     * @note for scripts, to use packing assignment
     *       and create a string from array or list of octets
     */
    Binary &setBin(const std::string &string);
    /**
     * Set value in position
     * @param[in] position in Binary octets
     * @param[in] value to set in positions
     * @return reference to itself
     * @note resize the Binary as needed
     */
    Binary &setBin(const size_t position, const uint8_t value);
    /**
     * Get octet in position
     * @param[in] position in Binary octets
     * @return octet value or zero if out of range
     */
    const uint8_t getBin(const size_t position) const;
    /**
     * Get octet in position
     * Caller can change value using @code bin[pos] = new_value @endcode
     * @param[in] position in Binary octets
     * @return octet value or zero if out of range
     */
    uint8_t &operator [](const size_t position);
    /**
     * Resize buffer
     * @param[in] length new length
     */
    Binary &resize(const size_t length);
    /**
     * Copy Binary to target memory block
     * @param[in, out] target memory block
     */
    void copy(uint8_t *target) const { memcpy(target, m_buf, m_size); }
    /**
     * Append single octet using operator +=
     * @param[in] octet to add
     * @return reference to itself
     * @note script can use the append() function.
     */
    Binary &operator += (const uint8_t octet) { return append(octet); }
    /**
     * Append single octet
     * @param[in] octet to add
     * @return reference to itself
     */
    Binary &append(const uint8_t octet);
    /**
     * append octets using operator +=
     * @param[in] rhs Binary to add
     * @return reference to itself
     * @note script can use the append() function.
     */
    Binary &operator += (const Binary &rhs) { return append(rhs); }
    /**
     * append octets
     * @param[in] rhs Binary to add
     * @return reference to itself
     */
    Binary &append(const Binary &rhs);
    /**
     * Convert IP address to string
     * @return Ip address
     * @note Support both IP version 4 and 6
     * @note the user should use with Binary IP address
     *  the class does not verify
     */
    std::string toIp() const;
    /**
     * Convert IP address string to Binary
     * @param[in] string address
     * @return true if valid IP address version 4 or 6
     * @note this function will toss old value
     */
    bool fromIp(const std::string &string);
    /**
     * Convert IP address string to Binary
     * @param[in] string address
     * @param[in] domain of IP to use
     * @return true if valid IP address version 4 or 6
     * @note this function will toss old value
     */
    bool fromIp(const std::string &string, int domain);
    /**
     * Convert ID address to string
     * @return address
     * @note Support MAC addresses
     */
    std::string toId() const { return bufToId(m_buf, m_size); }
    /**
     * Convert Binary ID to string
     * @param[in] id pointer to memory buffer
     * @param[in] length of memory buffer
     * @return address
     * @note Support MAC addresses
     */
    static std::string bufToId(const uint8_t *id, size_t length);
    /**
     * Convert ID address string to Binary
     * @param[in] string address
     * @return true if valid ID address
     * @note Support MAC addresses
     */
    bool fromId(const std::string &string);
    /**
     * Convert MAC address string to Binary
     * @param[in] string address
     * @return true if MAC address
     */
    bool fromMac(const std::string &string) { return fromId(string) && isMacLen(); }
    /**
     * Check if ID length is a valid MAC address
     * @return true if ID length is valid MAC length
     */
    bool isMacLen() const { return m_size == EUI48 || m_size == EUI64; }
    /**
     * Convert ID from EUI48 to EUI64 using 0xfffe padding
     * @return true if ID is EUI64 after conversion
     */
    bool eui48ToEui64();
    /**
     * Convert Hex to Binary with optional separators
     * @param[in] hex string
     * @return true if hex is proper
     */
    bool fromHex(const std::string &hex);
    /**
     * Convert Binary to hex string
     * @return hex string
     */
    std::string toHex() const {
        return bufToHex(m_buf, m_size);
    }
    /**
     * Convert Binary to hex string
     * @param[in] bin pointer to memory buffer
     * @param[in] length of memory buffer
     * @return hex string
     */
    static std::string bufToHex(const uint8_t *bin, size_t length);
    /**
     * Compare binaries
     * @param[in] rhs Binary to compare
     * @return true if binaries are identical
     */
    bool operator==(const Binary &rhs) const { return eq(rhs); }
    /**
     * Compare binaries
     * @param[in] rhs Binary to compare
     * @return true if binaries are identical
     */
    bool eq(const Binary &rhs) const;
    /**
     * Compare binaries
     * @param[in] rhs Binary to compare
     * @return true if binary is smaller
     */
    bool operator<(const Binary &rhs) const { return less(rhs); }
    /**
     * Compare binaries
     * @param[in] rhs Binary to compare
     * @return true if binary is smaller
     */
    bool less(const Binary &rhs) const;
};

__PTPMGMT_NAMESPACE_END

#endif /* __PTPMGMT_BIN_H */
