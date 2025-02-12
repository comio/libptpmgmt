/* SPDX-License-Identifier: LGPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright © 2022 Erez Geva <ErezGeva2@gmail.com> */

/** @file
 * @brief Messages dispatcher and builder classes
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2022 Erez Geva
 */

#include "msgCall.h"
#include "comp.h"

__PTPMGMT_NAMESPACE_BEGIN

void MessageDispatcher::callHadler(const Message &msg)
{
    callHadler(msg, msg.getTlvId(), msg.getData());
}
#ifdef HAVE_METHODS_COMPARE
DIAG_START
DIAG_IGNORE(METHODS_COMPARE_FLAGS)
// We convert function pointer for comparing, ignore warning
#define check_inherit(n) \
    if ((void*)(&MessageDispatcher::n##_h) == \
        (void*)(this->*(&MessageDispatcher::n##_h))) {\
        noTlvCallBack(msg, #n); return; }
#else /*HAVE_METHODS_COMPARE*/
#define check_inherit(n)
#endif /*HAVE_METHODS_COMPARE*/
#define _ptpmCaseUF(n) \
    case n: check_inherit(n)\
    n##_h(msg, *dynamic_cast<const n##_t*>(tlv), #n); break;
#define A(n, v, sc, a, sz, f) _ptpmCase##f(n)
void MessageDispatcher::callHadler(const Message &msg, mng_vals_e tlv_id,
    const BaseMngTlv *tlv)
{
    if(tlv == nullptr) {
        noTlv(msg);
        return;
    }
    switch(tlv_id) {
#include "ids.h"
        default:
            noTlv(msg);
            break;
    }
}
#ifdef HAVE_METHODS_COMPARE
DIAG_END
#endif /*HAVE_METHODS_COMPARE*/
bool MessageBuilder::buildTlv(actionField_e actionField, mng_vals_e tlv_id)
{
    if(!m_msg.isValidId(tlv_id))
        return false;
    if(actionField == GET || m_msg.isEmpty(tlv_id))
        return m_msg.setAction(actionField, tlv_id);
    if(actionField != SET && actionField != COMMAND)
        return false;
    BaseMngTlv *tlv = nullptr;
#define _ptpmCaseUFB(n) case n: {\
            n##_t *d = new n##_t;\
            if (d == nullptr) return false;\
            if (n##_b(m_msg, *d)) tlv = d; else delete d;\
            break; }
    switch(tlv_id) {
#define A(n, v, sc, a, sz, f) _ptpmCase##f(n)
#include "ids.h"
        default:
            return false;
    }
    if(tlv == nullptr)
        return false;
    m_tlv.reset(tlv);
    return m_msg.setAction(actionField, tlv_id, tlv);
}

__PTPMGMT_NAMESPACE_END
