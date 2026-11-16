import subprocess
import os
import time
import shutil


EXE_PATH = (
    "../build/thermal_controller.exe"
)

TELEMETRY_FILE = (
    "..build/Debug/telemetry.csv"
)


def run_simulation():

    # Remove old telemetry
    if os.path.exists(TELEMETRY_FILE):
        os.remove(TELEMETRY_FILE)


    process = subprocess.run(
        [
            EXE_PATH
        ],
        capture_output=True,
        text=True
    )


    if process.returncode != 0:

        return {
            "status": "FAILED",
            "error": process.stderr
        }


    if os.path.exists(TELEMETRY_FILE):

        return {
            "status":"COMPLETED",
            "file": TELEMETRY_FILE
        }


    return {
        "status":"FAILED",
        "error":"Telemetry file not generated"
    }