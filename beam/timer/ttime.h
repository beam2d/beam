#pragma once

namespace beam {

typedef long long sec_t;
typedef long long msec_t;
typedef long long usec_t;

class ttime {
  usec_t t_;

 public:
  ttime() {}
  ttime(const ttime& tt) : t_(tt.t_) {}
  explicit ttime(usec_t t) : t_(t) {}
  ttime& operator=(const ttime& tt) {
    t_ = tt.t_;
    return *this;
  }

  ttime operator+(ttime tt) const {
    return ttime(t_ + tt.t_);
  }
  ttime operator-(ttime tt) const {
    return ttime(t_ - tt.t_);
  }
  double operator/(ttime tt) const {
    return static_cast<double>(t_) / tt.t_;
  }

  bool operator==(ttime tt) const {
    return t_ == tt.t_;
  }
  bool operator!=(ttime tt) const {
    return !operator==(tt);
  }

  bool operator<(ttime tt) const {
    return t_ < tt.t_;
  }
  bool operator<=(ttime tt) const {
    return t_ <= tt.t_;
  }
  bool operator>=(ttime tt) const {
    return t_ >= tt.t_;
  }
  bool operator>(ttime tt) const {
    return t_ > tt.t_;
  }

  sec_t sec() const {
    return t_ / 1000000;
  }
  msec_t msec() const {
    return t_ / 1000;
  }
  usec_t usec() const {
    return t_;
  }
};

}  // namespace beam

