/* SPDX-License-Identifier: LGPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright 2021 Erez Geva */

/** @file
 * @brief messages dispatcher and builder classes
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright 2022 Erez Geva
 */

%luacode %{
ptpmgmt.MessageDispatcher = {}
function ptpmgmt.MessageDispatcher:callHadler(msg, tlv_id, tlv)
    if(type(msg) ~= 'userdata' or getmetatable(msg)['.type'] ~= 'Message') then
        error('MessageDispatcher::callHadler() msg must be a Message object', 2)
    end
    if(tlv == nil) then
        tlv = msg:getData()
        tlv_id = msg:getTlvId()
    elseif(type(tlv_id) ~= 'number') then
        error('MessageDispatcher::callHadler() tlv_id must be a number', 2)
    elseif(type(tlv) ~= 'userdata' or getmetatable(tlv)['.type'] ~= 'BaseMngTlv') then
        error('MessageDispatcher::callHadler() tlv must be a BaseMngTlv object', 2)
    end
    if(tlv ~= nil) then
        local idstr = ptpmgmt.Message.mng2str_c(tlv_id)
        local callback_name = idstr .. '_h'
        local conv_func = 'conv_' .. idstr
        if(type(getmetatable(self)[callback_name]) == "function" and
           type(ptpmgmt[conv_func]) == "function") then
            local data = ptpmgmt[conv_func](tlv)
            if(data ~= nil) then
                getmetatable(self)[callback_name](self, msg, data, idstr)
                return
            end
        end
    end
    if(type(getmetatable(self)['noTlv']) == "function") then
        self:noTlv(msg)
    end
end
function ptpmgmt.MessageDispatcher:new(msg)
    local obj = {}
    setmetatable(obj, self)
    self.__index = self
    if(msg ~= nil) then
        if(type(msg) ~= 'userdata' or getmetatable(msg)['.type'] ~= 'Message') then
            error('MessageDispatcher::MessageDispatcher() msg must be a Message object', 2)
        else
            self:callHadler(msg)
        end
    end
    return obj
end

ptpmgmt.MessageBulder = {}
function ptpmgmt.MessageBulder:buildtlv(actionField, tlv_id)
    if(type(actionField) ~= 'number') then
        error('MessageBulder::buildTlv() actionField must be a number', 2)
    elseif(type(tlv_id) ~= 'number') then
        error('MessageBulder::buildTlv() tlv_id must be a number', 2)
    end
    if(actionField == ptpmgmt.GET or ptpmgmt.Message.isEmpty(tlv_id)) then
        return self.m_msg:setAction(actionField, tlv_id)
    end
    local idstr = ptpmgmt.Message.mng2str_c(tlv_id)
    local tlv_pkg = idstr .. '_t'
    local callback_name = idstr .. '_b'
    if(type(getmetatable(self)[callback_name]) == "function" and
       type(ptpmgmt[tlv_pkg]) == "table") then
        local tlv = ptpmgmt[tlv_pkg]()
        if(tlv ~= nil and
           getmetatable(self)[callback_name](self, self.m_msg, tlv) and
           self.m_msg:setAction(actionField, tlv_id, tlv)) then
            self.m_tlv = tlv
            return true
        end
    end
    return false
end
function ptpmgmt.MessageBulder:new(msg)
    if(type(msg) ~= 'userdata' or getmetatable(msg)['.type'] ~= 'Message') then
        error('MessageBulder::MessageBulder() msg must be a Message object', 2)
    end
    local obj = { m_msg = msg }
    setmetatable(obj, self)
    self.__index = self
    return obj
end
%}
