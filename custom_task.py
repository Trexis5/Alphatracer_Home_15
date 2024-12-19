Import("env")
import os
import subprocess
import time

def is_serial_monitor_running():
    try:
        result = subprocess.run(["pio", "device", "monitor", "--status"], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return "open" in result.stdout.decode()
    except subprocess.CalledProcessError as e:
        return False

def is_monitor_task_running():
    try:
        result = subprocess.run(["pio", "device", "monitor", "--status"], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return "open" in result.stdout.decode()
    except subprocess.CalledProcessError as e:
        return False

def custom_upload(source, target, env):
    # Serial Monitor schließen, wenn er läuft
    if is_serial_monitor_running():
        try:
            subprocess.run(["pio", "device", "monitor", "--baud", "115200", "--exit"], check=True)
        except subprocess.CalledProcessError as e:
            print("Serial Monitor konnte nicht geschlossen werden:", e)

    # Monitor Task schließen, wenn er läuft
    if is_monitor_task_running():
        try:
            env.Execute("taskkill /IM monitor.exe /F")
        except Exception as e:
            print("Monitor Task konnte nicht geschlossen werden:", e)


    # Lösche den Flash
    try:
        env.Execute("python -m esptool --port COM14 erase_flash") # Port ggf. anpassen
    except Exception as e:
        print("Fehler beim Loeschen des Flash:", e)

    # Warte auf den Abschluss des Befehls (für Windows `timeout` verwenden)
    time.sleep(2)

    # Lade das Dateisystem hoch
    try:
        env.Execute("pio run --target uploadfs")
    except Exception as e:
        print("Fehler beim Hochladen des Dateisystems:", e)

    # Lade die Hauptfirmware hoch
    try:
        env.Execute("pio run --target upload")
    except Exception as e:
        print("Fehler beim Hochladen der Firmware:", e)

    # Monitor Task starten
    try:
        subprocess.run(["pio", "device", "monitor", "--baud", "115200"])
    except subprocess.CalledProcessError as e:
        print("Monitor Task konnte nicht gestartet werden:", e)

env.AddCustomTarget("custom_upload", None, custom_upload, "Custom Upload Process")

