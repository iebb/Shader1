import sys

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.serialization import pkcs12

base_file = sys.argv[1]
pfx_password = sys.argv[2].encode("ascii")

with open(base_file, "rb") as pfx_file:
    pfx_data = pfx_file.read()

private_key, certificate, additional_certificates = pkcs12.load_key_and_certificates(
    pfx_data, pfx_password, backend=default_backend()
)

cert_bytes = certificate.public_bytes(encoding=serialization.Encoding.DER)
open("derfile.bin", "wb+").write(cert_bytes)
print("PKCS#12 (PFX) file created successfully!")