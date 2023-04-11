#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/wifi-module.h"
#include "ns3/wave-module.h"




using namespace ns3;


int CalculateThroughput (FlowMonitorHelper *flowHelper, Ptr<FlowMonitor> monitor){
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper->GetClassifier ());

  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin ();
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
    std::cout << "Flow " << t.sourceAddress << " -> " << t.destinationAddress << "\n";
    std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
    std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
    std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ()) / 1000000 << " Mbps\n";
    return i->second.rxPackets;  
  
}


int run_simulation(float d){
// Create nodes
    NodeContainer nodes;
    nodes.Create(2);
    /*
    InternetStackHelper stack;
	  stack.Install (nodes);

	  Ipv4AddressHelper address;
	  address.SetBase ("1.1.1.0", "255.255.255.0");

	  Ipv4InterfaceContainer interfaces = address.Assign (devices);
	  
	  UdpServerHelper server1 (9);
	  ApplicationContainer apps;
	  apps = server1.Install (nodes.Get (1));
	  apps.Start (Seconds (1.0));
	  apps.Stop (Seconds (10.0));

	  UdpClientHelper client1 (interfaces.GetAddress (1), 9);
	  
	  client1.SetAttribute ("MaxPackets", UintegerValue (320));
	  client1.SetAttribute ("Interval", TimeValue (Seconds (0.05)));
	  client1.SetAttribute ("PacketSize", UintegerValue (10000));

	  apps = client1.Install (nodes.Get (0));
	  apps.Start (Seconds (2.0));
	  apps.Stop (Seconds (10.0));
*/
    YansWifiPhyHelper phy;
    phy.Set("TxPowerLevels", UintegerValue(3));
    phy.Set("TxPowerStart", DoubleValue(20));
    phy.Set("TxPowerEnd", DoubleValue(90));
    Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default();
    NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default();
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiChannel.AddPropagationLoss ("ns3::NakagamiPropagationLossModel","m0", DoubleValue (1.0),"m1", DoubleValue (1.0),"m2", DoubleValue (1.0));
    wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel","ReferenceLoss", DoubleValue (10.0));
    phy.SetChannel(wifiChannel.Create());
    phy.SetPcapDataLinkType(YansWifiPhyHelper::DLT_IEEE802_11);
    wifi80211p.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("OfdmRate6MbpsBW10MHz"), "ControlMode", StringValue("OfdmRate6MbpsBW10MHz"));
    NetDeviceContainer devices = wifi80211p.Install(phy, wifi80211pMac, nodes);
    WifiMacHelper wifiMac;
    wifiMac.SetType ("ns3::AdhocWifiMac");


    // model
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = Ptr<ListPositionAllocator>(new ListPositionAllocator());
    positionAlloc->Add(Vector(0.0, 0.0, 0.0)); // node 0
    positionAlloc->Add(Vector(d, 0.0, 0.0)); // node 1
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);

    // Install internet stack
    InternetStackHelper internet;
    internet.Install(nodes);

    // Assign IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = ipv4.Assign(devices);

    // Set up CBR traffic
    uint16_t port = 9;
    OnOffHelper onoff = OnOffHelper("ns3::UdpSocketFactory", Address(InetSocketAddress(interfaces.GetAddress(1), port)));
    onoff.SetConstantRate(DataRate("1Mbps"));
    ApplicationContainer app = onoff.Install(nodes.Get(0));
    app.Start(Seconds(1.0));
    app.Stop(Seconds(20.0));

    // Set up packet sink to receive traffic
    PacketSinkHelper sink = PacketSinkHelper("ns3::UdpSocketFactory", Address(InetSocketAddress(Ipv4Address::GetAny(), port)));
    app = sink.Install(nodes.Get(1));
    app.Start(Seconds(1.0));


    // Enable tracing
    phy.EnablePcap("TwoNodeAdhocNetwork", devices);

    // Run simulation
    FlowMonitorHelper flowHelper;
    Ptr<FlowMonitor> monitor = flowHelper.Install (nodes);
	  
	  
    Simulator::Stop(Seconds(36.0));
    Simulator::Run();
    int a = CalculateThroughput (&flowHelper, monitor);
    Simulator::Destroy();

    std::cout << "  Distance: " << d << std::endl;
    return a;
}

int main(int argc, char *argv[]) {

    CommandLine cmd;
    cmd.Parse(argc, argv);
    float i = 1.0;  
    while(run_simulation(i)>0){
     if(i>=4){
     i+=0.1;
     }
     else{
     i+=1;
    }
    }
    return 0;
}


