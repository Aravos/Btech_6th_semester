#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;


void CalculateThroughput (FlowMonitorHelper *flowHelper, Ptr<FlowMonitor> monitor)
{
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper->GetClassifier ());

  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
  {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
    std::cout << "Flow " << i->first << " (Node1-" << t.sourceAddress << " -> Node2-" << t.destinationAddress << ")\n";
    std::cout << "  Packets Sent: " << i->second.txPackets << "\n";
    std::cout << "  Packets Recived: " << i->second.rxPackets << "\n";
    std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ()) / 1000000 << " Mbps\n";
  }
}



int main (int argc, char *argv[]){
  //PART C
  
  double lat = 20.0;
  uint64_t rate1 = 5000000; // Data rate in bps
  
  for (int i = 1; i <= 10; i++) {
	  NodeContainer nodes;
	  nodes.Create (2);
	  
	  PointToPointHelper p2p;
	  p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (i*lat)));
	  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (rate1)));
	  //p2p.SetDeviceAttribute ("Mtu", UintegerValue (1400));

	  NetDeviceContainer devices = p2p.Install (nodes);
	  
	  InternetStackHelper stack;
	  stack.Install (nodes);

	  Ipv4AddressHelper address;
	  address.SetBase ("1.1.1.0", "255.255.255.0");

	  Ipv4InterfaceContainer interfaces = address.Assign (devices);

	  UdpServerHelper server (9);
	  ApplicationContainer apps = server.Install (nodes.Get (1));
	  apps.Start (Seconds (1.0));
	  apps.Stop (Seconds (10.0));

	  UdpClientHelper client (interfaces.GetAddress (1), 9);
	  client.SetAttribute ("MaxPackets", UintegerValue (320));
	  client.SetAttribute ("Interval", TimeValue (Seconds (0.05)));
	  client.SetAttribute ("PacketSize", UintegerValue (10000));
	  apps = client.Install (nodes.Get (0));
	  apps.Start (Seconds (2.0));
	  apps.Stop (Seconds (10.0));

	 // Install flow monitor on the nodes
	  FlowMonitorHelper flowHelper;
	  Ptr<FlowMonitor> monitor = flowHelper.Install (nodes);
	  
	  Simulator::Stop (Seconds (10.0));
	  Simulator::Run ();

	  CalculateThroughput (&flowHelper, monitor);

	  Simulator::Destroy ();
	  std::cout << "Link latency: " << i * lat << " ms\n";
	  }
  return 0;
}
