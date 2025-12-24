package datastructures

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestHashTable(t *testing.T) {
	t.Run("Insert and Get", func(t *testing.T) {
		ht := NewHashTable()
		
		ht.Insert(1, 100)
		ht.Insert(2, 200)
		ht.Insert(3, 300)
		
		assert.Equal(t, 100, ht.Get(1))
		assert.Equal(t, 200, ht.Get(2))
		assert.Equal(t, 300, ht.Get(3))
		assert.Equal(t, -1, ht.Get(4))
	})

	t.Run("Update existing key", func(t *testing.T) {
		ht := NewHashTable()
		
		ht.Insert(1, 100)
		assert.Equal(t, 100, ht.Get(1))
		
		ht.Insert(1, 200)
		assert.Equal(t, 200, ht.Get(1))
	})

	t.Run("Contains", func(t *testing.T) {
		ht := NewHashTable()
		
		ht.Insert(1, 100)
		ht.Insert(2, 200)
		
		assert.True(t, ht.Contains(1))
		assert.True(t, ht.Contains(2))
		assert.False(t, ht.Contains(3))
	})

	t.Run("Remove", func(t *testing.T) {
		ht := NewHashTable()
		
		ht.Insert(1, 100)
		ht.Insert(2, 200)
		ht.Insert(3, 300)
		
		assert.True(t, ht.Contains(1))
		assert.True(t, ht.Contains(2))
		assert.True(t, ht.Contains(3))
		
		ht.Remove(2)
		assert.True(t, ht.Contains(1))
		assert.False(t, ht.Contains(2))
		assert.True(t, ht.Contains(3))
		
		ht.Remove(1)
		assert.False(t, ht.Contains(1))
		assert.False(t, ht.Contains(2))
		assert.True(t, ht.Contains(3))
		
		ht.Remove(3)
		assert.False(t, ht.Contains(1))
		assert.False(t, ht.Contains(2))
		assert.False(t, ht.Contains(3))
		
		ht.Remove(99)
	})

	t.Run("Size", func(t *testing.T) {
		ht := NewHashTable()
		
		assert.Equal(t, 0, ht.Size())
		
		ht.Insert(1, 100)
		assert.Equal(t, 1, ht.Size())
		
		ht.Insert(2, 200)
		assert.Equal(t, 2, ht.Size())
		
		ht.Insert(1, 300)
		assert.Equal(t, 2, ht.Size())
		
		ht.Remove(2)
		assert.Equal(t, 1, ht.Size())
		
		ht.Remove(1)
		assert.Equal(t, 0, ht.Size())
	})

	t.Run("Items", func(t *testing.T) {
		ht := NewHashTable()
		
		ht.Insert(1, 100)
		ht.Insert(2, 200)
		ht.Insert(3, 300)
		
		items := ht.Items()
		assert.Equal(t, 3, len(items))
		
		itemsMap := make(map[int]int)
		for _, item := range items {
			itemsMap[item.Key] = item.Value
		}
		
		assert.Equal(t, 100, itemsMap[1])
		assert.Equal(t, 200, itemsMap[2])
		assert.Equal(t, 300, itemsMap[3])
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		ht := NewHashTable()
		
		ht.Insert(1, 100)
		ht.Insert(2, 200)
		ht.Insert(3, 300)
		
		var buf bytes.Buffer
		err := ht.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newHt := NewHashTable()
		err = newHt.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, ht.Size(), newHt.Size())
		assert.Equal(t, ht.Get(1), newHt.Get(1))
		assert.Equal(t, ht.Get(2), newHt.Get(2))
		assert.Equal(t, ht.Get(3), newHt.Get(3))
	})

	t.Run("SerializeText", func(t *testing.T) {
		ht := NewHashTable()
		
		ht.Insert(1, 100)
		ht.Insert(2, 200)
		ht.Insert(3, 300)
		
		var buf bytes.Buffer
		err := ht.SerializeText(&buf)
		require.NoError(t, err)
		
		newHt := NewHashTable()
		err = newHt.DeserializeText(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, ht.Size(), newHt.Size())
		assert.Equal(t, ht.Get(1), newHt.Get(1))
		assert.Equal(t, ht.Get(2), newHt.Get(2))
		assert.Equal(t, ht.Get(3), newHt.Get(3))
	})

	t.Run("Rehash", func(t *testing.T) {
		ht := NewHashTable(2)
		
		for i := 0; i < 20; i++ {
			ht.Insert(i, i*10)
		}
		
		assert.Equal(t, 20, ht.Size())
		
		for i := 0; i < 20; i++ {
			assert.Equal(t, i*10, ht.Get(i))
		}
	})
}

func TestCuckooHashTable(t *testing.T) {
	t.Run("Insert and Get", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		cht.Insert(1, 100)
		cht.Insert(2, 200)
		cht.Insert(3, 300)
		
		assert.Equal(t, 100, cht.Get(1))
		assert.Equal(t, 200, cht.Get(2))
		assert.Equal(t, 300, cht.Get(3))
		assert.Equal(t, -1, cht.Get(4))
	})

	t.Run("Update existing key", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		cht.Insert(1, 100)
		assert.Equal(t, 100, cht.Get(1))
		
		cht.Insert(1, 200)
		assert.Equal(t, 200, cht.Get(1))
	})

	t.Run("Contains", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		cht.Insert(1, 100)
		cht.Insert(2, 200)
		
		assert.True(t, cht.Contains(1))
		assert.True(t, cht.Contains(2))
		assert.False(t, cht.Contains(3))
	})

	t.Run("Remove", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		cht.Insert(1, 100)
		cht.Insert(2, 200)
		cht.Insert(3, 300)
		
		assert.True(t, cht.Contains(1))
		assert.True(t, cht.Contains(2))
		assert.True(t, cht.Contains(3))
		
		cht.Remove(2)
		assert.True(t, cht.Contains(1))
		assert.False(t, cht.Contains(2))
		assert.True(t, cht.Contains(3))
		
		cht.Remove(1)
		assert.False(t, cht.Contains(1))
		assert.False(t, cht.Contains(2))
		assert.True(t, cht.Contains(3))
		
		cht.Remove(3)
		assert.False(t, cht.Contains(1))
		assert.False(t, cht.Contains(2))
		assert.False(t, cht.Contains(3))
		
		cht.Remove(99)
	})

	t.Run("Size", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		assert.Equal(t, 0, cht.Size())
		
		cht.Insert(1, 100)
		assert.Equal(t, 1, cht.Size())
		
		cht.Insert(2, 200)
		assert.Equal(t, 2, cht.Size())
		
		cht.Insert(1, 300)
		assert.Equal(t, 2, cht.Size())
		
		cht.Remove(2)
		assert.Equal(t, 1, cht.Size())
		
		cht.Remove(1)
		assert.Equal(t, 0, cht.Size())
	})

	t.Run("Items", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		cht.Insert(1, 100)
		cht.Insert(2, 200)
		cht.Insert(3, 300)
		
		items := cht.Items()
		assert.Equal(t, 3, len(items))
		
		itemsMap := make(map[int]int)
		for _, item := range items {
			itemsMap[item.Key] = item.Value
		}
		
		assert.Equal(t, 100, itemsMap[1])
		assert.Equal(t, 200, itemsMap[2])
		assert.Equal(t, 300, itemsMap[3])
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		cht.Insert(1, 100)
		cht.Insert(2, 200)
		cht.Insert(3, 300)
		
		var buf bytes.Buffer
		err := cht.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newCht := NewCuckooHashTable()
		err = newCht.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, cht.Size(), newCht.Size())
		assert.Equal(t, cht.Get(1), newCht.Get(1))
		assert.Equal(t, cht.Get(2), newCht.Get(2))
		assert.Equal(t, cht.Get(3), newCht.Get(3))
	})

	t.Run("SerializeText", func(t *testing.T) {
		cht := NewCuckooHashTable()
		
		cht.Insert(1, 100)
		cht.Insert(2, 200)
		cht.Insert(3, 300)
		
		var buf bytes.Buffer
		err := cht.SerializeText(&buf)
		require.NoError(t, err)
		
		newCht := NewCuckooHashTable()
		err = newCht.DeserializeText(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, cht.Size(), newCht.Size())
		assert.Equal(t, cht.Get(1), newCht.Get(1))
		assert.Equal(t, cht.Get(2), newCht.Get(2))
		assert.Equal(t, cht.Get(3), newCht.Get(3))
	})

	t.Run("Rehash", func(t *testing.T) {
		cht := NewCuckooHashTable(2)
		
		for i := 0; i < 20; i++ {
			cht.Insert(i, i*10)
		}
		
		assert.Equal(t, 20, cht.Size())
		
		for i := 0; i < 20; i++ {
			assert.Equal(t, i*10, cht.Get(i))
		}
	})
}
