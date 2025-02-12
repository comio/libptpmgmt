/* SPDX-License-Identifier: LGPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright © 2021 Erez Geva <ErezGeva2@gmail.com> */

/** @file
 * @brief Provide sockets to communicate with a PTP daemon
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2021 Erez Geva
 *
 * @details
 *  provide 4 socket types:
 *  1. UDP using IP version 4
 *  2. UDP using IP version 6
 *  3. Raw Ethernet
 *  4. linuxptp Unix domain socket.
 *
 */

#ifndef __PTPMGMT_SOCK_H
#define __PTPMGMT_SOCK_H

#include <netinet/in.h>
#include <sys/un.h>
#include <linux/if_packet.h>
#include "cfg.h"
#include "ptp.h"
#include "buf.h"

__PTPMGMT_NAMESPACE_BEGIN

/**
 * @brief Base class for all sockets
 * @details
 *  provide functions that are supported by all socket's classes
 */
class SockBase
{
  protected:
    /**< @cond internal */
    int m_fd;
    bool m_isInit;
    SockBase() : m_fd(-1), m_isInit(false) {}
    bool sendReply(ssize_t cnt, size_t len) const;
    virtual bool sendBase(const void *msg, size_t len) = 0;
    virtual ssize_t rcvBase(void *buf, size_t bufSize, bool block) = 0;
    virtual bool initBase() = 0;
    virtual void closeChild() {}
    void closeBase();

  public:
    virtual ~SockBase() { closeBase(); }
    /**< @endcond */

    /**
     * close socket and release its resources
     */
    void close() { closeBase(); }
    /**
     * Allocate the socket and initialize it with current parameters
     * @return true if socket creation success
     */
    bool init() { return initBase(); }
    /**
     * Send the message using the socket
     * @param[in] msg pointer to message memory buffer
     * @param[in] len message length
     * @return true if message is sent
     * @note true does @b NOT guarantee the frame was successfully
     *  arrives its target. Only the network layer sends it.
     */
    bool send(const void *msg, size_t len)
    { return sendBase(msg, len); }
    /**
     * Send the message using the socket
     * @param[in] buf object with message memory buffer
     * @param[in] len message length
     * @return true if message is sent
     * @note true does @b NOT guarantee the frame was successfully
     *  arrives its target. Only the network layer sends it.
     */
    bool send(Buf &buf, size_t len)
    { return sendBase(buf.get(), len); }
    /**
     * Send the message using the socket
     * @param[in] buf object with message memory buffer
     * @param[in] len message length
     * @return true if message is sent
     * @note true does @b NOT guarantee the frame was successfully
     *  arrives its target. Only the network layer sends it.
     * @note identical to send. Some scripts fail to match proper function
     */
    bool sendBuf(Buf &buf, size_t len)
    { return sendBase(buf.get(), len); }
    /**
     * Receive a message using the socket
     * @param[in, out] buf pointer to a memory buffer
     * @param[in] bufSize memory buffer size
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     */
    ssize_t rcv(void *buf, size_t bufSize, bool block = false)
    { return rcvBase(buf, bufSize, block); }
    /**
     * Receive a message using the socket
     * @param[in, out] buf object with message memory buffer
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     */
    ssize_t rcv(Buf &buf, bool block = false)
    { return rcvBase(buf.get(), buf.size(), block); }
    /**
     * Receive a message using the socket
     * @param[in, out] buf object with message memory buffer
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     * @note identical to rcv. Some scripts fail to match proper function
     */
    ssize_t rcvBuf(Buf &buf, bool block = false)
    { return rcvBase(buf.get(), buf.size(), block); }
    /**
     * Get socket file description
     * @return socket file description
     * @note Can be used to poll, send or receive from socket.
     *  The user is advice to use properly. Do @b NOT free the socket.
     *  If you want to close the socket use the close function @b ONLY.
     */
    int getFd() const { return m_fd; }
    /**
     * Get socket file description
     * @return socket file description
     * @note Can be used to poll, send or receive from socket.
     *  The user is advice to use properly. Do @b NOT free the socket.
     *  If you want to close the socket use the close function @b ONLY.
     */
    int fileno() const { return m_fd; }
    #ifdef __PTPMGMT_SWIG_THREAD_START
    __PTPMGMT_SWIG_THREAD_START;
    #endif
    /**
     * Single socket polling
     * @param[in] timeout_ms timeout in milliseconds,
     *  until receive a packet. use 0 for blocking.
     * @return true if a packet is ready for receive
     * @note If user need multiple socket,
     *  then fetch the file description with fileno()
     *  And implement it, or merge it into an existing polling
     * @note Python: when building with 'PY_USE_S_THRD'
     *  using Python 'Global Interpreter Lock'.
     *  Which use mutex on all library functions.
     *  So this function can block other threads.
     *  In this case, user may prefer native Python select module.
     */
    bool poll(uint64_t timeout_ms = 0) const;
    /**
     * Single socket polling and update timeout
     * @param[in, out] timeout_ms timeout in milliseconds
     *  until receive a packet. use 0 for blocking.
     * @return true if a packet is ready for receive
     * @note The function will reduce the wait time from timeout
     *  when packet arrives. The user is advice to ensure the timeout
     *  is positive, as @b zero cause to block until receive a packet.
     * @note If user need multiple socket,
     *  then fetch the file description with fileno()
     *  And implement it, or merge it into an existing polling
     * @note Python: when building with 'PY_USE_S_THRD'
     *  using Python 'Global Interpreter Lock'.
     *  Which use mutex on all library functions.
     *  So this function can block other threads.
     *  In this case, user may prefer native Python select module.
     */
    bool tpoll(uint64_t &timeout_ms) const; /* poll with timeout update */
    #ifdef __PTPMGMT_SWIG_THREAD_END
    __PTPMGMT_SWIG_THREAD_END;
    #endif
};

/**
 * @brief Unix socket
 * @details
 *  provide Unix socket that can be used to communicate with
 *  linuxptp daemon, ptp4l.
 */
class SockUnix : public SockBase
{
  private:
    std::string m_me, m_peer, m_homeDir, m_lastFrom;
    sockaddr_un m_peerAddr;
    bool setPeerInternal(const std::string &str);
    bool sendAny(const void *msg, size_t len, const sockaddr_un &addr) const;
    static void setUnixAddr(sockaddr_un &addr, const std::string &str);
  protected:
    /**< @cond internal */
    bool sendBase(const void *msg, size_t len) override final;
    ssize_t rcvBase(void *buf, size_t bufSize, bool block) override final;
    bool initBase() override final;
    void closeChild() override final;
    /**< @endcond */

  public:
    SockUnix() { setUnixAddr(m_peerAddr, m_peer); }
    /**
     * Get peer address
     * @return string object with peer address
     */
    const std::string &getPeerAddress() const { return m_peer; }
    /**
     * Get peer address
     * @return string with peer address
     */
    const char *getPeerAddress_c() const { return m_peer.c_str(); }
    /**
     * Set peer address
     * @param[in] string object with peer address
     * @return true if peer address is updated
     */
    bool setPeerAddress(const std::string &string) {
        return setPeerInternal(string);
    }
    /**
     * Set peer address using configuration file
     * @param[in] cfg reference to configuration file object
     * @param[in] section in configuration file
     * @return true if peer address is updated
     * @note calling without section will fetch value from @"global@" section
     */
    bool setPeerAddress(const ConfigFile &cfg, const std::string &section = "") {
        return setPeerInternal(cfg.uds_address(section));
    }
    /**
     * Get self address
     * @return string object with self address
     */
    const std::string &getSelfAddress() const { return m_me; }
    /**
     * Get self address
     * @return string with self address
     */
    const char *getSelfAddress_c() const { return m_me.c_str(); }
    /**
     * Set self address
     * @param[in] string object with self address
     * @return true if self address is updated
     * @note address can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setSelfAddress(const std::string &string);
    /**
     * Set self address using predefined algorithm
     * @param[in] rootBase base used for root user
     * @param[in] useDef base used for non root user
     * @return true if self address is updated
     * @note address can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setDefSelfAddress(const std::string &rootBase = "",
        const std::string &useDef = "");
    /**
     * Get user home directory
     * @return string object with home directory
     */
    const std::string &getHomeDir();
    /**
     * Get user home directory
     * @return string with home directory
     */
    const char *getHomeDir_c();
    /**
     * Send the message using the socket to a specific address
     * @param[in] msg pointer to message memory buffer
     * @param[in] len message length
     * @param[in] addrStr Unix socket address (socket file)
     * @return true if message is sent
     * @note true does @b NOT guarantee the frame was successfully
     *  arrives its target. Only the network layer sends it.
     */
    bool sendTo(const void *msg, size_t len, const std::string &addrStr) const;
    /**
     * Send the message using the socket to a specific address
     * @param[in] buf object with message memory buffer
     * @param[in] len message length
     * @param[in] addrStr Unix socket address (socket file)
     * @return true if message is sent
     * @note true does @b NOT guarantee the frame was successfully
     *  arrives its target. Only the network layer sends it.
     */
    bool sendTo(Buf &buf, size_t len, const std::string &addrStr) const
    { return sendTo(buf.get(), len, addrStr); }
    /**
     * Receive a message using the socket from any address
     * @param[in, out] buf pointer to a memory buffer
     * @param[in] bufSize memory buffer size
     * @param[out] from Unix socket address (socket file)
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     * @note from store the origin address which send the packet
     */
    ssize_t rcvFrom(void *buf, size_t bufSize, std::string &from,
        bool block = false) const;
    /**
     * Receive a message using the socket from any address
     * @param[in] buf object with message memory buffer
     * @param[out] from Unix socket address (socket file)
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     * @note from store the origin address which send the packet
     */
    ssize_t rcvFrom(Buf &buf, std::string &from, bool block = false) const
    { return rcvFrom(buf.get(), buf.size(), from, block); }
    /**
     * Receive a message using the socket from any address
     * @param[in, out] buf pointer to a memory buffer
     * @param[in] bufSize memory buffer size
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     * @note use getLastFrom() to fetch origin address which send the packet
     */
    ssize_t rcvFrom(void *buf, size_t bufSize, bool block = false)
    { return rcvFrom(buf, bufSize, m_lastFrom, block); }
    /**
     * Receive a message using the socket from any address
     * @param[in] buf object with message memory buffer
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     * @note use getLastFrom() to fetch origin address which send the packet
     */
    ssize_t rcvFrom(Buf &buf, bool block = false)
    { return rcvFrom(buf.get(), buf.size(), m_lastFrom, block); }
    /**
     * Receive a message using the socket from any address
     * @param[in] buf object with message memory buffer
     * @param[in] block true, wait till a packet arrives.
     *                  false, do not wait, return error
     *                  if no packet available
     * @return number of bytes received or negative on failure
     * @note use getLastFrom() to fetch origin address which send the packet
     * @note identical to rcvFrom(). Some scripts fail to match proper function.
     */
    ssize_t rcvBufFrom(Buf &buf, bool block = false)
    { return rcvFrom(buf.get(), buf.size(), m_lastFrom, block); }
    /**
     * Fetch origin address from last rcvFrom() call
     * @return Unix socket address
     * @note store address only on the rcvFrom() call without the from parameter
     * @attention no protection or thread safe, fetch last rcvFrom() call with
     *  this object.
     */
    const std::string &getLastFrom() const { return m_lastFrom; }
};

/**
 * @brief Base for socket that uses network interface directly
 * @details
 *  provide functions to set network interface for UDP and Raw sockets.
 */
class SockBaseIf : public SockBase
{
  protected:
    /**< @cond internal */
    std::string m_ifName; /* interface to use */
    Binary m_mac;
    int m_ifIndex;
    bool m_have_if;
    bool setInt(const IfInfo &ifObj);
    SockBaseIf() : m_have_if(false) {}
    virtual bool setAllBase(const ConfigFile &cfg, const std::string &section) = 0;
    /**< @endcond */

  public:
    /**
     * Set network interface using its name
     * @param[in] ifName interface name
     * @return true if network interface exists and updated.
     * @note network interface can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setIfUsingName(const std::string &ifName);
    /**
     * Set network interface using its index
     * @param[in] ifIndex interface index
     * @return true if network interface exists and updated.
     * @note network interface can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setIfUsingIndex(int ifIndex);
    /**
     * Set network interface using a network interface object
     * @param[in] ifObj initialized network interface object
     * @return true if network interface exists and updated.
     * @note network interface can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setIf(const IfInfo &ifObj);
    /**
     * Set all socket parameters using a network interface object and
     *  a configuration file
     * @param[in] ifObj initialized network interface object
     * @param[in] cfg reference to configuration file object
     * @param[in] section in configuration file
     * @return true if network interface exists and update all parameters.
     * @note parameters can not be changed after initializing.
     *  User can close the socket, change any of the parameters value and
     *  initialize a new socket.
     * @note calling without section will fetch value from @"global@" section
     */
    bool setAll(const IfInfo &ifObj, const ConfigFile &cfg,
        const std::string &section = "") {
        return setIf(ifObj) && setAllBase(cfg, section);
    }
    /**
     * Set all socket parameters using a network interface object and
     *  a configuration file and initialize
     * @param[in] ifObj initialized network interface object
     * @param[in] cfg reference to configuration file object
     * @param[in] section in configuration file
     * @return true if network interface exists and update all parameters.
     * @note parameters can not be changed after initializing.
     *  User can close the socket, change any of the parameters value and
     *  initialize a new socket.
     * @note calling without section will fetch value from @"global@" section
     */
    bool setAllInit(const IfInfo &ifObj, const ConfigFile &cfg,
        const std::string &section = "") {
        return setAll(ifObj, cfg, section) && initBase();
    }
};

/**
 * @brief Base for UDP sockets
 * @details
 *  provide functions to set IP TTL, send and receive functions
 *  for UDP sockets
 */
class SockIp : public SockBaseIf
{
  protected:
    /**< @cond internal */
    int m_domain, m_udp_ttl;
    /* First for bind then for send */
    sockaddr *m_addr;
    size_t m_addr_len;
    const char *m_mcast_str; /* string form */
    Binary m_mcast;
    SockIp(int domain, const char *mcast, sockaddr *addr, size_t len);
    virtual bool initIp() = 0;
    bool sendBase(const void *msg, size_t len) override final;
    ssize_t rcvBase(void *buf, size_t bufSize, bool block) override final;
    bool initBase() override final;
    /**< @endcond */

  public:
    /**
     * Set IP ttl value
     * @param[in] udp_ttl IP time to live
     * @return true if IP ttl is updated
     * @note in IP version 6 the value is used for multicast hops
     * @note IP ttl can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setUdpTtl(uint8_t udp_ttl);
    /**
     * Set IP ttl value using configuration file
     * @param[in] cfg reference to configuration file object
     * @param[in] section in configuration file
     * @return true if IP ttl is updated
     * @note in IP version 6 the value is used for multicast hops
     * @note IP ttl can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     * @note calling without section will fetch value from @"global@" section
     */
    bool setUdpTtl(const ConfigFile &cfg, const std::string &section = "");
};

/**
 * @brief UDP over IP version 4 socket
 */
class SockIp4 : public SockIp
{
  private:
    sockaddr_in m_addr4;

  protected:
    /**< @cond internal */
    bool initIp() override final;
    bool setAllBase(const ConfigFile &cfg,
        const std::string &section) override final;

  public:
    SockIp4();
    /**< @endcond */
};

/**
 * @brief UDP over IP version 6 socket
 */
class SockIp6 : public SockIp
{
  private:
    sockaddr_in6 m_addr6;
    int m_udp6_scope;

  protected:
    /**< @cond internal */
    bool initIp() override final;
    bool setAllBase(const ConfigFile &cfg,
        const std::string &section) override final;

  public:
    SockIp6();
    /**< @endcond */

    /**
     * Set IP version 6 address scope
     * @param[in] udp6_scope IP version 6 address scope
     * @return true if IPv6 scope is updated
     * @note IPv6 scope can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setScope(uint8_t udp6_scope);
    /**
     * Set IP version 6 address scope using configuration file
     * @param[in] cfg reference to configuration file object
     * @param[in] section in configuration file
     * @return true if IPv6 scope is updated
     * @note IPv6 scope can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     * @note calling without section will fetch value from @"global@" section
     */
    bool setScope(const ConfigFile &cfg, const std::string &section = "");
};

/**
 * @brief Raw socket that uses PTP over Ethernet
 * @note The class does @b NOT support VLAN tags!
 */
class SockRaw : public SockBaseIf
{
  private:
    Binary m_ptp_dst_mac;
    int m_socket_priority;
    sockaddr_ll m_addr;
    iovec m_iov_tx[2], m_iov_rx[2];
    msghdr m_msg_tx, m_msg_rx;
    ethhdr m_hdr;
    uint8_t m_rx_buf[sizeof(ethhdr)];

  protected:
    /**< @cond internal */
    bool setAllBase(const ConfigFile &cfg,
        const std::string &section) override final;
    bool sendBase(const void *msg, size_t len) override final;
    ssize_t rcvBase(void *buf, size_t bufSize, bool block) override final;
    bool initBase() override final;

  public:
    SockRaw();
    /**< @endcond */
    /**
     * Set PTP multicast address using string from
     * @param[in] string address in a string object
     * @return true if PTP multicast address is updated
     * @note PTP multicast address can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     * @note function convert address to binary form and return false
     *  if conversion fail (address is using wrong format).
     */
    bool setPtpDstMacStr(const std::string &string);
    /**
     * Set PTP multicast address using binary from
     * @param[in] ptp_dst_mac address in binary string object
     * @return true if PTP multicast address is updated
     * @note PTP multicast address can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setPtpDstMac(const Binary &ptp_dst_mac);
    /**
     * Set PTP multicast address using binary from
     * @param[in] ptp_dst_mac address in binary form
     * @param[in] len address length
     * @return true if PTP multicast address is updated
     * @note PTP multicast address can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     */
    bool setPtpDstMac(const void *ptp_dst_mac, size_t len);
    /**
     * Set PTP multicast address using configuration file
     * @param[in] cfg reference to configuration file object
     * @param[in] section in configuration file
     * @return true if PTP multicast address is updated
     * @note PTP multicast address can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     * @note calling without section will fetch value from @"global@" section
     */
    bool setPtpDstMac(const ConfigFile &cfg, const std::string &section = "");
    /**
     * Set socket priority
     * @param[in] socket_priority socket priority value
     * @return true if socket priority is updated
     * @note socket priority can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     * @note the priority is used by network layer,
     *  it is not part of the packet
     */
    bool setSocketPriority(uint8_t socket_priority);
    /**
     * Set socket priority using configuration file
     * @param[in] cfg reference to configuration file object
     * @param[in] section in configuration file
     * @return true if socket priority is updated
     * @note socket priority can not be changed after initializing.
     *  User can close the socket, change this value, and
     *  initialize a new socket.
     * @note calling without section will fetch value from @"global@" section
     */
    bool setSocketPriority(const ConfigFile &cfg, const std::string &section = "");
};

__PTPMGMT_NAMESPACE_END

#endif /* __PTPMGMT_SOCK_H */
