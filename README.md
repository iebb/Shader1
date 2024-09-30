## shader1

Generate SHA-1.

Based on https://github.com/cuihaoleo/gpg-fingerprint-filter-gpu

### How to use:

1. Get CUDA and Python installed
2. use `make` or `build.sh` (for non-root env) to build `shader1`
3. `python3 [extract.py](extract.py) your_pfx.pfx password114514` to extract the DER file
4. run `./shader1 [pattern] [start_at] [stop_when]`
5. `python3 [assemble.py](assemble.py) your_pfx.pfx password114514 output.pfx [64-bit-hash] [key name]`

### Optional: 

Modify `found.py` to trigger something when a match is found.

### Pattern

- Only matches beginning part of a string.
- A hex digit means itself.
- Other Latin alphabets (`g` to `z`) are to match any hex digit.
- `{N}` to repeat previous digit or group for N times.
- `(PATTERN)` a group pattern.
- Use `|` to split multiple patterns.

Examples:

- `deadbeef` equals to regex `^deadbeef`
- `x{8}` equals to regex `^([0-9a-f])\1{7}$`
- `(xy){4}` equals to regex `^([0-9a-f][0-9a-f])\1{3}`
- `xxxxa{4}` equals to regex `^([0-9a-f])\1{3}aaaa`

### Start at and Stop when

`start_at`: a 32-bit number, `0-4294967295` to specify the starting lower-32bit
`stop_when`: stop when finding N matches, defaults to 1.

### For Baidu AI Studio

Use `build_baidu.sh` to build.

[Use this link](https://aistudio.baidu.com/aistudio/newbie?invitation=1&sharedUserId=13923715&sharedUserName=Nekoko14) 
to get 57 free hours of V100 credits.
