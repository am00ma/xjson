Title: API issues
Author: am00ma
Created: Sun, 29 Mar 2026 11:41:43 +0200
State: closed

- Some macros miss ref to buffer
  - `BufferErrorIf(cond, val)`
  - `BufferCheckCap(cap, val)`

--%--
From: am00ma
Date: Sun, 29 Mar 2026 11:59:28 +0200

For consume and concat, perhaps returning error is enough.
That way, it can start to resemble `sprintf/scanf` combo.
Perhaps, we can even support `%v` like golang.

--%--
From: am00ma
Date: Sun, 29 Mar 2026 11:59:28 +0200

Fixed - just added `p` as argument and ran tests
