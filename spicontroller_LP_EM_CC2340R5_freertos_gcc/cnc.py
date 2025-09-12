import socket
import struct

HOST = "192.168.2.110"   # CNC IP
PORT = 8193             # FOCAS2/Ethernet port

def send_packet(sock, req_id, func, handle, payload=b""):
    """
    Build and send a FOCAS packet.
    """
    total_len = 14 + len(payload)   # header + payload
    header = struct.pack("<IHHHH", total_len, req_id, func, handle, 0)
    sock.sendall(header + payload)

    # read response
    resp = sock.recv(4096)
    return resp

def parse_response(resp):
    """
    Parse FOCAS response header
    """
    total_len, req_id, func, handle, ret_code = struct.unpack("<IHHHH", resp[:14])
    payload = resp[14:total_len]
    return total_len, req_id, func, handle, ret_code, payload

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    # 1) Handshake: cnc_allclibhndl3
    # This packet format must match what the CNC expects.
    # Payload = (IP string, port, timeout) but exact encoding needs capture.
    payload = b"\x00" * 16   # placeholder
    send_packet(s, req_id=1, func=0x0101, handle=0, payload=payload)
    resp = s.recv(1024)
    print("Handshake response:", resp)

    # Extract handle from handshake response (usually 2 bytes in payload)
    # Here we fake it for demo:
    cnc_handle = 1

    # 2) Read spindle load: cnc_rdspmeter (func = 0x0124)
    spindle_no = 0   # main spindle
    payload = struct.pack("<H", spindle_no)
    send_packet(s, req_id=2, func=0x0124, handle=cnc_handle, payload=payload)
    resp = s.recv(1024)

    total_len, req_id, func, handle, ret_code, payload = parse_response(resp)
    if ret_code == 0:
        spindle_load = struct.unpack("<H", payload[:2])[0]
        print("Spindle Load:", spindle_load, "%")
    else:
        print("Error code:", ret_code)

    s.close()

if __name__ == "__main__":
    main()
