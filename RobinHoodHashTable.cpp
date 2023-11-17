#include "RobinHoodHashTable.h"

RobinHoodHashTable::RobinHoodHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
  this->table = new Entry[numBuckets];
  this->size = numBuckets;
  for (size_t i = 0; i < size; i++) {
    table[i].data = -1;       // null data
    table[i].distance = -1;   // null distance
  }
  this->hash = family->get();
}

RobinHoodHashTable::~RobinHoodHashTable() {
  delete [] table;
}

void RobinHoodHashTable::insert(int data) {

  /* If the table has this element, stop. */
  if (contains(data)) {
    return;
  }

  int key = hash(data) % this->size;

  /* If the slot is empty, insert it here, with no distance. */
  if (table[key].data == -1) {
    table[key].data = data;
    table[key].distance = 0;
    return;
  } 
  
  /* Otherwise, loop through and find where it should go. */
  else {
    int d = 1;
    size_t i = key + 1;
    int insertValue = data;

    /* Loop through the table. */
    while (i < key + size) {

      /* If we found an empty slot, insert it there. */
      if (table[i % size].data == -1) {
        table[i % size].data = insertValue;
        table[i % size].distance = d;
        return;
      }

      /* If the running distance is greater than the current slot's distance, swap them. */
      if (d > table[i % size].distance) {
        /* Swap the data elements.*/
        std::swap(insertValue, table[i % size].data);
        /* Swap their distances. */
        std::swap(d, table[i % size].distance);
      }

      i++;
      d++;

    }

    return;     // insert was unsuccessful

  }
}

bool RobinHoodHashTable::contains(int data) const {
  /* Get the key. */
  int key = hash(data) % this->size;

  /* If the key is where it's supposed to be, return true. */
  if (table[key].data == data) {
    return true;
  } 
  
  /* If not, start at the next slot. */
  size_t i = key + 1;
  int d = 1;

  /* Loop through the table, up to the point before the original slot. */
  while (i < key + size) {
    /* If the current entry is what we're looking for, it was found. */
    if (table[i % size].data == data) {
      return true;
    }

    /* If our running distance is greater than the distance at this slot, 
    the element can't be in the table. */
    if (d > table[i % size].distance) {
      return false;
    }

    i++;
    d++;

  }
  /* Wasn't found. */
  return false;
}

/**
 * You should implement this operation using backward-shift deletion: once
 * you've found the element to remove, continue scanning forward until you
 * find an element that is at its home location or an empty cell, then shift
 * each element up to that point backwards by one step.
 */
void RobinHoodHashTable::remove(int data) {
  /* if the value isn't in the table, return */
  if (!contains(data)) {
    return;
  }

  int key = hash(data) % this->size;
  size_t i = key;
  /* Loop through the table until we find the element, since it must be in the table. */
  while (i < key + size) {
    /* We've found the element to remove. */
    if (table[i % size].data == data) {
      break;
    }
    i++;
  }

  /* get the "true" index from i to make things easier. */
  i %= size;

  /* Make this entry empty. */
  table[i].data = -1;
  table[i].distance = -1;

  /* Keep track of the next entry. */
  int j = i + 1;

  /* Loop while the next entry isn't empty or in its home spot. */
  while (table[j % size].data != -1 && table[j % size].distance != 0) {
    /* Swap the entries. table[i % size] should always be an empty entry, since we deleted it. */
    std::swap(table[i % size], table[j % size]);

    /**
     * Decrement the distance of the node we just moved up.
     * table[j % size] should be an empty entry.
    */
    table[i % size].distance--;
    i++;
    j++;
  }
  return;
}

void RobinHoodHashTable::print() const {
  std::cout << "*\n";
  for (size_t i = 0; i < size; i++) {
    std::cout << "[ " << table[i].data << ", d = " << table[i].distance << " ]" << std::endl;
  }
  std::cout << "*\n";
 }