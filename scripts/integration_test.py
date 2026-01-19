import subprocess
import time
import os
import hashlib
import sys

# Change to project root
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))

# Paths
if os.name == 'nt':
    TRACKER_EXE = "build/bin/tracker.exe"
    DAEMON_EXE = "build/bin/peer_daemon.exe"
    CMD_EXE = "build/bin/send_cmd.exe"
else:
    TRACKER_EXE = "build/bin/tracker"
    DAEMON_EXE = "build/bin/peer_daemon"
    CMD_EXE = "build/bin/send_cmd"

def create_test_file(filename, size_mb):
    with open(filename, 'wb') as f:
        f.write(os.urandom(size_mb * 1024 * 1024))
    print(f"Created {filename} ({size_mb} MB)")

def get_file_hash(filename):
    sha = hashlib.sha256()
    with open(filename, 'rb') as f:
        while True:
            data = f.read(65536)
            if not data: break
            sha.update(data)
    return sha.hexdigest()

def send_cmd(port, *args):
    cmd = [CMD_EXE, str(port)] + list(args)
    res = subprocess.run(cmd, capture_output=True, text=True)
    return res.stdout.strip()

def run_process_bg(cmd):
    return subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def main():
    if not os.path.exists("build/bin"):
        print("Please run scripts/build.sh first!")
        sys.exit(1)

    print("--- Starting Integration Test (Client-Server) ---")
    
    if os.path.exists("test_out.bin"): os.remove("test_out.bin")
    if os.path.exists("downloaded.bin"): os.remove("downloaded.bin")

    # 1. Create Data
    SRC_FILE = "test_data.bin"
    create_test_file(SRC_FILE, 2)
    EXPECTED_HASH = get_file_hash(SRC_FILE)
    print(f"Source Hash: {EXPECTED_HASH}")

    # 2. Start Tracker
    print("Starting Tracker...")
    tracker = run_process_bg([TRACKER_EXE])
    time.sleep(1)

    seeder_daemon = None
    leech_daemon = None

    try:
        # 3. Start Seeder Daemon (P2P: 9001, Control: 9991)
        print("Starting Seeder Daemon...")
        seeder_daemon = run_process_bg([DAEMON_EXE, "9001", "9991"])
        time.sleep(1)
        
        send_cmd(9991, "tracker", "127.0.0.1", "8080")
        resp = send_cmd(9991, "seed", SRC_FILE)
        print(f"Seeder Response: {resp}")
        time.sleep(1)

        # 4. Start Leecher Daemon (P2P: 9002, Control: 9992)
        print("Starting Leecher Daemon...")
        leech_daemon = run_process_bg([DAEMON_EXE, "9002", "9992"])
        time.sleep(1)
        
        send_cmd(9992, "tracker", "127.0.0.1", "8080")
        resp = send_cmd(9992, "download", EXPECTED_HASH, "downloaded.bin")
        print(f"Leecher Response: {resp}")

        # Wait for download
        print("Waiting for download...")
        start_time = time.time()
        success = False
        while time.time() - start_time < 30:
            if os.path.exists("downloaded.bin"):
                if os.path.getsize("downloaded.bin") == os.path.getsize(SRC_FILE):
                     success = True
                     break
            time.sleep(0.5)
            
        if not success:
            print("Timeout waiting for file!")
            sys.exit(1)
            
        OUTPUT_HASH = get_file_hash("downloaded.bin")
        if OUTPUT_HASH == EXPECTED_HASH:
            print("SUCCESS: Hashes match!")
        else:
            print("FAILURE: Hash mismatch!")
            sys.exit(1)

    finally:
        print("Cleaning up processes...")
        if tracker: tracker.terminate()
        if seeder_daemon: seeder_daemon.terminate()
        if leech_daemon: leech_daemon.terminate()

if __name__ == "__main__":
    main()
