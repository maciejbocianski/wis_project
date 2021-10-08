#include <memory>
#include <tuple>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"


#include "proto.h"

class TransportMock : public Transport {
public:
    TransportMock()
    {
    }

    MOCK_CONST_METHOD0(is_connected, bool());
    MOCK_METHOD1(send, bool(void *packet));
};



class ProtoTest : public ::testing::Test {
public:
    ProtoTest() {}

protected:
    void SetUp() override
    {
        proto.set_transport(&transport);
    }

    void TearDown() override
    {
        // cleanup resources used
        
    }

    void set_tranport_connected()
    {
        ON_CALL(transport, is_connected()).WillByDefault(testing::Return(true));
    }

    void set_tranport_disconnected()
    {
        ON_CALL(transport, is_connected()).WillByDefault(testing::Return(false));
    }


    Proto proto;
    TransportMock transport;
};

TEST_F(ProtoTest, tranport_connection_state)
{
    // check if protocol is connected when transport is connected
    set_tranport_connected();
    EXPECT_CALL(transport, is_connected()).Times(1).RetiresOnSaturation();
    EXPECT_EQ(true, proto.is_connected());

    // check if protocol is disconnected when transport is disconnected
    set_tranport_disconnected();
    EXPECT_CALL(transport, is_connected()).Times(1).RetiresOnSaturation();
    EXPECT_EQ(false, proto.is_connected());
}


TEST_F(ProtoTest, sending)
{
    void *data;
    // check if sending fail when transport is disconnected
    set_tranport_disconnected();
    EXPECT_CALL(transport, is_connected()).Times(1).RetiresOnSaturation();
    EXPECT_EQ(false, proto.send_packet(data));

    // check if sending succeed when transport is connected
    set_tranport_connected();
    EXPECT_CALL(transport, is_connected()).Times(1).RetiresOnSaturation();
    EXPECT_CALL(transport, send(::testing::_)).WillOnce(testing::Return(true)).RetiresOnSaturation();
    EXPECT_EQ(true, proto.send_packet(data));

    // check if sending fail when transport send fail
    set_tranport_connected();
    EXPECT_CALL(transport, is_connected()).Times(1).RetiresOnSaturation();
    EXPECT_CALL(transport, send(::testing::_)).WillOnce(testing::Return(false)).RetiresOnSaturation();
    EXPECT_EQ(false, proto.send_packet(data));
}
