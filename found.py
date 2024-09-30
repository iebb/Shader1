import sys
import requests
import hashlib

s, filename, serial, count, total_seconds = sys.argv

rate = '%.3f MH/s' % (int(count) / int(total_seconds) / 1e6)
print("calling python: serial number", serial)
print("calling python: rate", rate)
print("calling python: hash", hashlib.sha1(open(sys.argv[1], "rb").read()).hexdigest())

