import pytest
import subprocess
import os
import time
import signal
import tempfile


@pytest.fixture(scope="session", autouse=True)
def test_server():
    """Compile and start a local test server for the duration of the test session."""
    src_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), "src")
    server_bin = os.path.join(tempfile.gettempdir(), "test_server_py")

    # Compile server with localhost IP
    compile_cmd = [
        "cc", "-o", server_bin,
        os.path.join(src_dir, "server.c"),
        os.path.join(src_dir, "c_common.c"),
        f"-I{src_dir}",
        "-USERVER_IP",
        '-DSERVER_IP="127.0.0.1"',
    ]
    result = subprocess.run(compile_cmd, capture_output=True, text=True)
    if result.returncode != 0:
        pytest.skip(f"Could not compile test server (rc={result.returncode}): stdout={result.stdout} stderr={result.stderr}")

    # Start server
    proc = subprocess.Popen([server_bin], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.5)

    # Point client at localhost
    os.environ["PEERCHAT_SERVER_IP"] = "127.0.0.1"

    yield proc

    # Teardown
    proc.send_signal(signal.SIGTERM)
    proc.wait(timeout=5)
    os.environ.pop("PEERCHAT_SERVER_IP", None)
    os.unlink(server_bin)
