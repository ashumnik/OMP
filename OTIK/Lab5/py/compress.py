import headers
import pprint
from decimal import *
import functools

RANGE = 30

def around(a,b):
    if a - RANGE < b or a + RANGE > b:
        return True
    else:
        return False

class FileOp:
    def __init__(self, file_header):
        self.file_header = file_header
        self.sorted_freq = [(b, self.file_header.byte_freq[b]) for b in sorted(self.file_header.byte_freq, key=self.file_header.byte_freq.get, reverse=True)]

    @functools.lru_cache(maxsize=256*2)
    def cum_freq(self, until_b, include=True):
        sum = 0

        for b, f in self.sorted_freq:
            if b == until_b:
                if include:
                    sum += f
                break
            sum += f

        return sum

    def compress(self, data):
        getcontext().prec = 99999999
        l = Decimal(0)
        h = Decimal(1)
        encoded = None

        for b in data:
            r = h - l
            h = l + r*Decimal(self.cum_freq(b))
            l = l + r*Decimal(self.cum_freq(b,include=False))
        number_repr_block = (l+h)/2
        encoded = number_repr_block
        return encoded

    def decompress(self, number):
        #number = '0.'+str(int(number))
        #number = Decimal(number)
        orig_number = number

        getcontext().prec = 99999999
        l = Decimal(0)
        h = Decimal(1)
        decoded = []

        for i in range(0,self.file_header.original_size):
            byte = None
            real_h = Decimal(h)
            real_l = Decimal(l)
            for b in range(0,255):
                b = bytes([b])
                r = h - l
                h = l + r*Decimal(self.cum_freq(b))
                l = l + r*Decimal(self.cum_freq(b,include=False))
                byte = b

                if l <= number and number < h:
                    break
                else:
                    h = real_h
                    l = real_l
            decoded.append(byte)
        return decoded
