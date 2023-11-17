#include "CuckooHashTable.h"

/**
 * Since cuckoo hashing requires two tables, you should create two tables
 * of size numBuckets / 2. Because our testing harness attempts to exercise
 * a number of different load factors, you should not change the number of
 * buckets once the hash table has initially be created.
 */
CuckooHashTable::CuckooHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
  this->size = numBuckets;
  t1.resize(numBuckets / 2);
  for (size_t i = 0; i < t1.size(); i++) { 
    t1[i] = -1;
  }
  t2.resize(numBuckets / 2);
  for (size_t i = 0; i < t2.size(); i++) { 
    t2[i] = -1;
  }
  this->h1 = family->get();
  this->h2 = family->get();
}

CuckooHashTable::~CuckooHashTable() {
}

void CuckooHashTable::insert(int data) {
  /* Get the key for the element. */
  int k1 = h1(data) % (size / 2);
  int k2;

  /* If the slot in the first table is open, place it there. */
  if (t1[k1] == -1) {
    t1[k1] = data;
    return;
  }

  /* Otherwise, displace whoever is there, and try placing that element in t2. */
  int displaced = t1[k1];
  t1[k1] = data;
  int currentTry = 2;

  /* Try to stabilize the tables until you hit the max number of retries. */
  for (size_t i = MAX_RETRIES; i > 0; i--) {

    /* If we're trying to insert into t1, */
    if (currentTry == 1) {

      /* Get the new key, */
      k1 = h1(displaced) % (size / 2);

      /* and check if t1 is open in that slot. If it is, place it there are return. */
      if (t1[k1] == -1) {
        t1[k1] = displaced;
        return;
      } 

      /* Otherwise, kick them out and try to place the new value. */
      else {
        std::swap(displaced, t1[k1]);
        currentTry = 2;
        continue;
      }
    }

    /* If we're trying to insert into t2, */
    else {

      /* Get the new key, */
      k2 = h2(data) % (size / 2);

      /* and check if t2 is open in that slot. If it is, place it there. */
      if (t2[k2] == -1) {
        t2[k2] = displaced;
        return;
      }

      /* Otherwise, kick them out and place the new value, then retry. */
      else {
        std::swap(displaced, t2[k2]);
        currentTry = 1;
        continue;
      }
    }
  }
  /* If this loop finishes without returning, the insertion was a fail. */
  return;
}

bool CuckooHashTable::contains(int data) const {
  int k1 = h1(data) % (size / 2);
  int k2 = h2(data) % (size / 2);
  return ((t1[k1] == data) || (t2[k2] == data));
}

void CuckooHashTable::remove(int data) {
  int k1 = h1(data) % (size / 2);
  int k2 = h2(data) % (size / 2);

  if (t1[k1] == data) {
    t1[k1] = -1;
  } else if (t2[k2] == data) {
    t2[k2] = -1;
  }
  return;
}
