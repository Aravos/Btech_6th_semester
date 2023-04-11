#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

int main (int argc, char *argv[]){
  //PART A
  double lat = 2.0;
  uint64_t rate = 5000000; // Data rate in bps

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper p2p;
  p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (lat)));
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (rate)));
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (1400));

  NetDeviceContainer devices = p2p.Install (nodes);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
