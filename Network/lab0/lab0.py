#Jongbin Baek 
import sys

ip_input = sys.argv[1] #223.1.1
mask_input = sys.argv[2][1:] #/24->24
temp = [] 
result = []
i = 0

ip = ip_input.split('.') # get ip without .

#if ip_address \24    
if (mask_input == '24'):
    netMask = '255.255.255' #subnet
    print("will print out:\n")
    for subnet in netMask.split('.'):
        temp = str(int(subnet) & int(ip[i])) #ip_address AND subnet
        result.append(temp)
        i += 1
    netWork = '.'.join(result) 
    for i in range(0, 10):
        print("{}.{} ".format(netWork, i), end = '')
    print("\n")
    for i in range(246, 256):
        print("{}.{} ".format(netWork, i), end = '')
    print("\nNumber of addresses =", i+1)

#if ip_address \16
if (mask_input == '16'):
    netMask = '255.255'
    print("will print out:\n")
    for subnet in netMask.split('.'):
        temp = str(int(subnet) & int(ip[i])) #ip_address AND subnet
        result.append(temp)
        i += 1
    netWork = '.'.join(result)
    for i in range(0, 10):
        print("{}.0.{} ".format(netWork, i), end = '')
    print("\n")
    for j in range(246, 256):
        print("{}.255.{} ".format(netWork, j), end = '')
    print("\nNumber of addresses =", (j+1)*(j+1))

