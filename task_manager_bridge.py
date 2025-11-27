import subprocess
import os

# Resolve the console C++ executable path relative to this script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CPP_EXE = os.path.join(SCRIPT_DIR, "Source_Code.exe")

def _run_sequence(lines, timeout=15):
    if not os.path.exists(CPP_EXE):
        return f"ERROR: {CPP_EXE} not found. Build Source_Code.cpp first."

    payload = "\n".join(lines) + "\n"
    try:
        result = subprocess.run(
            [CPP_EXE, "--api"],
            cwd=SCRIPT_DIR,
            input=payload,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
        out = result.stdout.strip()
        err = result.stderr.strip()
        return out if out else (err or "")
    except subprocess.TimeoutExpired:
        return "ERROR: Operation timed out."


def run_cpp_add_task(task, priority):
    # API mode: ADD then task then priority
    return _run_sequence(["ADD", str(task), str(priority)]) 

def run_cpp_display():
    # API mode: DISPLAY
    return _run_sequence(["DISPLAY"]) 

def run_cpp_run_task():
    # Simple (non-streaming) run; kept for fallback
    return _run_sequence(["RUN"]) 

def run_cpp_run_task_stream(progress_callback, timeout=30):
    """Stream RUN command output line by line to progress_callback.
    Shows the initial 'Running....' immediately, then final status when process ends.
    """
    if not os.path.exists(CPP_EXE):
        progress_callback(f"ERROR: {CPP_EXE} not found. Build Source_Code.cpp first.")
        return
    try:
        proc = subprocess.Popen(
            [CPP_EXE, "--api"],
            cwd=SCRIPT_DIR,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )
        # Send command then close stdin
        proc.stdin.write("RUN\n")
        proc.stdin.flush()
        proc.stdin.close()
        # Read lines as they arrive
        for line in proc.stdout:
            progress_callback(line.rstrip())
        proc.wait(timeout=timeout)
        err = proc.stderr.read().strip()
        if err:
            progress_callback(err)
    except subprocess.TimeoutExpired:
        progress_callback("ERROR: Operation timed out.")
    except Exception as e:
        progress_callback(f"ERROR: {e}")

def run_cpp_open_log():
    # API mode: OPENLOG (spawns OS to open file)
    return _run_sequence(["OPENLOG"]) 