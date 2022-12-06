# KeyValueStore
Implement a Key-Value Store. Thanks to [Emmanuel Goossaert](http://goossaert.com/)'s [Tutorial](https://codecapsule.com/2012/11/07/ikvs-implementing-a-key-value-store-table-of-contents/). Here's some code and notes.

<details>
<summary>Table of notes</summary>

- [Part 1 What are key-value stores and why implement one](#part-1-what-are-key-value-stores-and-why-implement-one)

- [Part 2 Using existing key-value stores as models](#part-2-using-existing-key-value-stores-as-models)

- [Part 3 Comparative Analysis of the Architectures of Kyoto Cabinet and LevelDB](#part-3-comparative-analysis-of-the-architectures-of-kyoto-cabinet-and-leveldb)

- [Part 4 API Design](#part-4-api-design)

  - [1. General principles for API design](#1-general-principles-for-api-design)
  
  - [2. Defining the functionalities for the public API of KingDB](#2-defining-the-functionalities-for-the-public-api-of-kingdb)
  
  - [3. Comparing the APIs of existing databases](#3-comparing-the-apis-of-existing-databases)

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

## Part 4: API Design
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

