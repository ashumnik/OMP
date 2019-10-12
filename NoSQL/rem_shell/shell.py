#!/usr/bin/python3.7
import subprocess
import re
import time
import sys
import datetime
import db_comm as db

print("{:^60}".format("Welcome to shell, that remembers everything"))
print("{:^60}".format('type "#help" to get help'))


database = db.DB_Connection()
database.get_db('shell')

while True:
    command = input('>')
    if command == '':
        continue
    execution_date = datetime.datetime.now()
    if command == 'quit' or command == 'exit' or command == 'q':
        sys.exit("exiting...")
    db_query_match = re.search(r'#get from (\*$|.+?)~(elapsed time|elapsed|stdout|stderr|std|date|all|avg)?\s*', command)
    if re.match(r'#help\s*', command ):
        print('type "#get from *command*~elapsed time" to get elapsed time of executed command')
        print('type "#get from *command*~elapsed"      to get elapsed time of executed command')
        print('type "#get from *command*~stdout"       to get stdout of executed command')
        print('type "#get from *command*~stderr"       to get stderr of executed command')
        print('type "#get from *command*~std"          to get both stdout and stderr of executed command')
        print('type "#get from *command*~date"         to get date when command was executed')
        print('type "#get from *command*~all"          to get all information about executed command')
        print('type "#get from *command*~avg"          to get avgerage elapsed time of executed command')
        print('type "#get from *~"                     to get all executed commands in database')
        continue

    #import pdb; pdb.set_trace()
    if db_query_match:
        from_collection = db_query_match.group(1)
        field = db_query_match.group(2)

        collection = None
        if from_collection == '*':
            collections = database.list_collection_names()
            for collection in collections:
                print(collection)
            continue
        else:
            collection = database.get_collection(from_collection)


        sum = 0
        cnt = 0
        for doc in collection.find():
            cnt += 1
            sum += doc['info']['elapsed_time']
            if field == 'elapsed time' or field == 'elapsed':
                # query elapsed time
                print('{:-^60}'.format('elapsed time'))
                print(doc['info']['elapsed_time'])
                pass
            elif field == 'stdout':
                # query stdout
                print('{:-^60}'.format('stdout'))
                print(doc['info']['stdout'])
                pass
            elif field == 'stderr':
                # query stdout
                print('{:-^60}'.format('stderr'))
                print(doc['info']['stderr'])
                pass
            elif field == 'std':
                # query both stdout and stderr
                print('{:-^60}'.format('stdout'))
                print(doc['info']['stdout'])

                print('{:-^60}'.format('stderr'))
                print(doc['info']['stderr'])
                pass
            elif field == 'date':
                # query date
                print('{:-^60}'.format('execution_date'))
                print(doc['info']['execution_date'])
                pass
            elif field == 'all':
                # query all
                print('{:-^60}'.format('elapsed time'))
                print(doc['info']['elapsed_time'])
                print('{:-^60}'.format('stdout'))
                print(doc['info']['stdout'])

                print('{:-^60}'.format('stderr'))
                print(doc['info']['stderr'])
                print('{:-^60}'.format('execution_date'))
                print(doc['info']['execution_date'])
                pass
        if field == 'avg':
            print('{:-^60}'.format('average time'))
            print(sum/cnt)
            
        continue

    time_start = time.time()
    p          = subprocess.Popen(command, shell=True, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
    time_end   = time.time()

    stdout, stderr = p.communicate()
    stdout = stdout.decode("utf-8")
    stderr = stderr.decode("utf-8")

    print('{:-^60}'.format('stdout'))
    print(stdout)

    print('{:-^60}'.format('stderr'))
    print(stderr)

    elapsed_time = time_end - time_start
    database.get_collection(command)
    database.insert_element({'command' : command, 
                             'info' : {
                                       'elapsed_time': elapsed_time,
                                       'stdout' : stdout,
                                       'stderr' : stderr,
                                       'execution_date' : execution_date
                                       }
                            })


    
