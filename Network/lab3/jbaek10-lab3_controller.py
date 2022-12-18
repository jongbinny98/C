# Lab3 Skeleton
#
# Hints/Reminders from Lab 3:
#
# To check the source and destination of an IP packet, you can use
# the header information... For example:
#
# ip_header = packet.find('ipv4')
#
# if ip_header.srcip == "1.1.1.1":
#   print "Packet is from 1.1.1.1"
#
# Important Note: the "is" comparison DOES NOT work for IP address
# comparisons in this way. You must use ==.
# 
# To send an OpenFlow Message telling a switch to send packets out a
# port, do the following, replacing <PORT> with the port number the 
# switch should send the packets out:
#
#    msg = of.ofp_flow_mod()
#    msg.match = of.ofp_match.from_packet(packet)

#    msg.actions.append(of.ofp_action_output(port = <PORT>))
#    msg.data = packet_in
#    self.connection.send(msg)
#
# To drop packets, simply omit the action.
#

from pox.core import core

# You can check if IP is in subnet with 
# IPAddress("192.168.0.1") in IPNetwork("192.168.0.0/24")
# install with:
# sudo apt install python-netaddr
from netaddr import IPNetwork, IPAddress

import pox.openflow.libopenflow_01 as of

log = core.getLogger()

class Routing (object):
  """
  A Firewall object is created for each switch that connects.
  A Connection object for that switch is passed to the __init__ function.
  """
  def __init__ (self, connection):
    # Keep track of the connection to the switch so that we can
    self.connection = connection

    # This binds our PacketIn event listener
    connection.addListeners(self)
  
  # Given an IP (either an IP object or a string) and a subnet ID
  # (as a string representing a number or an actual number), this
  # function returns whether the given IP is in the subnet
  # 10.0.[subnet ID].0/24.
  def inSubnet(self, ip_obj, subnet_id):
    return IPAddress(str(ip_obj)) in IPNetwork("10.0." + str(subnet_id) + ".0/24")

  def do_routing (self, packet, packet_in, port_on_switch, switch_id):
    # port_on_swtich - the port on which this packet was received
    # switch_id - the switch which received this packet
    icmp = packet.find('icmp')
    ipv4 = packet.find('ipv4')
    tcp = packet.find('tcp')

    def accept(por):
      msg = of.ofp_flow_mod()
      msg.match = of.ofp_match.from_packet(packet)
      msg.actions.append(of.ofp_action_output(port = por))
      msg.buffer_id = packet_in.buffer_id
      self.connection.send(msg)
    
    sourceIP = ipv4.srcip      
    destinationIP = ipv4.dstip

    if ipv4 == None:
      msg = of.ofp_flow_mod()
      # msg.match = of.ofp_match.from_packet(packet)
      msg.actions.append(of.ofp_action_output(port=of.OFPP_FLOOD))
      msg.buffer_id = packet_in.buffer_id
      self.connection.send(msg)
    if switch_id == 4: #s4
      if icmp != None:
        if port_on_switch == 2:         # port2 -> port1 on switch 4
          accept(1)
        elif port_on_switch == 1:       # port1 -> port2 on switch 4
          if self.inSubnet(sourceIP, 0) or self.inSubnet(sourceIP, 1):  #if sourceip is 
            if str(destinationIP) == "10.0.1.1":
              accept(2)
    elif switch_id == 2: #s2
      if icmp != None:
        if port_on_switch == 3: 
          if self.inSubnet(sourceIP, 1) and self.inSubnet(destinationIP, 0):   # port3 -> port1
            accept(1)
        elif port_on_switch == 1:
          if self.inSubnet(sourceIP, 0) and self.inSubnet(destinationIP, 1):   # port1 -> port3
            accept(3)
    elif switch_id == 1: #s1
      if port_on_switch == 4:
        if icmp != None:
          if self.inSubnet(sourceIP, 0) or self.inSubnet(sourceIP, 1):   # port4 -> port1 or port2
            if str(destinationIP) == "10.0.0.1":
              accept(1)
            elif str(destinationIP) == "10.0.0.2":
              accept(2)
      elif port_on_switch == 2:
        if icmp != None:
          if str(sourceIP) == "10.0.0.2":
            if str(destinationIP) == "10.0.0.1":
              accept(1)
            elif self.inSubnet(destinationIP, 1):
              accept(4)
        elif tcp != None:
          if str(sourceIP) == "10.0.0.2":
            if str(destinationIP) == "10.0.0.1":
              accept(1)
            elif self.inSubnet(destinationIP, 2):
              accept(3)
      elif port_on_switch == 1:
        if icmp != None:
          if str(sourceIP) == "10.0.0.1":
            if str(destinationIP) == "10.0.0.2":
              accept(2)
            elif self.inSubnet(destinationIP, 1):
              accept(4)
        elif tcp != None:
          if str(sourceIP) == "10.0.0.1":
            if str(destinationIP) == "10.0.0.2":
              accept(2)
            elif self.inSubnet(destinationIP, 2):
              accept(3)
      elif port_on_switch == 3:
        if tcp != None:
          if str(destinationIP) == "10.0.0.1":
            accept(1)
          elif str(destinationIP) == "10.0.0.2":
            accept(2)
    elif switch_id == 3: #s3
      if tcp != None:
        if port_on_switch == 1:
          if str(destinationIP) == "10.0.2.1":
            accept(2)
          elif str(destinationIP) == "10.0.2.2":
            accept(3)
        elif port_on_switch == 2:
          if str(sourceIP) == "10.0.2.1":
            if str(destinationIP) == "10.0.2.2":
              accept(3)
            elif self.inSubnet(destinationIP, 0):
              accept(1)
        elif port_on_switch == 3:
          if str(sourceIP) == "10.0.2.2":
            if str(destinationIP) == "10.0.2.1":
              accept(2)
            elif self.inSubnet(destinationIP, 0):
              accept(1)
      elif icmp != None:
        if port_on_switch == 2:
          if str(sourceIP) == "10.0.2.1":
            if str(destinationIP) == "10.0.2.2":
              accept(3)
        if port_on_switch == 3:
          if str(sourceIP) == "10.0.2.2":
            if str(destinationIP) == "10.0.2.1":
              accept(2)
    pass

  def _handle_PacketIn (self, event):
    """
    Handles packet in messages from the switch.
    """
    packet = event.parsed # This is the parsed packet data.
    if not packet.parsed:
      log.warning("Ignoring incomplete packet")
      return

    packet_in = event.ofp # The actual ofp_packet_in message.
    self.do_routing(packet, packet_in, event.port, event.dpid)

def launch ():
  """
  Starts the component
  """
  def start_switch (event):
    log.debug("Controlling %s" % (event.connection,))
    Routing(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)
