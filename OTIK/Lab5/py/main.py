import argparse
import headers
import compress
import pickle


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-u' , '--unzip' , action='store_true')
    parser.add_argument('-z' , '--zip'   , action='store_true')
    parser.add_argument('-t' , '--test'  , action='store_true')
    parser.add_argument('-l' , '--list'  , action='store_true')
    parser.add_argument('-f' , '--files' , action='append')
    parser.add_argument('-o' , '--out'   , action='store')

    return parser.parse_args()

args = get_args()

process_files = []
if args.zip:
    for filename in args.files:
        header_and_data = headers.process_raw_file(filename)
        process_files.append(header_and_data)
    for pf in process_files:
        f_op = compress.FileOp(pf[0].file_header)
        pf[0].compressed_data = f_op.compress(pf[1])#[2:])
        pf[0].dump(args.out)

if args.unzip:
    for filename in args.files:
        arch_header = headers.process_archive(filename)
        f_op = compress.FileOp(arch_header.file_header)
        with open(arch_header.filename, 'wb') as out:
            out.write(b''.join(f_op.decompress(arch_header.compressed_data)))


if args.test:
    for filename in args.files:
        print(headers.process_archive(filename).verify())

if args.list:
    for filename in args.files:
        with open(filename, 'rb') as archive:
           while True:
               try:
                  print(pickle.load(archive).filename)
               except Exception:
                  break
