#ifndef __NETO_BASE_EVENTCHANNEL_H__
#define __NETO_BASE_EVENTCHANNEL_H__

#include <memory>

namespace neto {
namespace base {
  class EventChannel;

  typedef std::shared_ptr<EventChannel> EventChannelPtr;
  typedef std::weak_ptr<EventChannel> EventChannelWeakPtr;

  // user are expected to implement callback system
  class EventChannel {
    public:
      EventChannel();
      virtual ~EventChannel();

      virtual int fd() const = 0;
      virtual int events() const = 0;
      // when disabled, no event handler will be called even when
      // there are events triggered
      virtual bool disabled() const = 0;

      virtual void handleIN() {}
      virtual void handleOUT() {}
      virtual void handleRDHUP() {}
      virtual void handlePRI() {}
      virtual void handleERR() {}
      virtual void handleHUP() {}
  };

} // namespace base
} // namespace neto

#endif // __NETO_BASE_EVENTCHANNEL_H__
