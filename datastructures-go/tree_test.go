package datastructures

import (
	"bytes"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestTree(t *testing.T) {
	t.Run("Insert and Find", func(t *testing.T) {
		tree := NewTree("test")
		
		tree.Insert(50)
		tree.Insert(30)
		tree.Insert(70)
		tree.Insert(20)
		tree.Insert(40)
		tree.Insert(60)
		tree.Insert(80)
		
		assert.True(t, tree.Find(50))
		assert.True(t, tree.Find(30))
		assert.True(t, tree.Find(70))
		assert.True(t, tree.Find(20))
		assert.True(t, tree.Find(40))
		assert.True(t, tree.Find(60))
		assert.True(t, tree.Find(80))
		assert.False(t, tree.Find(10))
		assert.False(t, tree.Find(90))
	})

	t.Run("Read preorder", func(t *testing.T) {
		tree := NewTree("test")
		
		tree.Insert(50)
		tree.Insert(30)
		tree.Insert(70)
		tree.Insert(20)
		tree.Insert(40)
		tree.Insert(60)
		tree.Insert(80)
		
		result := tree.Read()
		expected := "50 30 20 40 70 60 80"
		assert.Equal(t, expected, result)
	})

	t.Run("IsFull", func(t *testing.T) {
		// Полное дерево (все узлы имеют 0 или 2 детей)
		tree1 := NewTree("test1")
		tree1.Insert(50)
		tree1.Insert(30)
		tree1.Insert(70)
		tree1.Insert(20)
		tree1.Insert(40)
		tree1.Insert(60)
		tree1.Insert(80)
		
		assert.True(t, tree1.IsFull())
		
		// Неполное дерево (у узла 30 есть только левый ребенок 20)
		tree2 := NewTree("test2")
		tree2.Insert(50)
		tree2.Insert(30)
		tree2.Insert(70)
		tree2.Insert(20)
		
		assert.False(t, tree2.IsFull())
		
		// Дерево с 3 узлами (корень имеет 2 детей - это полно)
		tree3 := NewTree("test3")
		tree3.Insert(50)
		tree3.Insert(30)
		tree3.Insert(70)
		
		assert.True(t, tree3.IsFull())
		
		// Один узел (лист - это полно)
		tree4 := NewTree("test4")
		tree4.Insert(50)
		
		assert.True(t, tree4.IsFull())
		
		// Пустое дерево
		tree5 := NewTree("test5")
		assert.True(t, tree5.IsFull())
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		tree := NewTree("test")
		
		tree.Insert(50)
		tree.Insert(30)
		tree.Insert(70)
		tree.Insert(20)
		tree.Insert(40)
		tree.Insert(60)
		tree.Insert(80)
		
		var buf bytes.Buffer
		err := tree.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newTree := NewTree("new")
		err = newTree.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, tree.Read(), newTree.Read())
		assert.Equal(t, tree.IsFull(), newTree.IsFull())
		
		for _, value := range []int{20, 30, 40, 50, 60, 70, 80} {
			assert.Equal(t, tree.Find(value), newTree.Find(value))
		}
	})

	t.Run("SerializeText", func(t *testing.T) {
		tree := NewTree("test")
		
		tree.Insert(50)
		tree.Insert(30)
		tree.Insert(70)
		tree.Insert(20)
		tree.Insert(40)
		tree.Insert(60)
		tree.Insert(80)
		
		var buf bytes.Buffer
		err := tree.SerializeText(&buf)
		require.NoError(t, err)
		
		newTree := NewTree("new")
		err = newTree.DeserializeText(strings.NewReader(buf.String()))
		require.NoError(t, err)
		
		assert.Equal(t, tree.Read(), newTree.Read())
		assert.Equal(t, tree.IsFull(), newTree.IsFull())
		
		for _, value := range []int{20, 30, 40, 50, 60, 70, 80} {
			assert.Equal(t, tree.Find(value), newTree.Find(value))
		}
	})

	t.Run("Empty tree", func(t *testing.T) {
		tree := NewTree("test")
		
		assert.Equal(t, "", tree.Read())
		assert.True(t, tree.IsFull())
		assert.False(t, tree.Find(10))
		
		var buf bytes.Buffer
		err := tree.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newTree := NewTree("new")
		err = newTree.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, "", newTree.Read())
		assert.True(t, newTree.IsFull())
	})

	t.Run("Single node tree", func(t *testing.T) {
		tree := NewTree("test")
		tree.Insert(100)
		
		assert.Equal(t, "100", tree.Read())
		assert.True(t, tree.IsFull())
		assert.True(t, tree.Find(100))
		assert.False(t, tree.Find(50))
		
		var buf bytes.Buffer
		err := tree.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newTree := NewTree("new")
		err = newTree.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, tree.Read(), newTree.Read())
		assert.Equal(t, tree.IsFull(), newTree.IsFull())
	})
}
