import pickle

class FileHeader:
    def __init__(self, data):
        self.byte_freq = FileHeader.count_freq(data)
        self.original_size = len(data)

    @staticmethod
    def count_freq(data):
        frequencies = {}
        for byte in data:
            if byte in frequencies:
                frequencies[byte] += 1
            else:
                frequencies[byte] = 1
        return frequencies


class ArchiveHeader:
    def __init__(self, data, filename):
        self.magic              = int('0xBA0BAB',16)
        self.offset_to_n_header = 0
        self.file_header        = FileHeader(data)
        self.filename           = filename
        self.compressed_data    = None
