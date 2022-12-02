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