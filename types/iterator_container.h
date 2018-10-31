#pragma once

/**
 * Iterator wrapper.  This is used to encapsulate iterators into a SourceMod handle.
 */
template <typename T>
class IteratorContainer {
	public:
	IteratorContainer(T* source, typename T::iterator begin, typename T::iterator end):
			_struct(source), _it(begin), _end(end), _removed(false) {};
	
	bool Next() {
		// TODO implement a way to mark the current entry as deleted
		if (_removed) {
			_struct->erase(_current);
			_removed = false;
		}
		_current = _it;
		return _it++ != _end;
	}
	
	void MarkRemoved() {
		_removed = true;
	}
	
	typename T::iterator Current() {
		return _current;
	}
	
	private:
	T* _struct;
	typename T::iterator _current, _it, _end;
	bool _removed;
};
