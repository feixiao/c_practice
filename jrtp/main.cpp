#include <iostream>

#include <rtppacket.h>
#include <rtppacketbuilder.h>
#include <rtprandom.h>

using jrtplib::RTPPacket;
using jrtplib::RTPPacketBuilder;
using jrtplib::RTPRandom;

// g++ -o rtp main.cpp -I/usr/local/include/jrtplib3  -l jrtp
int main() {
    auto random = jrtplib::RTPRandom::CreateDefaultRandomNumberGenerator();
    RTPPacketBuilder builder(*random);
    
    builder.Init(1200);
    builder.SetDefaultPayloadType(107);
    builder.SetDefaultTimestampIncrement(20);
    builder.SetDefaultMark(true);

    uint8_t buf[128];
    builder.BuildPacket(buf, 128);

    auto pBuf = builder.GetPacket();
    printf("seq:%llu ts:%llu \n", builder.GetSequenceNumber(), builder.GetTimestamp());

    builder.BuildPacket(buf, 128);

    printf("seq:%llu ts:%llu \n", builder.GetSequenceNumber(), builder.GetTimestamp());

    builder.BuildPacket(buf, 128);

    printf("seq:%llu ts:%llu \n", builder.GetSequenceNumber(), builder.GetTimestamp());

    return 0;
}