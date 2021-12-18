#pragma once

#include <cstdlib>
#include <type_traits>

template<class T>
class Array final
{
public:

	static const int kDefaultCapacity = 8;

	Array() : Array(kDefaultCapacity) {}

	Array(int capacity)
	{
		mCapacity = capacity;
		dataPtr = static_cast<T*>(malloc(capacity * sizeof(T)));
	}

	// copy constructor
	Array(const Array<T>& other) : Array(other.mCapacity)
	{
		for (auto it = other.iterator(); it.hasNext(); it.next())
		{
			insert(it.get());
		}
	}

	// move constructor
	Array(Array<T>&& other)
	{
		this->mCapacity = other.mCapacity;
		this->dataPtr = other.dataPtr;
		this->mSize = other.mSize;

		other.dataPtr = nullptr;
		other.mCapacity = 0;
		other.mSize = 0;
	}

	~Array()
	{
		if (dataPtr != nullptr)
		{
			clear();
		}
	}

	// todo: think about what insert should really return : index or reference to the inserted value?
	int insert(const T& value)
	{
		return insert(mSize, value);
	}

	int insert(int index, const T& value)
	{
		if (mSize >= mCapacity)
		{
			enlargeAndMakeSpace(index);
		}
		else if (index != mSize)
		{
			makeSpace(index);
		}

		++mSize;
		new(&dataPtr[index]) T(value);

		return index;
	}

	void remove(int index)
	{
		dataPtr[index].~T();
		moveR2L(dataPtr + index + 1, dataPtr + index, mSize - index - 1);
		
		--mSize;
	}

	const T& operator[] (int index) const
	{
		return dataPtr[index];
	}

	T& operator[] (int index)
	{
		return dataPtr[index];
	}

	int size() const 
	{
		return mSize; 
	};

private:

	template <class U, class I>
	class IteratorBase
	{
	public:

		using IterableType = U;

		const T& get() const
		{
			return (*arr)[mCurrentIndex];
		}

		void next()
		{
			if (hasNext())
			{
				mCurrentIndex += mIndexDelta;
			}
		}

		// returns true if iterator points to a valid array element
		bool hasNext() const
		{
			return mCurrentIndex != mEnd;
		}

		I& operator++()
		{
			next();
			return *this;
		}

		I operator++(int)
		{
			I iter = *this;
			next();
			return iter;
		}

		const T& operator*() const
		{
			return get();
		}

	protected:

		IteratorBase(IterableType* arr, int indexDelta) : arr(arr), mIndexDelta(indexDelta)
		{
			if (indexDelta > 0)
			{
				mCurrentIndex = 0;
				mEnd = arr->size();
			}
			else if (indexDelta < 0)
			{
				mCurrentIndex = arr->size() - 1;
				mEnd = -1;
			}
			else
			{
				mCurrentIndex = 0;
				mEnd = 0;
			}
		}

	protected:

		int mCurrentIndex = 0;
		int mIndexDelta;
		int mEnd;

		IterableType* arr;
	};

public:

	class Iterator : public IteratorBase<Array<T>, Iterator>
	{
	public:

		void set(const T& value)
		{
			(*arr)[mCurrentIndex] = value;
		}

	private:

		Iterator(IterableType* arr, int indexDelta) : IteratorBase(arr, indexDelta) {}

		friend class Array;
	};

	class ConstIterator : IteratorBase<const Array<T>, ConstIterator>
	{
	private:

		ConstIterator(IterableType* arr, int indexDelta) : IteratorBase(arr, indexDelta) {}

		friend class Array;
	};

	Iterator iterator()
	{
		return Iterator(this, 1);
	}

	ConstIterator iterator() const
	{
		return ConstIterator(this, 1);
	}

	Iterator reverseIterator()
	{
		return Iterator(this, -1);
	}

	ConstIterator reverseIterator() const
	{
		return ConstIterator(this, -1);
	}

private:


	int mCapacity;

	T* dataPtr = nullptr;
	int mSize = 0;

	void enlargeAndMakeSpace(int index)
	{
		mCapacity *= 1.6f;

		T* newDataPtr = static_cast<T*>(malloc(mCapacity * sizeof(T)));

		// move elements before index
		moveL2R(dataPtr, newDataPtr, index);

		// move elements after index
		moveL2R(dataPtr + index, newDataPtr + index + 1, mSize - index);

		free(dataPtr);
		dataPtr = newDataPtr;
	}

	void makeSpace(int index)
	{
		moveL2R(dataPtr + index, dataPtr + index + 1, mSize - index);
	}

	//template<int delta>
	//void move(T* src, T* dest, int num)
	//{
	//	int start;
	//	int onePastEnd;
	//	if constexpr (delta > 0)
	//	{
	//		start = 0;
	//		onePastEnd = num + delta;
	//	}
	//	else if constexpr (delta < 0)
	//	{
	//		start = num - 1;
	//		onePastEnd = delta;
	//	}
	//	else
	//	{
	//		// avoid infinite loop when delta is 0
	//		start = 0;
	//		onePastEnd = 0;
	//	}

	//	if constexpr (std::is_move_constructible<T>())
	//	{
	//		for (int i = start; i != onePastEnd; i += delta)
	//		{
	//			dest[i] = std::move(src[i]);
	//		}
	//	}
	//	else
	//	{
	//		for (int i = start; i != onePastEnd; i += delta)
	//		{
	//			dest[i] = src[i];
	//			src[i].~T();
	//		}
	//	}
	//}

	//typedef void (Array<T>::*MoveType) (T*, T*, int);

	//MoveType const moveL2R = &Array<T>::move<-1>;
	//MoveType const moveR2L = &Array<T>::move<1>;

	//void (Array<T>::*) (T*, T*, int)  MoveL2R = &Array<T>::move<-1>;
	//void (Array<T>::*) (T*, T*, int)  MoveR2L = &Array<T>::move<1>;
	
	void moveL2R(T* src, T* dest, int num)
	{
		if constexpr (std::is_move_constructible<T>())
		{
			for (int i = num - 1; i >= 0; --i)
			{
				new(&dest[i]) T(std::move(src[i]));
			}
		}
		else
		{
			for (int i = num - 1; i >= 0; --i)
			{
				new(&dest[i]) T(src[i]);
				src[i].~T();
			}
		}
	}

	void moveR2L(T* src, T* dest, int num)
	{
		if constexpr (std::is_move_constructible<T>())
		{
			for (int i = 0; i < num; ++i)
			{
				new(&dest[i]) T(std::move(src[i]));
			}
		}
		else
		{
			for (int i = 0; i < num; ++i)
			{
				new(&dest[i]) T(src[i]);
				src[i].~T();
			}
		}
	}

	void clear()
	{
		// destroy elements
		for (int i = 0; i < mSize; ++i)
		{
			dataPtr[i].~T();
		}
		free(dataPtr);

		mSize = 0;
	}
};