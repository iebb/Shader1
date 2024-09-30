import sys

s, filename, serial, count, total_seconds = sys.argv

print("calling python: serial number", serial)
print("calling python: %.3f MH/s", int(count) / int(total_seconds) / 1e6)

