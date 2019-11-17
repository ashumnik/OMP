import re
import gmpy2
from mpi4py import MPI

top_value  = 10
multiplier = 1
multiplicand_count = 8 # should be equal to process number
random_state = gmpy2.random_state()
multiplicand_list = []
output_string = '{}'

chunk_size = 2
comm = MPI.COMM_WORLD.Create_cart([1,1,multiplicand_count/2 + 1])
rank = None
try:
    rank = comm.Get_rank()
    print('rank', rank)

    #chunk_size = int(multiplicand_count/comm.size)

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
        for m in multiplicand_list[0:2]:
            print(m)
            partitial_result = gmpy2.mul(partitial_result, m)
        print('=', partitial_result)
        comm.send(partitial_result, dest=last_proc_rank)
        print('Rank 0 sended')
    elif rank < last_proc_rank:
        for m in multiplicand_list[rank+1:rank+chunk_size]:
            print(m)
            partitial_result = gmpy2.mul(partitial_result, m)
        print('=', partitial_result)
        comm.send(partitial_result, dest=last_proc_rank)
        print('Rank {} sended'.format(rank))
    elif rank == last_proc_rank:
        print('Summary')
        for i in range(0, last_proc_rank):
            partitial_result = comm.recv()
            print('partitial_result',partitial_result)
            result = gmpy2.mul(result, partitial_result)
            print('result          ', result)
        output_string = output_string.format(result)
        output_string = re.sub(r'(\d+ \* \d+)',r'(\1)', output_string)
        output_string = re.sub(r'\(+',r'(', output_string)
        output_string = re.sub(r'\)+',r')', output_string)
        print(output_string)
except:
    pass
