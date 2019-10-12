from pymongo import MongoClient


class DB_Connection:
    instance = None
    last_db_name = None
    last_collection_name = None

    def __init__(self):
        if DB_Connection.instance is None:
            DB_Connection.instance = MongoClient()


    def get_db(self, db_name):
        DB_Connection.last_db_name = db_name;
        return DB_Connection.instance[db_name]

    def get_collection(self, collection_name, db_name='shell'):
        DB_Connection.last_collection_name = collection_name
        return DB_Connection.instance[db_name][collection_name]
    
    def insert_element(self, element, collection_name=None, db_name='shell'):
        if collection_name is None:
            collection_name = DB_Connection.last_collection_name

        return DB_Connection.instance[db_name][collection_name].insert_one(element)

    def list_collection_names(self, db_name='shell'):

        return DB_Connection.instance[db_name].list_collection_names()

