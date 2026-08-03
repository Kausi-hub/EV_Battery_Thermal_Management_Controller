import sqlite3

conn = sqlite3.connect("thermal.db")

cursor = conn.cursor()

cursor.execute("""
CREATE TABLE IF NOT EXISTS telemetry (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT,
    battery_temp REAL,
    coolant_temp REAL,
    pump_speed REAL,
    fan_speed REAL,
    fault_code INTEGER
)
""")

conn.commit()
conn.close()

print("Database created")