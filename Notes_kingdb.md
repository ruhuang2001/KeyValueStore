# kingdb notes

<details>
<summary>Table of notes</summary>

- [Part 1 What are key-value stores and why implement one](#part-1-what-are-key-value-stores-and-why-implement-one)

- [Part 2 Using existing key-value stores as models](#part-2-using-existing-key-value-stores-as-models)

- [Part 3 Comparative Analysis of the Architectures of Kyoto Cabinet and LevelDB](#part-3-comparative-analysis-of-the-architectures-of-kyoto-cabinet-and-leveldb)

- [Part 4 API Design](#part-4-api-design)

  - [1. General principles for API design](#1-general-principles-for-api-design)
  
  - [2. Defining the functionalities for the public API of KingDB](#2-defining-the-functionalities-for-the-public-api-of-kingdb)
  
  - [3. Comparing the APIs of existing databases](#3-comparing-the-apis-of-existing-databases)

- [Part 5 Hash table implementations](#part-5-hash-table-implementations)
  - [2. Implementations](#2-implementations)

- [Part 6 Open-Addressing Hash Tables](#part-6-open-addressing-hash-tables)
  - [1. Open-addressing hash tables](#1-open-addressing-hash-tables)
  - [2. Comparing different algorithm](#2-comparing-different-algorithm)

- [Part 7: Optimizing Data Structures for SSDs](#part-7-optimizing-data-structures-for-ssds)
  - []()
  - []()
</details>


## Part 1 What are key-value stores and why implement one
Key-value stores generally share the following interface :
- Get(key)
- Set(key, value)
- Delete(key)

Its performance depends on :
- The hardware
- The file system being used
- The actual application and order in which the keys are being accessed ([locality of reference](http://en.wikipedia.org/wiki/Locality_of_reference))
- The data set

eg: [Redis](https://github.com/redis/redis), [MongoDB](https://github.com/mongodb/mongo), [LevelDB](https://github.com/google/leveldb)...

## Part 2 Using existing key-value stores as models
Refer to LevelDB
- It is based on a Log-Structured Merge Tree(LSM)

## Part 3 Comparative Analysis of the Architectures of Kyoto Cabinet and LevelDB

Components of a Key-Value Store :
- Interface: minimum API--Get(), Put(), Delete()
- Parametrization
- Data Storage: used to access the memory where the data
- Data Structure: generally a hash table or B+ Tree. LevelDB used a Log-Structured Merge Tree
- Memory Management
- Iteration: solutions are mostly Iterators and Cursors
- String
- Lock Management
- Error Management
- Logging
- Transaction Management: ensures that all the operations are executed correctly
- Compression: compress the data
- Comparators
- Checksum
- Snapshot
- Partitioning
- Replication
- Testing Framework

Doxygen is a powerful tool to navigate through the hierarchies of modules and classes of an application

LevelDB is using a class called "Slice"
- A Slice holds a byte array along with the size that array
- know the size of the string in time O(1)
- the Slice class handles the copy by making a shallow copy.It simply copies the pointer to the byte array

LevelDB is not using exceptions at all, but a special class called Status. This class holds both an error value and an error message

## Part 4 API Design
### 1. General principles for API design
1. When in doubt, leave it out
2. Don’t make the client do anything the library could do.

### 2. Defining the functionalities for the public API of KingDB

Basic Function
- Opening and closing a database
- Reading and writing data to a database
- Iterating over the full collection of keys and values in a database
- Offer a way to tune parameters
- Offer a decent error notification interface

### 3. Comparing the APIs of existing databases

***3.1 Opening and closing a database***

In an API, function calls should be symmetric as much as possible, because it is more intuitive and logical
-  "If I call open(), then I should call close()" is infinitely more logical than “If I call open(), then I should delete the pointer.”

**Decision** : Use the conventional open() and close()

***3.2 Reads and Writes***

the key is passed by value, and the value is passed as a pointer so it can be updated by the call. The values are not returned by the calls, the returns are for error management here

**Decision** : Use Get() and Set() like LevelDB
```
/* LevelDB */
std::string value;
db->Get(leveldb::ReadOptions(), "key1", &value);
db->Put(leveldb::WriteOptions(), "key2", value);
```

***3.3 Iteration***

Two ways of doing things here :
- Using a cursor or using an iterator
    - Cursor: Kyoto Cabinet and BerkeleyDB---the keys and values are passed as pointers
    - Iterator: LevelDB---the keys and values are accessed as the return values of methods of the iterator

**Decision** : Simply copy the method names from LevelDB

***3.4 Parametrization***

```
/* LevelDB */
leveldb::DB* db;
leveldb::Options options;
options.create_if_missing = true;
options.compression = leveldb::kNoCompression;
leveldb::DB::Open(options, "/tmp/testdb", &db);
...
leveldb::WriteOptions write_options;
write_options.sync = true;
db->Put(write_options, "key", "value");
```

**Decision** : Use the LevelDB's method

db.Put(leveldb::WriteOptions, "key", "value");

***3.5 Error management***

How errors are being reported to the user when they occur, as she uses the public interfaces

```
# LevelDB
leveldb::Status s = db->Put(leveldb::WriteOptions(), "key", "value");
if (!s.ok()) {
  cerr << s.ToString() << endl;
}
```

**Decision** : Use the LevelDB's method

## Part 5 Hash table implementations
### 2. Implementations

***2.1 unordered_map from TR1***

The bucket array is allocated on the heap, and scales up or down automatically based on the load factor of the hash table

```
/* from gcc-4.8.0/libstdc++-v3/include/tr1/hashtable_policy.h */ 
template
  struct _Hash_node<_Value, false>
  {
    _Value       _M_v;
    _Hash_node*  _M_next;
  };
```

The bucket array is allocated at once on the heap, but the Nodes are allocated with individual calls to the C++ memory allocator

[Figure 5.1](https://i0.wp.com/codecapsule.com/wp-content/uploads/2013/04/kvstore_unordered_map_web.jpg?w=770&ssl=1) is useful.

***2.2 dense_hash_map from SparseHash***

Offers two hash table implementations : 
- sparse_hash_map : amazing memory footprint  but slow that use sparsetable
- dense_hash_map : handles collisions with quadratic internal probing

[Figure 5.2](https://i0.wp.com/codecapsule.com/wp-content/uploads/2013/04/kvstore_hash_dense_hash_map_web.jpg?w=770&ssl=1) is useful

***2.3 HashDB from Kyoto Cabinet***

The bucket array has a fixed length and is never resized

[Figure 5.3](https://i0.wp.com/codecapsule.com/wp-content/uploads/2013/04/kvstore_hash_kyoto_cabinet_web.jpg?w=770&ssl=1) is useful

When designing the data organization of a hash table, the preferred solution should be to store the collision data with the buckets and not with the entries

## Part 6 Open-Addressing Hash Tables

### 1. Open-addressing hash tables

It minimize the number of I/O operations to access entries

### 2. Comparing different algorithm
Comparing among Linear Probing, Hopscotch hashing, and Robin Hood hashing, Robin Hood hashing has 
- The best DIB(Distance to Initial Bucket which is also calling Probe Sequence Length” (PSL)) 
- The best DMB(Distance to Missing Bucket) - An acceptable DFB(Distance to Free Bucket)

[Visual representation of the DIB, DFB and DMB](https://i0.wp.com/codecapsule.com/wp-content/uploads/2014/05/kvstore-part6-metrics.png?w=530&ssl=1)

(TODO)Need a article to deep understand these algorithm.

## Part 7: Optimizing Data Structures for SSDs

[Fully notes about SSDs](./SSD.md)