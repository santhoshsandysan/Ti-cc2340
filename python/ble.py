import asyncio
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime
from bleak import BleakClient, BleakScanner

DEVICE_NAME = "Yantra Temp"
SERVICE_UUID = "0000fff0-0000-1000-8000-00805f9b34fb"

# Define characteristic UUIDs with labels
CHAR_UUIDS = {
    "CHAR1": "0000fff1-0000-1000-8000-00805f9b34fb",  # low byte
    "CHAR2": "0000fff2-0000-1000-8000-00805f9b34fb",  # high byte (preferred)
    "CHAR3": "0000fff3-0000-1000-8000-00805f9b34fb"   # high byte (fallback)
}

# Store data for plotting
time_data = []
temp_data = []


async def main():
    print(f"🔍 Scanning for '{DEVICE_NAME}' ...")
    devices = await BleakScanner.discover()

    target_address = None
    for d in devices:
        if d.name == DEVICE_NAME:
            target_address = d.address
            break

    if not target_address:
        print(f"❌ Device '{DEVICE_NAME}' not found.")
        return

    print(f"🔗 Connecting to {DEVICE_NAME} [{target_address}] ...")

    async with BleakClient(target_address) as client:
        if not client.is_connected:
            print("❌ Failed to connect.")
            return

        print("✅ Connected! Saving graph as image...\n")

        try:
            while True:
                try:
                    # Read CHAR1 (low byte)
                    low_val = await client.read_gatt_char(CHAR_UUIDS["CHAR1"])
                    low_val = low_val[0]

                    # Try CHAR2 first (high byte)
                    try:
                        high_val = await client.read_gatt_char(CHAR_UUIDS["CHAR2"])
                        high_val = high_val[0]
                    except Exception:
                        # fallback to CHAR3
                        high_val = await client.read_gatt_char(CHAR_UUIDS["CHAR3"])
                        high_val = high_val[0]

                    # Reconstruct float (scaled by 100 in firmware)
                    temp_scaled = (high_val << 8) | low_val
                    temperature = temp_scaled / 100.0

                    # Log
                    print(f"🌡️ Temperature: {temperature:.2f} °C")

                    # Append to graph data
                    now = datetime.now()
                    time_data.append(now)
                    temp_data.append(temperature)

                    # Update and save plot as image
                    plt.figure(figsize=(9, 5))
                    plt.plot(time_data, temp_data, label="Temperature", color="red")

                    plt.xlabel("Time")
                    plt.ylabel("Temperature (°C)")
                    plt.title("Temperature over Time")
                    plt.legend()

                    # Format x-axis as time
                    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter("%H:%M:%S"))
                    plt.gcf().autofmt_xdate()  # rotate time labels

                    plt.tight_layout()
                    plt.savefig("temperature_graph.png")
                    plt.close()

                except Exception as e:
                    print(f"⚠️ Read error: {e}")

                await asyncio.sleep(1)  # sample every 1s

        except Exception as e:
            print(f"⚠️ Loop error: {e}")


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\n⏹️ Stopped by user")
