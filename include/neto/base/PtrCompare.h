#ifndef __NETO_BASE_PTRCOMPARE_H__
#define __NETO_BASE_PTRCOMPARE_H__

#include <memory>

namespace neto {
namespace base {
  template<typename T>
  class PtrGreater{
    public:
      bool operator()(T &a, T &b) {
        if (*a > *b) {
          return true;
        }

        return false;
      }
  };

  template<typename T>
  class PtrLess{
    public:
      bool operator()(T &a, T &b) {
        if (*a < *b) {
          return true;
        }

        return false;
      }
  };

  template<typename T>
  class PtrEqual{
    public:
      bool operator()(T &a, T &b) {
        if (*a == *b) {
          return true;
        }

        return false;
      }
  };

  template<typename T>
  class PtrGreateEqual{
    public:
      bool operator()(T &a, T &b) {
        if (*a >= *b) {
          return true;
        }

        return false;
      }
  };

  template<typename T>
  class PtrLessEqual{
    public:
      bool operator()(T &a, T &b) {
        if (*a <= *b) {
          return true;
        }

        return false;
      }
  };

}
}

#endif // __NETO_BASE_PTRCOMPARE_H__
