package datastructures

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestArray(t *testing.T) {
	t.Run("NewArray", func(t *testing.T) {
		arr := NewArray("test")
		assert.Equal(t, "test", arr.Name)
		assert.Equal(t, 0, arr.Length())
	})

	t.Run("PushBack and Length", func(t *testing.T) {
		arr := NewArray("test")
		arr.PushBack("a")
		arr.PushBack("b")
		arr.PushBack("c")
		assert.Equal(t, 3, arr.Length())
	})

	t.Run("Get", func(t *testing.T) {
		arr := NewArray("test")
		arr.PushBack("a")
		arr.PushBack("b")
		
		val, err := arr.Get(0)
		require.NoError(t, err)
		assert.Equal(t, "a", val)
		
		val, err = arr.Get(1)
		require.NoError(t, err)
		assert.Equal(t, "b", val)
		
		_, err = arr.Get(2)
		assert.Error(t, err)
		
		_, err = arr.Get(-1)
		assert.Error(t, err)
	})

	t.Run("Insert", func(t *testing.T) {
		arr := NewArray("test")
		arr.PushBack("a")
		arr.PushBack("c")
		
		err := arr.Insert(1, "b")
		require.NoError(t, err)
		assert.Equal(t, 3, arr.Length())
		assert.Equal(t, "a b c", arr.Read())
		
		err = arr.Insert(0, "x")
		require.NoError(t, err)
		assert.Equal(t, "x a b c", arr.Read())
		
		err = arr.Insert(4, "y")
		require.NoError(t, err)
		assert.Equal(t, "x a b c y", arr.Read())
		
		err = arr.Insert(10, "z")
		assert.Error(t, err)
	})

	t.Run("Remove", func(t *testing.T) {
		arr := NewArray("test")
		arr.PushBack("a")
		arr.PushBack("b")
		arr.PushBack("c")
		
		err := arr.Remove(1)
		require.NoError(t, err)
		assert.Equal(t, 2, arr.Length())
		assert.Equal(t, "a c", arr.Read())
		
		err = arr.Remove(0)
		require.NoError(t, err)
		assert.Equal(t, "c", arr.Read())
		
		err = arr.Remove(10)
		assert.Error(t, err)
	})

	t.Run("Replace", func(t *testing.T) {
		arr := NewArray("test")
		arr.PushBack("a")
		arr.PushBack("b")
		arr.PushBack("c")
		
		err := arr.Replace(1, "x")
		require.NoError(t, err)
		assert.Equal(t, "a x c", arr.Read())
		
		err = arr.Replace(10, "y")
		assert.Error(t, err)
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		arr := NewArray("test")
		arr.PushBack("hello")
		arr.PushBack("world")
		arr.PushBack("test")
		
		var buf bytes.Buffer
		err := arr.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newArr := NewArray("new")
		err = newArr.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, arr.data, newArr.data)
	})

	t.Run("SerializeText", func(t *testing.T) {
		arr := NewArray("test")
		arr.PushBack("hello")
		arr.PushBack("world")
		arr.PushBack("test")
		
		var buf bytes.Buffer
		err := arr.SerializeText(&buf)
		require.NoError(t, err)
		
		newArr := NewArray("new")
		err = newArr.DeserializeText(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, arr.data, newArr.data)
	})
}
