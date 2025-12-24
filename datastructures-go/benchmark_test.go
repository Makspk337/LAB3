package datastructures

import (
	"testing"
)

func BenchmarkArrayPushBack(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				arr := NewArray("benchmark")
				for j := 0; j < size; j++ {
					arr.PushBack("value")
				}
				_ = arr.Length()
			}
		})
	}
}

func BenchmarkDoublyListPushTailAndFind(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				dl := NewDoublyList("benchmark")
				for j := 0; j < size; j++ {
					dl.PushTail("value")
				}
				_ = dl.Find("value")
			}
		})
	}
}

func BenchmarkForwardListPushTailAndFind(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				fl := NewForwardList("benchmark")
				for j := 0; j < size; j++ {
					fl.PushTail("value")
				}
				_ = fl.Find("value")
			}
		})
	}
}

func BenchmarkQueuePushPop(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				q := NewQueue("benchmark")
				for j := 0; j < size; j++ {
					q.Push("value")
				}
				for !q.IsEmpty() {
					_ = q.Pop()
				}
			}
		})
	}
}

func BenchmarkStackPushPop(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				s := NewStack("benchmark")
				for j := 0; j < size; j++ {
					s.Push("value")
				}
				for j := 0; j < size; j++ {
					_ = s.Pop()
				}
			}
		})
	}
}

func BenchmarkHashTableInsertGet(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				ht := NewHashTable(size)
				for j := 0; j < size; j++ {
					ht.Insert(j, j)
				}
				for j := 0; j < size; j++ {
					_ = ht.Get(j)
				}
			}
		})
	}
}

func BenchmarkCuckooHashTableInsertGet(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				cht := NewCuckooHashTable(size)
				for j := 0; j < size; j++ {
					cht.Insert(j, j)
				}
				for j := 0; j < size; j++ {
					_ = cht.Get(j)
				}
			}
		})
	}
}

func BenchmarkTreeInsertFind(b *testing.B) {
	for _, size := range []int{8, 16, 32, 64, 128, 256, 512, 1024} {
		b.Run("size="+string(rune(size)), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				t := NewTree("benchmark")
				for j := 0; j < size; j++ {
					t.Insert(j)
				}
				_ = t.Find(size / 2)
			}
		})
	}
}
