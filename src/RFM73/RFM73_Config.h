#ifndef RFM73_CONFIG_H
#define RFM73_CONFIG_H

// ---------------------------- Network Settings ---------------------------- //
#ifndef RFM73_STACK_RF_CHANNEL
  #define RFM73_STACK_RF_CHANNEL                  5
#endif

#ifndef RFM73_STACK_DATARATE
  #define RFM73_STACK_DATARATE                    RFM73_DATARATE_1MBPS
#endif

#ifndef RFM73_STACK_LDA_GAIN_ENABLE
  #define RFM73_STACK_LDA_GAIN_ENABLE             true
#endif

#ifndef RFM73_STACK_NETWORK_ADDRESS
  #define RFM73_STACK_NETWORK_ADDRESS             0xBBBB
#endif

#ifndef RFM73_STACK_OUTPUT_POWER
  #define RFM73_STACK_OUTPUT_POWER                RFM73_PLUS_5DBM
#endif

// -------------------------------------------------------------------------- //

// ----------------------------- Pipe Settings ------------------------------ //
#ifndef RFM73_STACK_RX_ADDR_PIPE0
  #define RFM73_STACK_RX_ADDR_PIPE0               0xAA
#endif

#ifndef RFM73_STACK_RX_ADDR_PIPE1
  #define RFM73_STACK_RX_ADDR_PIPE1               0x01
#endif

#ifndef RFM73_STACK_RX_ADDR_PIPE2
  #define RFM73_STACK_RX_ADDR_PIPE2               0x02
#endif

#ifndef RFM73_STACK_RX_ADDR_PIPE3
  #define RFM73_STACK_RX_ADDR_PIPE3               0x55
#endif

#ifndef RFM73_STACK_RX_ADDR_PIPE4
  #define RFM73_STACK_RX_ADDR_PIPE4               0x04
#endif

#ifndef RFM73_STACK_RX_ADDR_PIPE5
  #define RFM73_STACK_RX_ADDR_PIPE5               0x05
#endif


#endif