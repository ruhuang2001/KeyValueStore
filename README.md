# KeyValueStore
Implement a Key-Value Store. Thanks to [Emmanuel Goossaert](http://goossaert.com/)'s [Tutorial](https://codecapsule.com/2012/11/07/ikvs-implementing-a-key-value-store-table-of-contents/). Here's some code and notes.

## Part 1 What are key-value stores, and why implement one
Key-value stores generally share the following interface
- Get(key)
- Set(key, value)
- Delete(key)

Its performance depends on:
- The hardware.
- The file system being used.
- The actual application and order in which the keys are being accessed([locality of reference](http://en.wikipedia.org/wiki/Locality_of_reference))
- The data set

eg: [Redis](https://github.com/redis/redis), [MongoDB](https://github.com/mongodb/mongo), [LevelDB](https://github.com/google/leveldb)...

## Part 2 Using existing key-value stores as models
Refer to LevelDB
- It is based on a Log-Structured Merge Tree(LSM)

## Part 3 Comparative Analysis of the Architectures of Kyoto Cabinet and LevelDB

Components of a Key-Value Store
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

LevelDB is using a class called "Slice". 
- A Slice holds a byte array along with the size that array.
- know the size of the string in time O(1).
- the Slice class handles the copy by making a shallow copy.It simply copies the pointer to the byte array.

LevelDB is not using exceptions at all, but a special class called Status. This class holds both an error value and an error message.

## Part 4: API Design


