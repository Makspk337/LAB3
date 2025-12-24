package datastructures

import (
	"encoding/binary"
	"fmt"
	"io"
)

type SlotState uint8

const (
	Empty SlotState = iota
	Filled
	Deleted
)

const maxLoadFactor = 0.6

type HashTable struct {
	keys    []int
	values  []int
	states  []SlotState
	capacity int
	count    int
}

func NewHashTable(capacity ...int) *HashTable {
	cap := 8
	if len(capacity) > 0 && capacity[0] > 0 {
		cap = capacity[0]
	}
	
	ht := &HashTable{
		capacity: nextPow2(cap),
	}
	ht.keys = make([]int, ht.capacity)
	ht.values = make([]int, ht.capacity)
	ht.states = make([]SlotState, ht.capacity)
	
	for i := range ht.states {
		ht.states[i] = Empty
	}
	
	return ht
}

func nextPow2(n int) int {
	p := 1
	for p < n {
		p <<= 1
	}
	return p
}

func (ht *HashTable) hash(key int) int {
	x := uint32(key)
	x *= 2654435761
	return int(x & uint32(ht.capacity-1))
}

func (ht *HashTable) rehash(newCapacity int) {
	newCapacity = nextPow2(newCapacity)
	if newCapacity < 2 {
		newCapacity = 2
	}
	
	oldKeys := ht.keys
	oldValues := ht.values
	oldStates := ht.states
	oldCap := ht.capacity
	
	ht.keys = make([]int, newCapacity)
	ht.values = make([]int, newCapacity)
	ht.states = make([]SlotState, newCapacity)
	ht.capacity = newCapacity
	ht.count = 0
	
	for i := range ht.states {
		ht.states[i] = Empty
	}
	
	for i := 0; i < oldCap; i++ {
		if oldStates[i] == Filled {
			ht.Insert(oldKeys[i], oldValues[i])
		}
	}
}

func (ht *HashTable) Insert(key, value int) {
	if float64(ht.count+1)/float64(ht.capacity) > maxLoadFactor {
		ht.rehash(ht.capacity * 2)
	}
	
	idx := ht.hash(key)
	firstDeleted := -1
	
	for probe := 0; probe < ht.capacity; probe++ {
		pos := (idx + probe) & (ht.capacity - 1)
		
		if ht.states[pos] == Filled && ht.keys[pos] == key {
			ht.values[pos] = value
			return
		}
		
		if ht.states[pos] == Deleted && firstDeleted == -1 {
			firstDeleted = pos
		}
		
		if ht.states[pos] == Empty {
			target := pos
			if firstDeleted != -1 {
				target = firstDeleted
			}
			ht.keys[target] = key
			ht.values[target] = value
			ht.states[target] = Filled
			ht.count++
			return
		}
	}
	
	ht.rehash(ht.capacity * 2)
	ht.Insert(key, value)
}

func (ht *HashTable) Contains(key int) bool {
	return ht.Get(key) != -1
}

func (ht *HashTable) Get(key int) int {
	idx := ht.hash(key)
	
	for probe := 0; probe < ht.capacity; probe++ {
		pos := (idx + probe) & (ht.capacity - 1)
		
		if ht.states[pos] == Empty {
			return -1
		}
		
		if ht.states[pos] == Filled && ht.keys[pos] == key {
			return ht.values[pos]
		}
	}
	
	return -1
}

func (ht *HashTable) Remove(key int) {
	idx := ht.hash(key)
	
	for probe := 0; probe < ht.capacity; probe++ {
		pos := (idx + probe) & (ht.capacity - 1)
		
		if ht.states[pos] == Empty {
			return
		}
		
		if ht.states[pos] == Filled && ht.keys[pos] == key {
			ht.states[pos] = Deleted
			ht.count--
			return
		}
	}
}

func (ht *HashTable) Size() int {
	return ht.count
}

func (ht *HashTable) Items() []struct{ Key, Value int } {
	items := make([]struct{ Key, Value int }, 0, ht.count)
	
	for i := 0; i < ht.capacity; i++ {
		if ht.states[i] == Filled {
			items = append(items, struct{ Key, Value int }{
				Key:   ht.keys[i],
				Value: ht.values[i],
			})
		}
	}
	
	return items
}

func (ht *HashTable) SerializeBinary(w io.Writer) error {
	items := ht.Items()
	length := uint32(len(items))
	
	if err := binary.Write(w, binary.LittleEndian, length); err != nil {
		return err
	}
	
	for _, item := range items {
		if err := binary.Write(w, binary.LittleEndian, int32(item.Key)); err != nil {
			return err
		}
		if err := binary.Write(w, binary.LittleEndian, int32(item.Value)); err != nil {
			return err
		}
	}
	
	return nil
}

func (ht *HashTable) DeserializeBinary(r io.Reader) error {
	var length uint32
	if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
		return err
	}
	
	ht.rehash(nextPow2(int(length)*2 + 8))
	
	for i := uint32(0); i < length; i++ {
		var key, value int32
		if err := binary.Read(r, binary.LittleEndian, &key); err != nil {
			return err
		}
		if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
			return err
		}
		
		ht.Insert(int(key), int(value))
	}
	
	return nil
}

func (ht *HashTable) SerializeText(w io.Writer) error {
	items := ht.Items()
	
	if _, err := fmt.Fprintf(w, "%d\n", len(items)); err != nil {
		return err
	}
	
	for _, item := range items {
		if _, err := fmt.Fprintf(w, "%d %d\n", item.Key, item.Value); err != nil {
			return err
		}
	}
	
	return nil
}

func (ht *HashTable) DeserializeText(r io.Reader) error {
	var length int
	if _, err := fmt.Fscanf(r, "%d\n", &length); err != nil {
		return err
	}
	
	ht.rehash(nextPow2(length*2 + 8))
	
	for i := 0; i < length; i++ {
		var key, value int
		if _, err := fmt.Fscanf(r, "%d %d\n", &key, &value); err != nil {
			return err
		}
		
		ht.Insert(key, value)
	}
	
	return nil
}

type CuckooHashTable struct {
	keys1   []int
	vals1   []int
	used1   []bool
	
	keys2   []int
	vals2   []int
	used2   []bool
	
	capacity int
	count    int
}

func NewCuckooHashTable(capacity ...int) *CuckooHashTable {
	cap := 8
	if len(capacity) > 0 && capacity[0] > 0 {
		cap = capacity[0]
	}
	
	capacityInt := 1
	for capacityInt < cap {
		capacityInt <<= 1
	}
	if capacityInt < 2 {
		capacityInt = 2
	}
	
	cht := &CuckooHashTable{
		capacity: capacityInt,
	}
	
	cht.keys1 = make([]int, cht.capacity)
	cht.vals1 = make([]int, cht.capacity)
	cht.used1 = make([]bool, cht.capacity)
	
	cht.keys2 = make([]int, cht.capacity)
	cht.vals2 = make([]int, cht.capacity)
	cht.used2 = make([]bool, cht.capacity)
	
	return cht
}

func (cht *CuckooHashTable) hash1(key int) int {
	x := uint32(key)
	x *= 2654435761
	return int(x & uint32(cht.capacity-1))
}

func (cht *CuckooHashTable) hash2(key int) int {
	x := uint32(key)
	x ^= x >> 16
	x *= 2246822519
	x ^= x >> 13
	return int(x & uint32(cht.capacity-1))
}

func (cht *CuckooHashTable) rehash(newCapacity int) {
	current := cht.Items()
	
	cht.capacity = 1
	for cht.capacity < newCapacity {
		cht.capacity <<= 1
	}
	if cht.capacity < 2 {
		cht.capacity = 2
	}
	
	cht.keys1 = make([]int, cht.capacity)
	cht.vals1 = make([]int, cht.capacity)
	cht.used1 = make([]bool, cht.capacity)
	
	cht.keys2 = make([]int, cht.capacity)
	cht.vals2 = make([]int, cht.capacity)
	cht.used2 = make([]bool, cht.capacity)
	
	cht.count = 0
	
	for _, item := range current {
		cht.Insert(item.Key, item.Value)
	}
}

func (cht *CuckooHashTable) Contains(key int) bool {
	return cht.Get(key) != -1
}

func (cht *CuckooHashTable) Get(key int) int {
	i1 := cht.hash1(key)
	if cht.used1[i1] && cht.keys1[i1] == key {
		return cht.vals1[i1]
	}
	
	i2 := cht.hash2(key)
	if cht.used2[i2] && cht.keys2[i2] == key {
		return cht.vals2[i2]
	}
	
	return -1
}

func (cht *CuckooHashTable) Insert(key, value int) {
	if cht.Contains(key) {
		i1 := cht.hash1(key)
		if cht.used1[i1] && cht.keys1[i1] == key {
			cht.vals1[i1] = value
			return
		}
		
		i2 := cht.hash2(key)
		if cht.used2[i2] && cht.keys2[i2] == key {
			cht.vals2[i2] = value
			return
		}
	}
	
	if float64(cht.count+1)/float64(cht.capacity) > 0.45 {
		cht.rehash(cht.capacity * 2)
	}
	
	curKey := key
	curVal := value
	
	const maxKicks = 32
	for kick := 0; kick < maxKicks; kick++ {
		i1 := cht.hash1(curKey)
		if !cht.used1[i1] {
			cht.used1[i1] = true
			cht.keys1[i1] = curKey
			cht.vals1[i1] = curVal
			cht.count++
			return
		}
		
		curKey, cht.keys1[i1] = cht.keys1[i1], curKey
		curVal, cht.vals1[i1] = cht.vals1[i1], curVal
		
		i2 := cht.hash2(curKey)
		if !cht.used2[i2] {
			cht.used2[i2] = true
			cht.keys2[i2] = curKey
			cht.vals2[i2] = curVal
			cht.count++
			return
		}
		
		curKey, cht.keys2[i2] = cht.keys2[i2], curKey
		curVal, cht.vals2[i2] = cht.vals2[i2], curVal
	}
	
	cht.rehash(cht.capacity * 2)
	cht.Insert(curKey, curVal)
}

func (cht *CuckooHashTable) Remove(key int) {
	i1 := cht.hash1(key)
	if cht.used1[i1] && cht.keys1[i1] == key {
		cht.used1[i1] = false
		cht.count--
		return
	}
	
	i2 := cht.hash2(key)
	if cht.used2[i2] && cht.keys2[i2] == key {
		cht.used2[i2] = false
		cht.count--
		return
	}
}

func (cht *CuckooHashTable) Size() int {
	return cht.count
}

func (cht *CuckooHashTable) Items() []struct{ Key, Value int } {
	items := make([]struct{ Key, Value int }, 0, cht.count)
	
	for i := 0; i < cht.capacity; i++ {
		if cht.used1[i] {
			items = append(items, struct{ Key, Value int }{
				Key:   cht.keys1[i],
				Value: cht.vals1[i],
			})
		}
	}
	
	for i := 0; i < cht.capacity; i++ {
		if cht.used2[i] {
			items = append(items, struct{ Key, Value int }{
				Key:   cht.keys2[i],
				Value: cht.vals2[i],
			})
		}
	}
	
	return items
}

func (cht *CuckooHashTable) SerializeBinary(w io.Writer) error {
	items := cht.Items()
	length := uint32(len(items))
	
	if err := binary.Write(w, binary.LittleEndian, length); err != nil {
		return err
	}
	
	for _, item := range items {
		if err := binary.Write(w, binary.LittleEndian, int32(item.Key)); err != nil {
			return err
		}
		if err := binary.Write(w, binary.LittleEndian, int32(item.Value)); err != nil {
			return err
		}
	}
	
	return nil
}

func (cht *CuckooHashTable) DeserializeBinary(r io.Reader) error {
	var length uint32
	if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
		return err
	}
	
	cht.rehash(int(length)*4 + 8)
	
	for i := uint32(0); i < length; i++ {
		var key, value int32
		if err := binary.Read(r, binary.LittleEndian, &key); err != nil {
			return err
		}
		if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
			return err
		}
		
		cht.Insert(int(key), int(value))
	}
	
	return nil
}

func (cht *CuckooHashTable) SerializeText(w io.Writer) error {
	items := cht.Items()
	
	if _, err := fmt.Fprintf(w, "%d\n", len(items)); err != nil {
		return err
	}
	
	for _, item := range items {
		if _, err := fmt.Fprintf(w, "%d %d\n", item.Key, item.Value); err != nil {
			return err
		}
	}
	
	return nil
}

func (cht *CuckooHashTable) DeserializeText(r io.Reader) error {
	var length int
	if _, err := fmt.Fscanf(r, "%d\n", &length); err != nil {
		return err
	}
	
	cht.rehash(length*4 + 8)
	
	for i := 0; i < length; i++ {
		var key, value int
		if _, err := fmt.Fscanf(r, "%d %d\n", &key, &value); err != nil {
			return err
		}
		
		cht.Insert(key, value)
	}
	
	return nil
}
