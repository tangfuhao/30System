#ifndef __LIST_H__
#define __LIST_H__

/* 可変長配列クラス */
template <class T> class List {
public:
	List();
	List(int size);
	List(int size, int increase);
	~List();

public:
	void add(T element);
	T    get(int index) const;
	T    operator[](int index);
	T    removeAt(int index);
	T    remove(T element);
	void removeAll();
	int  size() const;
	bool isEmpty() const;
	int  indexOf(T element) const;

private:
	T*  data_;        /* 初期配列     */
	int size_;        /* 配列のサイズ */
	int elements_;    /* 要素数       */
	int increase_;    /* 増加分       */

	/* 初期化 */
	void init(int size, int increase);
};

/* コンストラクタ */
template <class T> List<T>::List()
{
	init(5, 5);
	return;
}

/* コピーコンストラクタ */
template <class T> List<T>::List(int size)
{
	init(size, 5);
	return;
}

/* コピーコンストラクタ */
template <class T> List<T>::List(int size, int increase)
{
	init(size, increase);
	return;
}

/* デスクトラクタ */
template <class T> List<T>::~List()
{
	/* メモリの解放 */
	delete[] data_;
	return;
}

/* 空かどうかを返す */
template <class T> bool List<T>::isEmpty() const
{
	return elements_ == 0;
}

/* 要素を追加する */
template <class T> void List<T>::add(T element)
{
	/* 配列が一杯になった */
	if (size_ == elements_) {

		/* 配列を2倍にする */
		size_ += increase_;
		T* temp = new T[size_];

		/* 最適化？ */
		int numElements = elements_;

		/* オリジナルの配列を新しい配列にコピー */
		for (int i = 0; i < numElements; i++) {
			temp[i] = data_[i];
		}

		delete[] data_;
		data_ = temp;
	}

	/* 要素の追加 */
	data_[elements_] = element;
	elements_++;
	return;
}

/* i番目の要素を得る */
template <class T> T List<T>::get(int index) const
{
	/* 範囲チェック */
	if (index < 0 || index >=elements_) {
		return (T)NULL;
	}
	return data_[index];
}

/* 配列のようにアクセスする */
template <class T> T List<T>::operator[](int index)
{
	return (this->get(index));
}

/* サイズを得る */
template <class T> int List<T>::size() const
{
	return elements_;
}

/* i番目の要素を削除する */
template <class T> T List<T>::removeAt(int index)
{
	/* 範囲チェック */
	if (index < 0 || index >=elements_) {
		return (T)NULL;
	}

	/* 削除する要素をコピー */
	T toRemove = data_[index];

	/* 空きを埋める */
	int numElements = elements_;
	for (int i = index; i < numElements - 1; i++) {
		data_[i] = data_[i + 1];
	}
	elements_--;
	return toRemove;
}

template <class T> T List<T>::remove(T element)
{
	/* 最適化？ */
	int size = this->size();

	for (int i = 0; i < size; i++) {
		/* 削除する要素が見つかった */
		if (data_[i] == element) {
			return (removeAt(i));
		}
	}

	return (T)NULL;
}

template <class T> void List<T>::removeAll()
{
	/* メモリの解放 */
	delete[] data_;
	init(5, 5);
}

template <class T> void List<T>::init(int size, int increase)
{
	/* 要素数 */
	elements_ = 0;

	/* サイズと増加分を設定 */
	size_     = size     > 0 ? size : 5;
	increase_ = increase > 0 ? increase : 5;

	/* 初期配列を作る */
	data_ = new T[size_];
	return;
}

template <class T> int List<T>::indexOf(T element) const
{
	/* 最適化？ */
	int size = this->size();

	/* 要素を探す */
	for (int i = 0; i < size; i++) {
		/* 要素が見つかった */
		if (data_[i] == element) {
			return i;
		}
	}

	/* 見つからなかった */
	return -1;
}

#endif
