// SPDX-FileCopyrightText: (c) 2021-2024 Shawn Silverman <shawn@pobox.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

// ConnectionState.h defines all the data needed to host a connection.
// This file is part of the QNEthernet library.

#pragma once

#include "lwip/opt.h"

#if LWIP_TCP

// C++ includes
#include <functional>
#include <vector>

#include "lwip/altcp.h"

namespace qindesign {
namespace network {
namespace internal {

// ConnectionState holds all the state needed for a connection.
struct ConnectionState final {
  // Creates a new object and sets `arg` as the pcb's arg. This also reserves
  // TCP_WND bytes as buffer space.
  ConnectionState(altcp_pcb *const tpcb, void *const arg)
      : pcb(tpcb),
        buf{},
        bufPos(0),
        removeFunc{} {
    altcp_arg(tpcb, arg);
    buf.reserve(TCP_WND);
  }

  // Sets the callback arg to nullptr and then calls the 'remove' function. The
  // object should be deleted before more 'tcp' functions are called.
  ~ConnectionState() {
    // Ensure callbacks are no longer called with this as the argument
    altcp_arg(pcb, nullptr);

    if (removeFunc != nullptr) {
      removeFunc(this);
    }
  }

  altcp_pcb * /*volatile*/ pcb;

  // Incoming data buffer
  std::vector<uint8_t> buf;
  /*volatile*/ size_t bufPos;

  // Called from the destructor after the callback arg is deleted.
  std::function<void(ConnectionState *)> removeFunc;
};

}  // namespace internal
}  // namespace network
}  // namespace qindesign

#endif  // LWIP_TCP
