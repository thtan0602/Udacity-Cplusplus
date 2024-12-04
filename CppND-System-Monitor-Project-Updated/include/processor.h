#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

  // Private members
 private:
  float previous_total;
  float previous_idle;
};

#endif