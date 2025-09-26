import usb.core
import usb.util

# Replace with the VID/PID for your CC2351 dongle
VID = 0x0451
PID = 0x0949

# Find the device
dev = usb.core.find(idVendor=VID, idProduct=PID)

if dev is None:
    raise ValueError("CC2351 device not found")
print("Device found!")

# Set the active configuration
dev.set_configuration()

# Find endpoint
cfg = dev.get_active_configuration()
intf = cfg[(0, 0)]

# Usually CC2351 Packet Sniffer HID device has a single IN endpoint
ep = usb.util.find_descriptor(
    intf,
    custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN
)

if ep is None:
    raise ValueError("Endpoint not found")
print(f"Endpoint found: {ep.bEndpointAddress}")

# Read packets in a loop
try:
    while True:
        data = dev.read(ep.bEndpointAddress, ep.wMaxPacketSize, timeout=1000)
        print("Raw packet:", data)
except usb.core.USBError as e:
    if e.errno == 110:  # timeout
        pass
