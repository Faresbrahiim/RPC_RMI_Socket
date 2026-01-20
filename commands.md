routage statique : 
    R1(config)# ip route 192.168.2.0 255.255.255.0 192.168.1.2
    R1# show ip route
    R1(config)# no ip route 192.168.2.0 255.255.255.0 192.168.1.2
    R1(config)# ip route 0.0.0.0 0.0.0.0 10.0.0.1
cable serial (hada 9bel ymkn)
    R1(config)# interface serial0/1/0
    R1(config-if)# ip address 10.0.0.1 255.255.255.252
    R1(config-if)# no shutdown
    R1(config-if)# clock rate 128000   ! si cette interface est DCE
    R1(config-if)# do wr
Configuration RIP 
    R1(config)# router rip
    R1(config-router)# version 2
    R1(config-router)# no auto-summary
    R1(config-router)# network 192.168.1.0 // adresse net
    R1(config-router)# network 10.0.0.0 // adresse net
Configuration OSPF
    R1(config)# router ospf 1 
    R1(config-router)# network 192.168.1.0 0.0.0.255 area 0
    R1(config-router)# network 10.0.0.0 0.0.0.3 area 0
Configuration DHCP
    Router(config)# ip dhcp pool LAN
    Router(dhcp-config)# network 192.168.1.0 255.255.255.0
    Router(dhcp-config)# default-router 192.168.1.1
    Router(dhcp-config)# dns-server 8.8.8.8
    Router(dhcp-config)# exit
Configurer R1 pour SSH
    Créer un utilisateur xxx avec mot de passe yyy
      enable
    configure terminal
    // name of the router 
    hostname R1
    // just a identifer needed to genrate a key for encryption
    ip domain-name reseau.local 
    crypto key generate rsa
    ip ssh version 2

    // Configuring VTY lines 0 to 4 means 5 users can be connected (simultaneous)on this line 
    line vty 0 4
    // activate the  username and password for this router
    login local
     // Only allow SSH connections (no Telnet)
    transport input ssh

    ssh -l xxx 192.168.1.1
    Mot de passe : yyy

ACL 
    ip access-list extended WEB-FILTER
    deny tcp host 192.168.1.50 host 192.168.2.10 eq 80
    permit tcp 192.168.1.0 0.0.0.255 host 192.168.2.10 eq 80
    permit ip any any
VLANS 

    ! Sous-interface pour VLAN 10  (just virtual ones)
    R1(config)# interface g0/0.10
    R1(config-subif)# encapsulation dot1Q 10
    R1(config-subif)# ip address 10.0.0.1 255.0.0.0
    R1(config-subif)# exit

! Sous-interface pour VLAN 20
// encapsulate  like give each  vlan a  color sticker just to avoid confusion  in trunk
    R1(config)# interface g0/0.20
    R1(config-subif)# encapsulation dot1Q 20
    R1(config-subif)# ip address 20.0.0.1 255.0.0.0
    R1(config-subif)# exit

    Switch> enable
    Switch# configure terminal
    Switch(config)# hostname S1

    Switch(config)# vlan 10
    Switch(config-vlan)# name prof
    Switch(config-vlan)# exit

    Switch(config)# vlan 20
    Switch(config-vlan)# name student
    Switch(config-vlan)# exit

     This selects the physical port FastEthernet0/2 on the switch cs switch have many ports
! Port pour PC1 → VLAN10
    Switch(config)# interface f0/2
    Think: “I’m telling the switch: this is the door PC1 will use” ,, 
    Switch(config-if)# switchport mode access
    Assigns the port f0/2 to VLAN 10
    Switch(config-if)# switchport access vlan 10
    Switch(config-if)# exit

! Port pour PC2 → VLAN20
    Switch(config)# interface f0/3
    Switch(config-if)# switchport mode access
    Switch(config-if)# switchport access vlan 20
    Switch(config-if)# exit

    Switch(config)# interface f0/24
    Switch(config-if)# switchport mode trunk // switch to router
    Switch(config-if)# switchport trunk allowed vlan 10,20 // only vlan 10 and 20 is allowed  ,All other VLANs are blocked on this link
    Switch(config-if)# exit