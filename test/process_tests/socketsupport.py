import tempfile
import sys
import random
import subprocess
import os
import time
import datetime
import psutil

from testsupport import (
    run_project_executable,
    info,
    warn,
    find_project_executable,
    test_root,
    project_root,
    run,
    ensure_library,
)

def is_server_listening(port: int) -> bool:
    for conn in psutil.net_connections():
        if conn.laddr.port == port and conn.status == "LISTEN":
            return True
    return False

def run_client(num_threads: int, port: int, messages: int):
    info(
        f"Running client with {num_threads} threads and {messages} messages."
    )
    with tempfile.TemporaryFile(mode="w+") as stdout:
        try:
            run_project_executable(
                "client",
                args=[
                    "-s", "127.0.0.1:"+str(port),
                    "-c", str(num_threads),
                    "-m", str(messages),
                ],
                stdout=stdout,
            ) 
        except subprocess.CalledProcessError as e:
            warn(f"{e}")
            return False
        return True

def run_verifier(port: int, messages: int):
    info(
        f"Running verifier for {messages} messages."
    )
    with tempfile.TemporaryFile(mode="w+") as stdout:
        try:
            run_project_executable(
                "verifier",
                args=[
                    "-s", "127.0.0.1:"+str(port),
                    "-m", str(messages),
                ],
                stdout=stdout,
            )
        except subprocess.CalledProcessError as e:
            warn(f"{e}")
            return False
        return True

def test_server(
    num_client_threads: int,
    messages: int,
    timed: bool
) -> None:
    try:
        # make sure port 31000 is not binded anymore
        while is_server_listening(31000):
            info("Waiting for port to be unbound")
            time.sleep(5)
        
        server = find_project_executable("kvs")
        info(f"Running server executable: {server}")

        with subprocess.Popen(
            server,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        ) as proc:
            # Wait for server init
            timeout = time.time() + 60*1 # 1 minute timeout
            while not is_server_listening(31000):
                time.sleep(0.1)
                if time.time() > timeout:
                    warn(f"Server init timed-out")
                    sys.exit(1)
            info("Server is listening at port 31000")
            
            if timed:
                start = datetime.datetime.now()

            if not (run_client(num_client_threads, 31000, messages)):
                proc.terminate()
                sys.exit(1)

            if timed:
                end = datetime.datetime.now()
                diff = end - start
            
            info(f"Client finished")
            
            if not (run_verifier(31000, messages)):
                proc.terminate()
                sys.exit(1)
            
            info(f"Verifier finished")
            
            proc.terminate()
            stdout, stderr = proc.communicate()
            

        info("OK")

    except OSError as e:
        warn(f"Failed to run command: {e}")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        warn(f"{e}")
        sys.exit(1)
    finally:
        if timed:
            return diff