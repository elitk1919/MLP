//============================================================================
// Name : Node.h
// Author : David Nogueira
//============================================================================
#ifndef NODE_H
#define NODE_H

#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert> // for assert()

#define CONSTANT_WEIGHT_INITIALIZATION 0

class Node {
public:
  Node() {
    m_num_inputs = 0;
    m_bias = 0.0;
    m_weights.clear();
  };
  Node(int num_inputs,
       bool use_constant_weight_init = true,
       double constant_weight_init = 0.5) {
    m_num_inputs = num_inputs;
    m_bias = 0.0;
    m_weights.clear();
    //initialize weight vector
    WeightInitialization(m_num_inputs,
                         use_constant_weight_init,
                         constant_weight_init);
  };

  ~Node() {
    m_num_inputs = 0;
    m_bias = 0.0;
    m_weights.clear();
  };

  void WeightInitialization(int num_inputs,
                            bool use_constant_weight_init = true,
                            double constant_weight_init = 0.5) {
    m_num_inputs = num_inputs;
    //initialize weight vector
    if (use_constant_weight_init) {
      m_weights.resize(m_num_inputs, constant_weight_init);
    } else {
      m_weights.resize(m_num_inputs);
      std::generate_n(m_weights.begin(),
                     m_num_inputs,
                      utils::gen_rand());
    }
  }

  int GetInputSize() const {
    return m_num_inputs;
  }

  void SetInputSize(int num_inputs) {
    m_num_inputs = num_inputs;
  }

  double GetBias() const {
    return m_bias;
  }
  void SetBias(double bias) {
    m_bias = bias;
  }

  std::vector<double> & GetWeights() {
    return m_weights;
  }

  const std::vector<double> & GetWeights() const {
    return m_weights;
  }

  size_t GetWeightsVectorSize() const {
    return m_weights.size();
  }

  void GetInputInnerProdWithWeights(const std::vector<double> &input,
                                    double * output) const {
    assert(input.size() == m_weights.size());
    double inner_prod = std::inner_product(begin(input),
                                           end(input),
                                           begin(m_weights),
                                           0.0);
    *output = inner_prod;
  }

  void GetOutputAfterSigmoid(const std::vector<double> &input,
                             double * output) const {
    double inner_prod = 0.0;
    GetInputInnerProdWithWeights(input, &inner_prod);
    *output = utils::sigmoid(inner_prod);
  }

  void GetBooleanOutput(const std::vector<double> &input,
                        bool * bool_output,
                        double threshold = 0.5) const {
    double value;
    GetOutputAfterSigmoid(input, &value);
    *bool_output = (value >threshold) ? true : false;
  };

  void UpdateWeights(const std::vector<double> &x,
                     double error,
                     double learning_rate) {
    assert(x.size() == m_weights.size());
    for (size_t i = 0; i < m_weights.size(); i++)
      m_weights[i] += x[i] * learning_rate *  error;
  };

  void UpdateWeight(int weight_id,
                    double increment,
                    double learning_rate) {
    m_weights[weight_id] += learning_rate*increment;
  }

protected:
  int m_num_inputs{ 0 };
  double m_bias{ 0.0 };
  std::vector<double> m_weights;
};

#endif //NODE_H