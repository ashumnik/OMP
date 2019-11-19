import re
import gmpy2
from mpi4py import MPI

top_value  = 10         # change to get higher values
multiplier = 1          # change to get higher value
multiplicand_count = 16 # minimally should be equal to half of that number + 1
random_state = gmpy2.random_state()
multiplicand_list = []
output_string = '{}'

chunk_size = int(multiplicand_count/MPI.COMM_WORLD.size)+1
rank = MPI.COMM_WORLD.Get_rank()
if rank == 0:
    print('chunk size is ', chunk_size)

comm = MPI.COMM_WORLD.Create_cart([1,1,multiplicand_count/2 + 1])
try:
    rank = comm.Get_rank()

    for i in range(0, multiplicand_count):
        multiplicand_list.append(1+gmpy2.mpz_random(random_state, top_value)*multiplier)
        output_string = output_string.format(multiplicand_list[-1])
        if i < multiplicand_count - 1: 
            output_string += ' * {}'

    output_string = output_string.format(multiplicand_list[-1])
    output_string += ' = {}'

    partitial_result = 1
    result = 1
    last_proc_rank = comm.size-1

    if rank == 0:
        for m in multiplicand_list[0:chunk_size]:
            partitial_result = gmpy2.mul(partitial_result, m)
        comm.send(partitial_result, dest=last_proc_rank)
    elif rank < last_proc_rank:
        l = rank*chunk_size
        r = rank*chunk_size+chunk_size
        for m in multiplicand_list[l:r]:
            partitial_result = gmpy2.mul(partitial_result, m)
        comm.send(partitial_result, dest=last_proc_rank)
    elif rank == last_proc_rank:
        for i in range(0, last_proc_rank):
            partitial_result = comm.recv()
            result = gmpy2.mul(result, partitial_result)
        output_string = output_string.format(result)
        output_string = re.sub(r'(\d+ \* \d+)',r'(\1)', output_string)
        output_string = re.sub(r'\(+',r'(', output_string)
        output_string = re.sub(r'\)+',r')', output_string)
        print(output_string)
except:
    pass
