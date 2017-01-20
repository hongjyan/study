/*****************************************************************************
 * @brief  Ping proxy implementation.
 * @module Ping Proxy
 * @owner  Avengers
 *
 * Copyright 2017 Nokia. All rights reserved.
 *****************************************************************************/

#include <string>
#include <thread>
#include <cmath>

#include <tpi/PingProxy.hpp>

#include <common/Utils.hpp>
#include <common/ContextImpl.hpp>
#include <common/LogProxy.hpp>
#include <common/NamespaceProxyImpl.hpp>

#include "ProxyImpl.hpp"

namespace tpi
{
namespace ping
{

const int8_t NOT_AVAILABLE = -2;
const uint8_t ZERO = 0;
const uint32_t PINGRESPONSE_TIMEOUT_5SEC = 5;
const float REAL_ZERO = 0.0;
const std::string PING = "ping";
const std::string PING6 = "ping6";
const std::string DONT_FRAG = "do";
const std::string ALLOW_FRAG = "dont";
const std::string RTT_LINE = " = ";
const std::string RTT_DELIM = "/";
const std::string RECEIVE_START = ", ";
const std::string RECEIVE_END = "received";
const std::string RECEIVE_STRING = "0 ";

Proxy::impl::impl(In<NamespaceProxy> ctx)
            : ctx_(ctx.getImpl()->getContext())
{
}

Proxy::impl::~impl()
{
}

uint32_t Proxy::impl::convertRttToInt(InOut<std::stringstream> rttStream)
{
    uint32_t rttInteger = ZERO;
    float rttFloat = REAL_ZERO;
    rttStream >> rttFloat;
    rttInteger = ((rttFloat - trunc(rttFloat)) > REAL_ZERO)? (ceil(rttFloat)) : rttFloat;
    return rttInteger;
}

uint32_t Proxy::impl::getRtt(In<std::string> input)
{
    std::size_t pos = input.find(RTT_LINE);

    if (std::string::npos != pos)
    {
        size_t rttStartPos = input.find(RTT_DELIM, pos);
        if (std::string::npos != rttStartPos)
        {
            rttStartPos += RTT_DELIM.length();
            size_t rttEndPos = input.find(RTT_DELIM, rttStartPos);
            std::stringstream rttStream;
            rttStream << input.substr(rttStartPos, rttEndPos - rttStartPos);

            return convertRttToInt(rttStream);
        }
    }
    TPL_LOG(TPL_WARNING, "Rtt value not present in Ping response.");
    return ZERO;
}

std::string Proxy::impl::getPacketsReceived(In<std::string> input)
{
    std::string packetReceived;
    std::size_t pos = input.find(RECEIVE_START);

    if (std::string::npos == pos)
    {
        TPL_LOG(TPL_ERROR, "Ping failed. Unable to parse response for received packets");
    }
    else
    {
        std::size_t receiveEndPos = input.find(RECEIVE_END, pos);
        if (std::string::npos != receiveEndPos)
        {
            std::size_t receiveStartPos = pos + RECEIVE_START.length();
            packetReceived = input.substr(receiveStartPos, (receiveEndPos - receiveStartPos));
        }
    }
    return packetReceived;
}

std::string Proxy::impl::createCommand(In<Request> req)
{
    int destIpType = tpl::common::getIpType((req.destinationIpAddress).to_string());
    int srcIpType = NOT_AVAILABLE;

    if (req.sourceIpAddress.is_initialized() && (!((req.sourceIpAddress.get()).to_string()).empty()))
    {
        srcIpType = tpl::common::getIpType((req.sourceIpAddress.get()).to_string());
    }

    std::stringstream systemCmdStrStream;
    if (srcIpType == NOT_AVAILABLE || destIpType == srcIpType)
    {
        std::string command = (destIpType == AF_INET)? PING:PING6;
        std::string dfBit = (req.dfBit.get() == true)? DONT_FRAG:ALLOW_FRAG;

        (srcIpType == NOT_AVAILABLE)?
        (systemCmdStrStream << command << " -i "<< req.interval.get() << " -M " << dfBit << " -W "
        << PINGRESPONSE_TIMEOUT_5SEC << " -c "<< (req.count.get()) << " -s " << req.dataSize.get()
        << " -Q " << (uint32_t)req.dscp.get() << " " << (req.destinationIpAddress).to_string()) :

        (systemCmdStrStream << command << " -i "<< req.interval.get() << " -M " << dfBit << " -W "
        << PINGRESPONSE_TIMEOUT_5SEC << " -c "<< (req.count.get()) << " -I "
        << (req.sourceIpAddress.get()).to_string() << " -s " << req.dataSize.get() << " -Q "
        << (uint32_t)req.dscp.get() << " " << (req.destinationIpAddress).to_string());
    }
    else
    {
        TPL_LOG(TPL_ERROR, "Ping Failed. Invalid Ip Address");
    }
    return systemCmdStrStream.str();
}

ErrorCode Proxy::impl::processResponse(In<std::string> input, Out<tpi::ping::Response> pingResult)
{
    ErrorCode ec = tpi::make_error_code(tpi::ErrorValue::InternalError);
    std::string packetReceived = getPacketsReceived(input);

    if ((!packetReceived.empty()) && (packetReceived.compare(RECEIVE_STRING) != ZERO))
    {
        uint32_t rtt = getRtt(input);
        pingResult.successfulPings = atoi(packetReceived.c_str());
        pingResult.avgRtt = rtt;
        ec = tpi::make_error_code(tpi::ErrorValue::Success);
    }
    else
    {
        pingResult.successfulPings = ZERO;
        pingResult.avgRtt = ZERO;
        TPL_LOG(TPL_ERROR, "Ping Falied. Received unknown number of packets:"<< packetReceived.c_str());
    }
    return ec;
}

void Proxy::impl::getPingResponseAndInitiateCallback(In<Request> req, In<Handler<Response>> handler)
{
    Context& context = ctx_;
    context.getImpl()->setPingThreadId(std::this_thread::get_id());
    std::string systemCmdString = createCommand(req);
    ErrorCode ec = tpi::make_error_code(tpi::ErrorValue::InternalError);
    tpi::ping::Response pingResult;

    if (!(systemCmdString.empty()))
    {
        std::string output;
        if (!(ec = tpl::common::executeCmd(systemCmdString, output)))
        {
            ec = processResponse(output, pingResult);
        }
    }
    if (context.getImpl()->getPingThreadId() == std::this_thread::get_id())
    {
        handler(ec, "", pingResult);
    }
}

ErrorCode Proxy::impl::start(In<Request> req, In<Handler<Response>> handler)
{
    std::thread pingThread(&Proxy::impl::getPingResponseAndInitiateCallback, this, req, handler);
    pingThread.detach();
    return (tpi::make_error_code(tpi::ErrorValue::Success));
}

ErrorCode Proxy::impl::stop()
{
    Context& context = ctx_;
    context.getImpl()->setPingThreadId(std::this_thread::get_id());
    return (tpi::make_error_code(tpi::ErrorValue::Success));
}

}  // namespace ping
}  // namespace tpi

