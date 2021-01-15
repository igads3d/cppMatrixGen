//
// Created by igads3d on 05.11.2020.
//

#ifndef LIBMATRIXGEN_VECTOR_MATRIX_H_
#define LIBMATRIXGEN_VECTOR_MATRIX_H_
#include "libmatrixgen_common.h"


struct ConstraintPair {
  size_t constraint;
  uint64_t multiplier;
};


template <typename T, size_t dimcount>
class VectorMatrix {
  size_t size {};
  std::vector<ConstraintPair> constraints {};
  std::vector<T> matrix_vec;

 public:
  VectorMatrix(std::initializer_list<size_t> constraints) {
    if (constraints.size() == dimcount) {
	  set_size_and_constraints(constraints);
	  matrix_vec.reserve(size);
	}
    else {
      throw std::invalid_argument("constraints count doesn't match with dimensions count");
    }
  };

  template <typename Callback>
  VectorMatrix(std::initializer_list<size_t> constraints, Callback callback) {
	if (constraints.size() == dimcount) {
	  set_size_and_constraints(constraints);
	  matrix_vec.reserve(size);
	  fill_matrix_with(callback);
	}
	else {
	  throw std::invalid_argument("constraints count doesn't match with dimensions count");
	}
  };

  size_t get_dimensions_count() {
	return dimcount;
  };

  size_t get_dimension_constraint(unsigned dimension) {
    return constraints[dimension].constraint;
  };

  T& value(std::initializer_list<size_t> position) {
    return matrix_vec[pos_to_counter(position)];
  };

  void value(std::initializer_list<size_t> position, T& value) {
	matrix_vec[pos_to_counter(position)] = value;
  };

  void erase() {
    this->size = 0;
    this->constraints.erase();
    this->matrix_vec.erase();
  };

  T& operator[] (std::initializer_list<size_t> position) {
    return matrix_vec[pos_to_counter(position)];
  };

  template<typename Callback>
  void fill_matrix_with(Callback callback) {
    unsigned counter = 0;
    std::vector<std::pair<size_t, size_t>> pos_posmax(dimcount);
    for (auto& i : constraints) {
      pos_posmax[counter].first = 0;
      pos_posmax[counter].second = i.constraint;
    }

    for (auto& item : matrix_vec) {
      item = callback(pos_posmax);
      increment_local_position(pos_posmax);
    }
  }

  void set_size_and_constraints(std::initializer_list<size_t> constraints) {
	size_t size = 1;
	uint64_t multiplier = 1;
	for (auto& i : constraints){
	  if (i > 0) {
		size *= i;
		this->constraints.emplace_back({i, multiplier});
		multiplier *= i;
	  }
	  else {
		this->constraints.erase();
		throw std::invalid_argument("one of the dimensions constrainted to zero");
	  }
	}
	this->size = size;
  }

  ~VectorMatrix() = default;

 private:
  std::vector<size_t> counter_to_pos (uint64_t counter) {
    uint64_t value = 0;
    size_t v_counter = 0;
    std::vector<size_t> position(dimcount);
    std::fill(position.begin(), position.end(), 0);

    for (auto rit = constraints.crbegin(); rit != constraints.crend(); rit++){
      while ( (value + rit->multiplier) < counter ) {
        value += rit->multiplier;
        position[v_counter]++;
      }
      v_counter++;
    }

    std::reverse(position.begin(), position.end());
	return position;
  }

  uint64_t pos_to_counter (std::initializer_list<size_t> pos) {
    unsigned counter = 0;
    uint64_t result = 0;
    for (auto& i : pos){
      result += i*constraints[counter].multiplier;
      counter++;
    }
	return result;
  }

  void increment_local_position (std::vector<std::pair<size_t, size_t>>& position) {
    for (auto& i : position){
	  if (i.first + 1 == i.second) {
		i.first = 0;
	  }
	  else {
		i.first += 1;
		break;
	  }
    }
  }

};


#endif //LIBMATRIXGEN_VECTOR_MATRIX_H_
