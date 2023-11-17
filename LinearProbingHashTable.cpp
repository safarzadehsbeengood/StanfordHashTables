#include "LinearProbingHashTable.h"
#include <iostream>

LinearProbingHashTable::LinearProbingHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
  this->table = new int[numBuckets];    // creates the hash table with the number of buckets
  for (size_t i = 0; i < numBuckets; i++) {
    table[i] = -1;  // initialize with -1 (non-tombstone, empty)
  }
  this->size = numBuckets;              // store the size for this hash table
  this->hash = family->get();           // pick a hash function
  // std::cout << std::endl << "function: " << family->name() << std::endl;
}

LinearProbingHashTable::~LinearProbingHashTable() {
  delete [] table;                      // delete the table
}

void LinearProbingHashTable::insert(int data) {
  int key = hash(data) % this->size;    // grab the hash key

  /* if the value is already in the table, return */
  if (contains(data)) {
    return;
  }

  /* If the slot is taken, do a loop around until you find a free slot */
  if (table[key] > -1) {
    size_t i = key + 1;  // start at the next slot

    /* loop while i < key + size, which represents a full loop around the table */
    while (i < key + size) {

      /* if the slot is empty and not a tombstone, insert the value */
      if (table[i % size] == -1 || table[i % size] == -2) {
        table[i % size] = data;
        return;
      }

      i++;

    }
    return;           // all slots were taken
  } else {
    /* the slot is open */
    table[key] = data;
    return;
  }
  // print();
}

bool LinearProbingHashTable::contains(int data) const {
  int key = hash(data) % this->size;  // grab the key

  /* do a full loop around the table, and return true if the element is found */
  for (size_t i = key; i < key + size; i++) {
    if (table[i % size] == data) {
      return true;
    } else if (table[i % size] == -1) {
      return false;
    }
  }

  /* it wasn't found */
  return false;
}

void LinearProbingHashTable::remove(int data) {
  if (!contains(data)) {
    return;
  }
  int key = hash(data) % this->size;
  if (table[key] == data) {
    table[key] = -2; // (two)mbstone
    return;
  } else {
    size_t i = key;
    while (i < key + size) {

      /* reached a non-tombstone empty slot, which means the value isn't in here */
      if (table[i % size] == -1) {
        return;
      }
      if (table[i % size] == data) {
        table[i % size] = -2; 
        return;
      }
      i++;
    }
  }
}

void LinearProbingHashTable::print() const {
  std::cout << "[ ";
  for (size_t i = 0; i < size; i++) {
    if (table[i] != 0) {
      std::cout << table[i] << " ";
    } else {
      std::cout << "x "; 
    }
  }
  std::cout << "]\n";
 }
