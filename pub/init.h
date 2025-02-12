/* SPDX-License-Identifier: LGPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright © 2021 Erez Geva <ErezGeva2@gmail.com> */

/** @file
 * @brief Init a pmc application
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2021 Erez Geva
 *
 */

#ifndef __PTPMGMT_INIT_H
#define __PTPMGMT_INIT_H

#include "msg.h"
#include "opt.h"
#include "sock.h"

__PTPMGMT_NAMESPACE_BEGIN

/**
 * Initilize configuration file, socket, message based on PMC options.
 * The user can write an application using the pmc command line.
 */
class Init
{
  private:
    ConfigFile m_cfg;
    Message m_msg;
    std::unique_ptr<SockBase> m_sk;
    char m_net_select;
    bool m_use_uds;

  public:
    /**
     * Close socket
     */
    void close();

    /**
     * Proccess PMC options
     * @param[in] opt PMC options
     * @return 0 on scuccess
     * @note function return proper value to return from main()
     */
    int proccess(const Options &opt);

    /**
     * Get configuration file object
     * @return object
     */
    const ConfigFile &cfg() const { return m_cfg; }

    /**
     * Get Message object
     * @return object
     */
    Message &msg() { return m_msg; }

    /**
     * Get Socket object
     * @return object or null if not exist
     * @note User @b should not try to free this socket object
     */
    SockBase *sk() { return m_sk.get(); }

    /**
     * Get network selection character
     * @return
     *   'u' for unix socket using a SockUnix object,
     *   '4' for a PTP IPv4 socket using a SockIp4 object,
     *   '6' for a PTP IPv6 socket using a SockIp6 object,
     *   '2' for a PTP layer 2 socket using a SockRaw object,
     */
    char getNetSelect() { return m_net_select; }

    /**
     * Is the socket provide by this object, a Unix socket?
     * @return true if the socket is a UDS socket
     */
    bool use_uds() const { return m_use_uds; }
};

__PTPMGMT_NAMESPACE_END

#endif /* __PTPMGMT_INIT_H */
