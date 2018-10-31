#pragma once

/**
 * Iterator wrapper.  This is used to encapsulate iterators into a SourceMod handle.
 */
template <typename T>
class IteratorContainer {
	public:
	IteratorContainer(typename T::iterator begin, typename T::iterator end):
			_it(begin), _end(end) {};
	
	bool Next() {
		// TODO implement a way to mark the current entry as deleted
		// if (_deleted) {
		//     _struct->erase(_current);
		// }
		_current = _it;
		return _it++ != _end;
	}
	
	typename T::iterator Current() {
		return _current;
	}
	
	private:
	typename T::iterator _current, _it, _end;
};
