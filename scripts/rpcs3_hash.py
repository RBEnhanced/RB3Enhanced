import sys
import math
import hashlib
from elftools.elf.elffile import ELFFile

def cmod(a, b):
    return abs(a)%abs(b)*(1-2*(a<0))

# referencing RPCS3(Nekotekina): https://github.com/RPCS3/rpcs3/blob/master/Utilities/StrFmt.cpp#L127
def base57(in_data):
    # Precomputed tail sizes if input data is not a multiple of 8
    tails = [0, 2, 3, 5, 6, 7, 9, 10]
    alphabet = "0123456789ACEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
    in_size = len(in_data)
    # Get full output size
    out_size = int(in_size / 8) * 11 + tails[in_size % 8]
    out_string = bytearray(int(out_size))
    # Each 8 bytes of input data produces 11 bytes of base57 output
    for i in range(0, math.ceil(out_size / 11)):
        data_off = i * 8
        p = i * 11 # output offset
        # If the remaining data is less than 8 bytes, pad it with zeroes
        if in_size - data_off < 8:
            value_be = in_data[data_off : data_off + (in_size - data_off)] + bytearray(in_size - data_off)
        else:
            value_be = in_data[data_off : data_off + 8]
        # Convert from a big endian integer
        value = int.from_bytes(value_be, byteorder='big')
        for j in range (10, -1, -1):
            if p + j < out_size:
                out_string[p + j] = ord(alphabet[cmod(value, 57)])
            value //= 57
    return out_string.decode("utf-8")


def generate_hash(filename):
    with open(filename, 'rb') as f:
        elffile = ELFFile(f)
        h = hashlib.sha1()
        for seg in elffile.iter_segments():
            if seg["p_type"] == "PT_LOAD":
                h.update((1).to_bytes(4, byteorder='big'))
            else:
                h.update(seg["p_type"].to_bytes(4, byteorder='big'))
            h.update(seg["p_flags"].to_bytes(4, byteorder='big'))

            if seg["p_type"] == "PT_LOAD":
                h.update(seg["p_vaddr"].to_bytes(8, byteorder='big'))
                h.update(seg["p_memsz"].to_bytes(8, byteorder='big'))
                h.update(seg.data())
        return base57(h.digest())
        

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("rpcs3_hash.py input.prx output.txt")
    else:
        with open(sys.argv[2], 'w', encoding='utf8') as f:
            f.write("PRX-")
            f.write(generate_hash(sys.argv[1]))
            f.write("-0")
