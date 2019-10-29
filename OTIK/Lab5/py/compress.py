import headers
import ctypes as c

RANGE = 30

def around(a,b):
    if a - RANGE < b or a + RANGE > b:
        return True
    else:
        return False

class FileOp:
    def __init__(self, file_header):
        self.file_header = file_header

    def cum_freq(self, until_b, include=True):
        sorted_freq = [(b, self.file_header.byte_freq[b]) for k in sorted(self.file_header.byte_freq, key=self.file_header.byte_freq.get, reverse=True)]
        sum = 0

        for b,f in sorted_freq:
            if b == until_b:
                if include:
                    sum += f
                break
            sum += f

        return sum

    def compress(self, data):
        l = 0
        h = 2**64-1
        block_size = 64
        encoded = []
        number_repr_block = 0

        i = 0
        for b in data:
            r = h - l + 1
            h = l + (r*self.cum_freq(b))/block_size
            l = l + (r*self.cum_freq(b,include=False))/block_size
            number_repr_block = (l+h)/2
            if i % block_size == 0:
                encoded.append(number_repr_block)
            i += 1
        if len(encoded) == 0:
            encoded.append(number_repr_block)
        return encoded

    def decompress(self, n_list):
        l = 0
        h = 2**64-1
        block_size = 64
        decoded = []
        number_repr_block = 0

        for n in n_list:
            for b in range(0,255):
                r = h - l + 1
                h = l + (r*self.cum_freq(b))/block_size
                l = l + (r*self.cum_freq(b,include=False))/block_size
                number_repr_block = (l+h)/2
                if around(number_repr_block, n):
                    decoded.append(b)
        return decoded
