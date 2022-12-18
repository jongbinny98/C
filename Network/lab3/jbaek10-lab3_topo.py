from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.cli import CLI
from mininet.node import RemoteController

class final_topo(Topo):
  def build(self):
    # setting up the hosts
    h1 = self.addHost('h1',mac='00:00:00:00:00:01',ip='10.0.0.1/24', defaultRoute="h1-eth1")
    h2 = self.addHost('h2',mac='00:00:00:00:00:02',ip='10.0.0.2/24', defaultRoute="h2-eth1")
    h3 = self.addHost('h3',mac='00:00:00:00:00:03',ip='10.0.1.1/24', defaultRoute="h3-eth1")
    h4 = self.addHost('h4',mac='00:00:00:00:00:04',ip='10.0.2.1/24', defaultRoute="h4-eth1")
    h5 = self.addHost('h5',mac='00:00:00:00:00:05',ip='10.0.2.2/24', defaultRoute="h5-eth1")
    
    # setting up the switch
    s1 = self.addSwitch('s1')
    s2 = self.addSwitch('s2')
    s3 = self.addSwitch('s3')
    s4 = self.addSwitch('s4')

    # setting up the link between host and the switch
    self.addLink('h1', 's1', port1=1, port2=1)
    self.addLink('h2', 's1', port1=1, port2=2)
    self.addLink('h3', 's4', port1=1, port2=2)
    self.addLink('h4', 's3', port1=1, port2=2)
    self.addLink('h5', 's3', port1=1, port2=3)

    # setting up the link between switch and switch
    self.addLink('s1', 's3', port1=3, port2=1)
    self.addLink('s1', 's2', port1=4, port2=1)
    self.addLink('s3', 's2', port1=4, port2=2)
    self.addLink('s2', 's4', port1=3, port2=1)    

    pass


def configure():
  topo = final_topo()
  net = Mininet(topo=topo, controller=RemoteController)
  net.start()
  # use static ARP
  net.staticArp() 
  CLI(net)
  
  net.stop()


if __name__ == '__main__':
  configure()
