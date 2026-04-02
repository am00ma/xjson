Title: Parse strings properly as per JSON spec (with escapes)
Author: am00ma
Created: Sun, 29 Mar 2026 18:45:55 +0200
State: closed

Currently only skips till next quote, so even escaping is impossible.

--%--
Author: am00ma
Date: Thu, 02 Apr 2026 13:46:57 +0200

Fixed: created `consume_json_string` function with tests.
