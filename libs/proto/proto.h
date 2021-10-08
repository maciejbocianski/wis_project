


class Transport {
    public:
        virtual bool send(void *packet) = 0;
        virtual bool is_connected() const = 0;  
};
    

class Proto {
    public:
        bool send_packet(void *packet)
        {
            if (!is_connected()) {
                return false;
            }
            return _transport->send(packet);
        }

        bool is_connected() const
        {
            return _transport->is_connected();
        }

        void set_transport(Transport *transport) 
        {
            _transport = transport;
        }

    private:
        Transport *_transport;
};
