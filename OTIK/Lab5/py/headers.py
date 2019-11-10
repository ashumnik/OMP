import pickle

class FileHeader:
    def __init__(self, data):
        self.original_size = len(data)
        self.byte_freq = FileHeader.count_freq(data, self.original_size)

    @staticmethod
    def count_freq(data, size):
        frequencies = {}
        for byte in data:
            if byte in frequencies:
                frequencies[byte] += 1
            else:
                frequencies[byte] = 1
        for byte in frequencies:
            frequencies[byte] /= size
        return frequencies


class ArchiveHeader:
    magic = int('0xBA0BAB',16)

    def __init__(self, data, filename):
        self.magic              = int('0xBA0BAB',16)
        self.offset_to_n_header = 0
        self.file_header        = FileHeader(data)
        self.filename           = filename
        self.compressed_data    = None

    def verify(self):
        return self.magic == ArchiveHeader.magic

    def dump(self, filename):
        self.offset_to_n_header = len(pickle.dumps(self))
        with open(filename, 'ab') as archive:
            pickle.dump(self, archive)



def process_raw_file(filename):
    with open(filename, 'rb') as file:
        byte_list = []
        b = file.read(1)
        while b:
            byte_list.append(b)
            b = file.read(1)
    return (ArchiveHeader(byte_list, filename), byte_list)

def process_archive(filename):
    a_header = None
    with open(filename, 'rb') as file:
        a_header = pickle.load(file)
    return a_header
