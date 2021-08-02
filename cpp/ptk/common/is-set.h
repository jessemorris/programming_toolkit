#pragma once

#include <memory>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

namespace ptk {

/**
 * @brief Class wrapper to check if a value has been set or not. 
 * 
 * Example usage:
 * is_set<double> v;
 * v.isSet() -> false
 * v = 10.0;
 * v.isSet() -> true
 * v.data() -> 10
 * 
 * By default, data cannot be overwritten so once it is set you cannot set again (TODO(jesse) maybe throw an error
 * if this is attempted?). This behaviour can be changed by setting overwrite to true
 * 
 * Used primarily to avoid the case of having a variable with a "bool X_set" associated with it or by using null ptr's
 * which can be dangerous of not managed carefully.
 * 
 * NOTE: Behaviour not tested with raw or smart pointers. Umdefined behaviour if T is complex and does not have
 * an default constructor eg (T t will throw an error) as T is initalised on the stack. Could avoid by
 * making data_ a unique ptr and moving it around?
 * 
 * @tparam T 
 */
template<typename T>
class is_set {

	public:
		is_set(const bool overwrite_ = false)
			:	can_overwrite(overwrite_),
				set_(false),
				data_() {} //assumes T has a empty constructor (if not make ptr?)

		is_set(const T& t_, const bool overwrite_ = false)
			:	can_overwrite(overwrite_),
				set_(true),
				data_(t_) {} //asumes has good copy constructor?

		is_set(T&& t_, const bool overwrite_ = false)
			:	can_overwrite(overwrite_),
				set_(true),
				data_(t_) {} //asumes has good copy constructor?

		~is_set() = default;


		bool isSet() const {return set_;}
		const T& data() const {return data_;}

		/**
		 * @brief Copy constructor.
		 * 
		 * Only set if other has a value set.
		 * 
		 * @param other 
		 */
		is_set(const is_set& other) {
			if(other.set_) {
				set(other.data_);
			}
		}

		is_set& operator = (const T& t) {
			set(t);
			return *this;
		}

		is_set& operator = (const is_set& other) {
			if(other.set_) {
				set(other.data_);
			}
			return *this;
		}


		is_set operator+(const T& t) {
			//what do when this is not set?
			is_set temp(data_ + t);
			return temp;
		}

		is_set operator+(const is_set& other) {
			//what do when this is not set?
			is_set temp(data_ + other.data());
			return temp;
		}

		is_set operator-(const T& t) {
			//what do when this is not set?
			is_set temp(data_ - t);
			return temp;
		}

		is_set operator-(const is_set& other) {
			//what do when this is not set?
			is_set temp(data_ - other.data());
			return temp;
		}

		friend std::ostream& operator<<(std::ostream& out, const is_set& object) {
			std::string s = object.set_ ? "set" : "not set";
			out << object.data_ << " (" << s << ")" << std::endl;
			return out;
		}


	private:
		void set(const T& t) {
			//if set and !overwrite -> update
			//if not set -> update
			//else dont
			if(set_ && can_overwrite) {
				data_ = t;
				//set should alrady be true
			}
			if(!set_) {
				data_ = t;
				set_ = true;
			}
		}

	private:
		bool can_overwrite;
		bool set_;
		T data_;




};

template<typename T>
is_set<T> operator+(const T &a, const is_set<T> &b) {
	return is_set<T>(a + b.data());
}

template<typename T>
is_set<T> operator-(const T &a, const is_set<T> &b) {
	return is_set<T>(a - b.data());
}

    
} // namespace ptk
