#pragma once

/**
 * Iterator wrapper.  This is used to encapsulate iterators into a SourceMod handle.
 */
template <typename T>
class IteratorContainer {
	public:
	IteratorContainer(T* source, typename T::iterator begin, typename T::iterator end):
			_struct(source), _current(begin), _it(begin), _end(end), _current_removed(false) {};
	
	bool Next() {
		// no point advancing the iterator if it's at the end
		if (_it != _end && _current != _end) {
			_current = _it++;
			_current_removed = false;
			
			return _current != _end;
		}
		return false;
	}
	
	void Remove() {
		if (!_current_removed) {
			_current_removed = true;
			_struct->erase(_current);
		}
	}
	
	bool IsRemoved() {
		return _current_removed;
	}
	
	typename T::iterator Current() {
		return _current;
	}
	
	private:
	T* _struct;
	typename T::iterator _current, _it, _end;
	bool _current_removed;
};
