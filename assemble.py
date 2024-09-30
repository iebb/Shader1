import sys

from cryptography import x509
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives._serialization import BestAvailableEncryption
from cryptography.hazmat.primitives.serialization import pkcs12


base_file = sys.argv[1]
pfx_password = sys.argv[2].encode("ascii")
modified_file = sys.argv[3]

replacement = bytes.fromhex(sys.argv[4])
assert len(replacement) == 8

if len(sys.argv) > 5:
    friendly_name = sys.argv[5].encode("ascii")
else:
    friendly_name = "key"


with open(base_file, "rb") as pfx_file:
    pfx_data = pfx_file.read()

# Parse the PFX file
private_key, certificate, additional_certificates = pkcs12.load_key_and_certificates(
    pfx_data, pfx_password, backend=default_backend()
)

cert_bytes = certificate.public_bytes(encoding=serialization.Encoding.DER)
digest = hashes.Hash(hashes.SHA1(), backend=default_backend())
digest.update(cert_bytes)
print(f"SHA-1 hash of the old certificate: {digest.finalize().hex()}")

# replace bytes
mutable_bytes = bytearray(cert_bytes)
mutable_bytes[16:24] = replacement
certificate = x509.load_der_x509_certificate(bytes(mutable_bytes))
cert_bytes = certificate.public_bytes(encoding=serialization.Encoding.DER)
digest = hashes.Hash(hashes.SHA1(), backend=default_backend())
digest.update(cert_bytes)
print(f"SHA-1 hash of the new certificate: {digest.finalize().hex()}")

# Serialize the private key, certificate, and additional certificates into a PKCS#12 structure
pfx_data = pkcs12.serialize_key_and_certificates(
    name=friendly_name,                # The friendly name for the private key and cert
    key=private_key,                   # Private key (optional)
    cert=certificate,                  # Primary certificate (optional)
    cas=additional_certificates,       # Additional certificates (optional)
    encryption_algorithm=BestAvailableEncryption(pfx_password)  # Encryption with password
)

# Save the PKCS#12 data to a file
with open("output.pfx", "wb") as pfx_file:
    pfx_file.write(pfx_data)

print("PKCS#12 (PFX) file created successfully!")